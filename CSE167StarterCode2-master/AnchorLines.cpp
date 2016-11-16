#include "AnchorLines.h"
#include "Window.h"

vector <glm::vec3> linesVertices;
vector <unsigned int> linesIndices;

AnchorLines::AnchorLines(glm::vec3 anchorPoint1, glm::vec3 anchorPoint2)
{
	toWorld = glm::mat4(1.0f);

	ID = 0;

	color = { 0.5f,1.0f,0.0f };

	linesVertices.clear();
	linesVertices.push_back(anchorPoint1);
	linesVertices.push_back(anchorPoint2);

	linesIndices.clear();
	linesIndices.push_back(0);
	linesIndices.push_back(1);

	glGenVertexArrays(1, &linesVAO);
	glGenBuffers(1, &linesVBO);
	glGenBuffers(1, &linesEBO);

	glBindVertexArray(linesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);

	glBufferData(GL_ARRAY_BUFFER, linesVertices.size() * sizeof(glm::vec3), &linesVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, linesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, linesIndices.size() * sizeof(unsigned int), &linesIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

AnchorLines::~AnchorLines()
{
	glDeleteVertexArrays(1, &linesVAO);
	glDeleteBuffers(1, &linesVBO);
	glDeleteBuffers(1, &linesEBO);
}

void AnchorLines::draw(GLuint shaderProgram)
{
	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	GLuint uColorLoc = glGetUniformLocation(shaderProgram, "myColor");
	glUniform3f(uColorLoc, color.x, color.y, color.z);

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	glBindVertexArray(linesVAO);

	glDrawElements(GL_LINES, linesIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void AnchorLines::drawSelection(GLuint shaderProgram)
{
	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	GLuint uIDLoc = glGetUniformLocation(shaderProgram, "ID");
	glUniform1ui(uIDLoc, ID);

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	glBindVertexArray(linesVAO);

	glDrawElements(GL_POINTS, linesIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void AnchorLines::update(glm::vec3 point1, glm::vec3 point2)
{
	linesVertices.clear();
	linesVertices.push_back(point1);
	linesVertices.push_back(point2);

	glBindVertexArray(linesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);

	glBufferData(GL_ARRAY_BUFFER, linesVertices.size() * sizeof(glm::vec3), &linesVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, linesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, linesIndices.size() * sizeof(unsigned int), &linesIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}
