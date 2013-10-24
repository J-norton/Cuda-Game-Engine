#pragma once
#include "Shader.h"
#include "BufferManager.h"
#include "WorldState.h"
#include <vector>
#include "TriangleMesh.h"
#include "ObjFileReader.h"

#define COMPOSITE_GRID_H
#include "Visitor Header.h"

#include "ScenarioException.h"
#include "FrameBufferTexture.h"

class CompositeGrid
{
private:
	bool is_leaf;
	bool is_root;
	int x;
	int y;

	int left;
	int bottom;
	int right;
	int top;

    static Texture frame_image;

	//Recursively allocates the memory for the grid
	void recursiveBuild();

	BufferManager buffers;
    //FrameBufferTexture frame;

	//CompositeGrid &parent;
	std::vector<CompositeGrid*> children;
	std::vector<TriangleMesh> meshes;
	//Draws all of the geometry in each individual tile
	void recursiveDraw();

	//Texture that renders error messages to the screen in case of an
	//uncaught exception
	TriangleMesh error_texture;

    float counter;

	static Shader grid_shader;
public:
	friend class Visitor;
	friend class XmlVisitor;
	friend class CenteredXmlVisitor;
	friend class GlobalXmlVisitor;
	
	//Switches which are set by command line arguments
	static bool clip_tiles;
	static bool center_view;

	void init();
	void draw();
	void acceptVisitor(Visitor* v);
	CompositeGrid(int x, int y, int l, int r, int b, int t); 
	CompositeGrid(const CompositeGrid& copy);
	CompositeGrid& operator=(const CompositeGrid& rhs);
	CompositeGrid(void);
	~CompositeGrid(void);
};
