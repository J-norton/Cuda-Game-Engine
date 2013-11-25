#pragma once
#include <boost/program_options.hpp>
#include "Camera.h"
#include "Node.h"


class RenderTarget
{
private:
	int last_mouse_x;
	int last_mouse_y;
	Camera camera;
	//Indicates whether the mouse needs to be recentered
	bool need_reset_mouse;

	int width;
	int height;
	std::string execution_path;
	//Command line arguments from main()
	int argcount;
	char** strings;
	bool captured;
	void captureImage();
    int scenario_id;
	Node root_object;
public:
	void mouseMoved(int x, int y);
	void handleInput(unsigned char key_pressed, int x, int y);
	void dispatchVisitor();
	int getWidth();
	int getHeight();
	void render();
	RenderTarget(int width, int height, int argc, char** argv);
	RenderTarget();
	~RenderTarget(void);
};

