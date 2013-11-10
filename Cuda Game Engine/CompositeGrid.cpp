//#include "CompositeGrid.h"
//
//bool CompositeGrid::clip_tiles = false;
//bool CompositeGrid::center_view = false;
//
//CompositeGrid::CompositeGrid(int x_size, int y_size, int l, int r, int b, int t) :
//	left(l),
//	right(r),
//	bottom(b),
//	top(t)
//	//last_mouse_x(-1),
//	//last_mouse_y(-1)
//{
//}
//
//CompositeGrid::CompositeGrid()
//{
//}
//
//CompositeGrid::CompositeGrid(const CompositeGrid& copy) :
//	left(copy.left),
//	right(copy.right),
//	bottom(copy.bottom),
//	top(copy.top)
//	//camera(copy.camera)
//{
//	//meshes = copy.meshes;
//}
//
////void CompositeGrid::handleInput(unsigned char key_pressed, int x, int y)
////{
////	switch (key_pressed)
////	{
////	case 'w':
////		camera.setLocation(-camera.view_direction * 0.22 + camera.location);
////		break;
////	case 'a':
////		camera.setLocation((camera.view_direction.cross(camera.up_direction)) * 0.22 + camera.location);
////		break;
////	case 's':
////		camera.setLocation(camera.view_direction * 0.22 + camera.location);
////		break;
////	case 'd':
////		camera.setLocation(-(camera.view_direction.cross(camera.up_direction)) * 0.22 + camera.location);
////		break;
////	}
////	//std::cout << "x: " << camera.location.x << "y: " << camera.location.y << "z: " << camera.location.z << std::endl;
////}
////
////void CompositeGrid::mouseMoved(int x, int y)
////{
////	int dx = last_mouse_x - x;
////	int dy = last_mouse_y - y;
////	last_mouse_x = x;
////	last_mouse_y = y;
////
////	camera.mouseChanged(dx, dy);
////}
//
//CompositeGrid& CompositeGrid::operator=(const CompositeGrid& rhs)
//{
//	left = rhs.left;
//	right= rhs.right;
//	bottom = rhs.bottom;
//	top = rhs.top;
//	//meshes = rhs.meshes;
//	//camera = rhs.camera;
//
//	return *this;
//}
//
//
//CompositeGrid::~CompositeGrid(void)
//{
//	//TODO: Clean memory
//	//for (int i = 0; i < children.size(); i++)
//	//	delete children[i];
//}
//
//
//void CompositeGrid::draw()
//{
//	//if (camera.isDirty())
//	//	camera.bind();
//	//recursiveDraw();
//	if (clip_tiles)
//		glScissor(left, bottom, right - left, top - bottom);
//	if (center_view)
//		glViewport(left, bottom, right - left, top - bottom);
//	updateObjects();
//
//	glutWarpPointer((right - left) / 2, (top - bottom) / 2);
//
//	//last_mouse_x = (right - left) / 2;
//	//last_mouse_y = (top - bottom) / 2;
//}
//
//void CompositeGrid::updateObjects()
//{
//	//physics_engine.processGeometry(meshes);
//}
//
////void CompositeGrid::recursiveDraw()
////{
////	if (center_view)
////		glViewport(left, bottom, right - left, top - bottom);
////	if (clip_tiles)
////		glScissor(left, bottom, right - left, top - bottom);
////
////	try
////	{
////		//for (int i = 0; i < meshes.size(); i++)
////		//{
////		//	meshes[i].draw();
////		//}
////	}
////	catch(ScenarioException e)
////	{
////	}
////}
//
////void CompositeGrid::acceptVisitor(Visitor* v)
////{
//	//v->visitNode(&root_object);
//	//root_object.acceptVisitor(v);
//	//v->visitCompositeGrid(this);
////}