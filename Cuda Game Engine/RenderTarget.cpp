#include "RenderTarget.h"


RenderTarget::RenderTarget(int w, int h, int argc, char** argv) :
    width(w),
    height(h),
    argcount(argc),
    strings(argv),
    captured(true)
{
    scenario_id = -1;
    int tile_size = 4;
    bool clip  = false;
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("capture", "capture golden image")
        ("scissor", boost::program_options::value<int>(), "scissor enabled")
        ("center", boost::program_options::value<int>(), "center view")
        ("scenario", boost::program_options::value<int>( &scenario_id)->default_value(-1), "scenario ID")
        ("size", boost::program_options::value<int>(&tile_size)->default_value(4), "set tile size")
        ;

    boost::program_options::variables_map vm;
    try {
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    }
    catch (boost::program_options::error& e)
    {
        std::cout << "Invalid command line options" << std::endl;
        std::cout << "Undefined behavior may result" << std::endl;
    }

    boost::program_options::notify(vm);    

    if (vm.count("capture")) 
        captured = false;
    if (vm.count("help")) {
        std::cout << desc << "\n";
    }
    if (vm.count("scissor") == 1) {
        std::cout << "Scissor test enabled\n";
        CompositeGrid::clip_tiles = true;
        glEnable(GL_SCISSOR_TEST);
    }
    if (vm.count("center") == 1) {
        std::cout << "All tiles set to centered" << std::endl;
        CompositeGrid::center_view = true;
    }
    if (scenario_id) {
        std::cout << "Stress testing scenario number: " << scenario_id << std::endl;
    }

    if (vm.count("size")) {
        std::cout << "Tile size was set to " 
            << vm["size"].as<int>() << ".\n";
        tile_size = vm["size"].as<int>();
    } else {
        std::cout << "Tile size was not set.\n";
        std::cout << "Defaulting to 4x4 tile grid.\n";
    }

    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glClearColor(0.16, 0.6, 0.6, 1.0);
    //Create a 4x4 tile grid
    tiles = CompositeGrid( tile_size, tile_size, 0, w, 0, h);

    tiles.init();
}

RenderTarget::RenderTarget()
{
}


RenderTarget::~RenderTarget(void)
{
}

void RenderTarget::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    tiles.draw();
    //if (!captured)
    //{
    //    captureImage();
    //    captured = true;
    //}
    glutSwapBuffers();
    glutPostRedisplay();
}

void RenderTarget::dispatchVisitor()
{
    CenteredXmlVisitor v_center = CenteredXmlVisitor("..\\Test Files\\XML Resources\\Content Resources Test.xml", argcount, strings);

    std::string beginning = "..\\Test Files\\XML Scenarios\\Scene Input";
    if (scenario_id < 0)
        std::cout << "Scenario case is less than 0, check command line arguments";
    std::string num_str = std::to_string(scenario_id); 
    GlobalXmlVisitor v_single = GlobalXmlVisitor("..\\Test Files\\XML Resources\\Content Resources Test.xml",
        beginning.append(num_str).append(".xml"), argcount, strings);


    if (CompositeGrid::center_view)
        tiles.acceptVisitor(&v_center);
    else
        tiles.acceptVisitor(&v_single);
}

//Captures the output of one frame and writes it to an image file
void RenderTarget::captureImage()
{
    float *pixels = new float[width * height * 4];
    glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, pixels);
    try
    {
        Magick::Blob blob( pixels, width * height * 4 ); 
        Magick::Image image("1024x1024", "white");
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                int offset = 4 * i * width + j * 4;
                Magick::ColorRGB c = Magick::ColorRGB(pixels[offset], pixels[offset + 1], pixels[offset + 2]);
                image.pixelColor(j, i, c);
            }
        }
        image.flip();
        image.write("../Test Files/Golden Images/Golden Image.jpeg");
    }
    catch (Magick::Error& e)
    {
        std::cout << "Error occurred while writing golden image." << std::endl;
        std::cout << e.what();
    }
    delete[] pixels;
}

int RenderTarget::getWidth()
{
    return width;
}

int RenderTarget::getHeight()
{
    return height;
}