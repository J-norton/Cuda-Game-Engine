#pragma once
//#include <vector>
//#include "TriangleMesh.h"

//#define COMPOSITE_GRID_H
//#include "Visitor Header.h"

//#include "ScenarioException.h"
//#include "GeometryManager.cuh"
//#include "Camera.h"
//#include "Node.h"

//class CompositeGrid
//{
//private:
//	int left;
//	int bottom;
//	int right;
//	int top;
//	//Camera camera;
//	//The root container for all the geometry in the scene
//	//Node root_object;
//
//	//int last_mouse_x;
//	//int last_mouse_y;
//
//	GeometryManager physics_engine;
//
//	//std::vector<TriangleMesh> meshes;
//	//Draws all of the geometry in each individual tile
//	void recursiveDraw();
//
//    //float counter;
//
//public:
//	friend class Visitor;
//	friend class GlobalXmlVisitor;
//	
//	//Switches which are set by command line arguments
//	static bool clip_tiles;
//	static bool center_view;
//
//	void mouseMoved(int x, int y);
//	void handleInput(unsigned char key_pressed, int x, int y);
//	void updateObjects();
//	void draw();
//	//void acceptVisitor(Visitor* v);
//	CompositeGrid(int x, int y, int l, int r, int b, int t); 
//	CompositeGrid(const CompositeGrid& copy);
//	CompositeGrid& operator=(const CompositeGrid& rhs);
//	CompositeGrid(void);
//	~CompositeGrid(void);
//};
