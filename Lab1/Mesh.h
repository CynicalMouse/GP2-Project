#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include "obj_loader.h"
#include "Vertex.h"

class Mesh
{

public:
	// CONSTRUCTOR & DESTRUCTOR //
	Mesh();
	~Mesh();

	// MESH CREATION //
	void draw();
	void loadMeshFile(const std::string& fileName);
	void initialiseMesh(const IndexedModel& model);

	// VERTEX MESH CREATION //
	void drawVertexes();
	void loadVertexs(Vertex* vertices, unsigned int numVertices);
	void loadMeshVert(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);

private:
	// ATTRIBUTES //
	enum
	{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS];
	unsigned int drawCount;
};
