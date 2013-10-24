#include "CompositeGrid.h"

bool CompositeGrid::clip_tiles = false;
bool CompositeGrid::center_view = false;
Shader CompositeGrid::grid_shader = Shader("VS Grid Shader.glsl", "FS Grid Shader.glsl");
Texture CompositeGrid::frame_image = Texture();

CompositeGrid::CompositeGrid(int x_size, int y_size, int l, int r, int b, int t) :
    x(x_size),
    y(y_size),
    left(l),
    right(r),
    bottom(b),
    top(t)
{
    counter = 0;
    buffers = BufferManager(); 
    //frame = FrameBufferTexture(l, b, r - l, t - b);

    // Used to find midpoints of the current square
    float mid_x = (l + r) / 2;
    float mid_y = (b + t) / 2;

    if (x > 1 && y > 1)
    {
        is_leaf = false;
        // Recursively construct subtiles
        children = std::vector<CompositeGrid*>();
        // Bottom left subtile
        children.push_back(new CompositeGrid(x / 2, y / 2, l, mid_x, b, mid_y));
        // Bottom right subtile
        children.push_back(new CompositeGrid(x / 2, y / 2, mid_x, r, b, mid_y) );
        // Top left subtile
        children.push_back(new CompositeGrid(x / 2, y / 2, l, mid_x, mid_y, t) );
        // Top right subtile
        children.push_back(new CompositeGrid(x / 2, y / 2, mid_x, r, mid_y, t) );
    } else
    {
        is_leaf = true;
    }
}

CompositeGrid::CompositeGrid()
{
}

CompositeGrid::CompositeGrid(const CompositeGrid& copy) :
    is_leaf(copy.is_leaf),
    x(copy.x),
    y(copy.y),
    left(copy.left),
    right(copy.right),
    bottom(copy.bottom),
    top(copy.top),
    buffers(copy.buffers)
    //frame(copy.frame)
{
    meshes = copy.meshes;
    children = copy.children;
}

CompositeGrid& CompositeGrid::operator=(const CompositeGrid& rhs)
{
    is_leaf = rhs.is_leaf;
    x = rhs.x;
    y = rhs.y;
    left = rhs.left;
    right= rhs.right;
    bottom = rhs.bottom;
    top = rhs.top;
    buffers = rhs.buffers;
    children = rhs.children;
    meshes = rhs.meshes;
    //frame = rhs.frame;

    return *this;
}


CompositeGrid::~CompositeGrid(void)
{
    //TODO: Clean memory
    //for (int i = 0; i < children.size(); i++)
    //	delete children[i];
}

void CompositeGrid::init()
{
    grid_shader = Shader("VS Grid Shader.glsl", "FS Grid Shader.glsl");

    float* vertex_data = new float[6 * (x + 1) + 6 * (y + 1)];
    float x_width = 2.0 / (float) x;
    float y_width = 2.0 / (float) y;

    int k = 0;
    float x_coord = -1;
    float y_coord = -1;
    // Fill vertex_data with all vertex locations

    for (int j = 0; j < y + 1; j++)
    {
        vertex_data[k++] = -1;
        vertex_data[k++] = y_coord;
        vertex_data[k++] = 0;

        vertex_data[k++] = 1.0;
        vertex_data[k++] = y_coord;
        vertex_data[k++] = 0.0;

        y_coord += y_width;
    }
    for (int j = 0; j < x + 1; j++)
    {
        vertex_data[k++] = x_coord;
        vertex_data[k++] = -1;
        vertex_data[k++] = 0.0f;

        vertex_data[k++] = x_coord;
        vertex_data[k++] = 1;
        vertex_data[k++] = 0.0f;

        x_coord += x_width;
    }

    ShaderParameter param = ShaderParameter("position", (6 * (x + 1) + 6 * (y + 1) ) * 4, 3, vertex_data, true);

    buffers.addBuffer(param);
    //recursiveInit();
}

void CompositeGrid::draw()
{
	//Code used to draw now unneeded grid
    //grid_shader.bind();
    //WorldState::pushShader(&grid_shader);
    //buffers.bind();
    //WorldState::pushBuffer(&buffers);
    //glDrawArrays(GL_LINES, 0, (x + y + 2) * 2);
    //buffers.unbind();
    //WorldState::popBuffer(&buffers);
    //grid_shader.unbind();
    //WorldState::popShader(&grid_shader);

    recursiveDraw();
    if (clip_tiles)
        glScissor(left, bottom, right - left, top - bottom);
    if (center_view)
        glViewport(left, bottom, right - left, top - bottom);
}

void CompositeGrid::recursiveDraw()
{
    if (center_view)
        glViewport(left, bottom, right - left, top - bottom);
    if (clip_tiles)
        glScissor(left, bottom, right - left, top - bottom);

    if (!is_leaf)
        for (CompositeGrid* child: children)
            child->recursiveDraw();
    try
    {
        for (int i = 0; i < meshes.size(); i++)
        {
            //meshes[i].rotate();
        	meshes[i].draw();
        }
    }
    catch(ScenarioException e)
    {
        error_texture.setText(e.what());
        error_texture.draw();
    }

}

void CompositeGrid::acceptVisitor(Visitor* v)
{
    v->visitCompositeGrid(this);
}