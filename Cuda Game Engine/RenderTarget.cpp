#include "RenderTarget.h"


RenderTarget::RenderTarget(int w, int h, int argc, char** argv) :
	width(w),
	height(h),
	argcount(argc),
	strings(argv),
	captured(true),
	last_mouse_x(-1),
	last_mouse_y(-1),
	need_reset_mouse(true)
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
	glClearColor(0.16, 0.3, 0.3, 1.0);
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

	if (camera.isDirty())
		camera.bind();
	root_object.draw();
	//tiles.draw();
	//if (!captured)
	//{
	//    captureImage();
	//    captured = true;
	//}


	//Resets the mouse to the center of the screen
	if (need_reset_mouse)
	{
		glutWarpPointer(width / 2, height / 2);
		last_mouse_x = width / 2;
		last_mouse_y = height / 2;
		need_reset_mouse = false;
	}

	glutSwapBuffers();
	glutPostRedisplay();
}

void RenderTarget::dispatchVisitor()
{
	std::string beginning = "..\\Test Files\\XML Scenarios\\Scene Input";
	if (scenario_id < 0)
		std::cout << "Scenario case is less than 0, check command line arguments";
	std::string num_str = std::to_string(scenario_id); 
	GlobalXmlVisitor v_single = GlobalXmlVisitor("..\\Test Files\\XML Resources\\Content Resources Test.xml",
		beginning.append(num_str).append(".xml"), argcount, strings);

	//tiles.acceptVisitor(&v_single);
	root_object.acceptVisitor(&v_single);
}

void RenderTarget::handleInput(unsigned char key_pressed, int x, int y)
{
	//tiles.handleInput(key_pressed, x, y);
	switch (key_pressed)
	{
	case 'w':
		camera.setLocation(-camera.view_direction * 0.22 + camera.location);
		break;
	case 'a':
		camera.setLocation((camera.view_direction.cross(camera.up_direction)) * 0.22 + camera.location);
		break;
	case 's':
		camera.setLocation(camera.view_direction * 0.22 + camera.location);
		break;
	case 'd':
		camera.setLocation(-(camera.view_direction.cross(camera.up_direction)) * 0.22 + camera.location);
		break;
	}
}

void RenderTarget::mouseMoved(int x, int y)
{
	need_reset_mouse = true;
	if (last_mouse_x == -1)
		last_mouse_x = x;
	if (last_mouse_y == -1)
		last_mouse_y = y;

	int dx = last_mouse_x - x;
	int dy = last_mouse_y - y;
	last_mouse_x = x;
	last_mouse_y = y;

	camera.mouseChanged(dx, dy);
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