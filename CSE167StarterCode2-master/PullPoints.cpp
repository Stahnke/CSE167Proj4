#include "PullPoints.h"
#include "Window.h"

vector <glm::vec3> pullVertices;
vector <unsigned int> pullIndices;

PullPoints::PullPoints(vector<glm::vec3> pullPoints)
{
	toWorld = glm::mat4(1.0f);

	color = { 0.5f,1.0f,0.0f};

	pullVertices = pullPoints;

	for (int x = 1; x < pullVertices.size(); x++)
	{
		pullIndices.push_back(x);
	}
	pullIndices.push_back(0);

	glGenVertexArrays(1, &pullVAO);
	glGenBuffers(1, &pullVBO);
	glGenBuffers(1, &pullEBO);

	glBindVertexArray(pullVAO);

	glBindBuffer(GL_ARRAY_BUFFER, pullVBO);

	glBufferData(GL_ARRAY_BUFFER, pullVertices.size() * sizeof(glm::vec3), &pullVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pullEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, pullIndices.size() * sizeof(unsigned int), &pullIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

PullPoints::~PullPoints()
{
	glDeleteVertexArrays(1, &pullVAO);
	glDeleteBuffers(1, &pullVBO);
	glDeleteBuffers(1, &pullEBO);
}

void PullPoints::draw(GLuint shaderProgram)
{
	

	glm::mat4 modelview = Window::V * toWorld;

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");

	GLuint uColorLoc = glGetUniformLocation(shaderProgram, "myColor");
	glUniform3f(uColorLoc, color.x, color.y, color.z);

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	glBindVertexArray(pullVAO);

	glPointSize(10.0f);
	glDrawElements(GL_POINTS, pullIndices.size(), GL_UNSIGNED_INT, 0);
	glPointSize(1.0f);
	glDrawElements(GL_LINES, pullIndices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void PullPoints::update()
{
}
