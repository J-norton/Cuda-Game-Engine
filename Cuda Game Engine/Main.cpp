#include <iostream>

//Include GL headers. Note: glew before glut
#include <GL/glew.h>
#include <GL/freeglut.h>

//Include CUDA headers
#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <helper_cuda.h>
#include <helper_cuda_gl.h>

#include "RenderTarget.h"

RenderTarget render_manager ;

void displayLoop()
{
	render_manager.render();
}

void resizeLoop(int width, int height)
{
	glViewport(0, 0, width, height);
}

void keyboardFunction(unsigned char key, int x, int y)
{
	render_manager.handleInput(key, x, y);
}

void mouseNotClickedMotion(int x, int y)
{
	render_manager.mouseMoved(x, y);
}


void initializeCuda();

int main(int argc, char** argv)
{
	Magick::InitializeMagick(*argv);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(1024, 1024);

	glutCreateWindow("James Engine");
	if (glewInit() != GLEW_OK)
		std::cout << "glewInit failed";

	//Initialize Cuda prior to setting callbacks
	initializeCuda();

	glutDisplayFunc(&displayLoop);
	glutKeyboardFunc(&keyboardFunction);
	glutPassiveMotionFunc(&mouseNotClickedMotion);
	glutReshapeFunc(&resizeLoop);

	render_manager = RenderTarget(1024, 1024, argc, argv);
	render_manager.dispatchVisitor();
	const GLubyte* version_str = glGetString(GL_VERSION);

	glutMainLoop();
}

void initializeCuda()
{
	cudaError_t err = cudaGLSetGLDevice(gpuGetMaxGflopsDeviceId());
}