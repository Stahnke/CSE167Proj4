#include "AnchorPoint.h"
#include "Window.h"

vector <glm::vec3> anchorVertices;
vector <unsigned int> anchorIndices;

AnchorPoint::AnchorPoint(glm::vec3 anchorPoint, unsigned int ID, unsigned int COLOR) : ID(ID)
{
	toWorld = glm::mat4(1.0f);
	const unsigned int RED = 0;
	const unsigned int YELLOW = 1;

	if (COLOR == RED)
		color = { 1.0f,0.0f,0.0f };
	else if (COLOR == YELLOW)
		color = { 0.5f,1.0f,0.0f };

	anchorVertices.clear();
	anchorVertices.push_back(anchorPoint);

	anchorIndices.clear();
	anchorIndices.push_back(0);

	glGenVertexArrays(1, &anchorVAO);
	glGenBuffers(1, &anchorVBO);
	glGenBuffers(1, &anchorEBO);

	glBindVertexArray(anchorVAO);

	glBindBuffer(GL_ARRAY_BUFFER, anchorVBO);

	glBufferData(GL_ARRAY_BUFFER, anchorVertices.size() * sizeof(glm::vec3), &anchorVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, anchorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, anchorIndices.size() * sizeof(unsigned int), &anchorIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

AnchorPoint::~AnchorPoint()
{
	glDeleteVertexArrays(1, &anchorVAO);
	glDeleteBuffers(1, &anchorVBO);
	glDeleteBuffers(1, &anchorEBO);
}

void AnchorPoint::draw(GLuint shaderProgram)
{

	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	GLuint uColorLoc = glGetUniformLocation(shaderProgram, "myColor");
	glUniform3f(uColorLoc, color.x, color.y, color.z);

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	glBindVertexArray(anchorVAO);


	glDrawElements(GL_POINTS, anchorIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void AnchorPoint::drawSelection(GLuint shaderProgram)
{
	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	GLuint uIDLoc = glGetUniformLocation(shaderProgram, "ID");
	glUniform1ui(uIDLoc, ID);

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	glBindVertexArray(anchorVAO);

	glDrawElements(GL_POINTS, anchorIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void AnchorPoint::update(glm::vec3 point)
{
	anchorVertices.clear();
	anchorVertices.push_back(point);

	glBindVertexArray(anchorVAO);

	glBindBuffer(GL_ARRAY_BUFFER, anchorVBO);

	glBufferData(GL_ARRAY_BUFFER, anchorVertices.size() * sizeof(glm::vec3), &anchorVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, anchorEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, anchorIndices.size() * sizeof(unsigned int), &anchorIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
