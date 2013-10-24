#pragma once
#include <boost/program_options.hpp>
#include "CompositeGrid.h"


class RenderTarget
{
private:
	CompositeGrid tiles;
	int width;
	int height;
	std::string execution_path;
	//Command line arguments from main()
	int argcount;
	char** strings;
	bool captured;
	void captureImage();
    int scenario_id;
public:
	void dispatchVisitor();
	int getWidth();
	int getHeight();
	void render();
	RenderTarget(int width, int height, int argc, char** argv);
	RenderTarget();
	~RenderTarget(void);
};

