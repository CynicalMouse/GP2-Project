#include "Mesh.h"

// CONSTRUCTOR & DESTRUCTOR //
#pragma region Constructor & Destructor
Mesh::Mesh()
{
	drawCount = NULL;
	vertexArrayObject = NULL;
	vertexArrayBuffers[0] = NULL;
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vertexArrayObject);
}
#pragma endregion

// MESH CREATION //
#pragma region Mesh Creation
void Mesh::draw()
{
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::loadMeshFile(const std::string& fileName)
{
	IndexedModel model = OBJModel(fileName).ToIndexedModel();
	initialiseMesh(model);
}

void Mesh::initialiseMesh(const IndexedModel& model)
{
	drawCount = model.indices.size();

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);

	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.positions[0]), &model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, model.positions.size() * sizeof(model.texCoords[0]), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.indices.size() * sizeof(model.indices[0]), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

}

#pragma endregion

// VERTEX MESH CREATION //
#pragma region Vertex Mesh Creation
void Mesh::drawVertexes()
{
	glBindVertexArray(vertexArrayObject);
	glDrawArrays(GL_TRIANGLES, 0, drawCount);
	glBindVertexArray(0);
}

void Mesh::loadVertexs(Vertex* vertices, unsigned int numVertices)
{
	drawCount = numVertices;

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> textCoords;

	// Reserve space for data
	positions.reserve(numVertices);
	textCoords.reserve(numVertices);

	for (int i = 0; i < numVertices; i++)
	{
		// Store positions
		positions.push_back(*vertices[i].getPos());
		textCoords.push_back(*vertices[i].getTexturePos());
	}

	glGenBuffers(NUM_BUFFERS, vertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW); 

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(textCoords[0]), &textCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void Mesh::loadMeshVert(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedModel model;

	for (unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].getPos());
		model.texCoords.push_back(*vertices[i].getTexturePos());
		model.normals.push_back(*vertices[i].getNormal());
	}

	for (unsigned int i = 0; i < numIndices; i++)
	{
		model.indices.push_back(indices[i]);
	}
		
	initialiseMesh(model);
}

#pragma endregion