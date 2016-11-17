#include "Sphere.h"
#include "Window.h"

vector<glm::vec3> sphereVertices;
vector<glm::vec3> sphereNormals;
vector<unsigned int> sphereIndices;

int prevSlices;
int prevStacks;
float pi = 3.14159265358979323846;

Sphere::Sphere()
{
	toWorld = glm::mat4(1.0f);
	initSphere();
}

Sphere::~Sphere()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteBuffers(1, &sphereVBO);
	glDeleteBuffers(1, &sphereEBO);
}

void Sphere::draw(GLuint shaderProgram)
{
	solidSphere(1, 20, 20);

	// Draw the sphere regardless of whether it was previously updated or not

	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);
	glBindVertexArray(sphereVAO);
	glDrawArrays(GL_TRIANGLES, 0, sphereVertices.size());
	glBindVertexArray(0);
}

// This function initializes the properties of a sphere. Only sets up the buffer attributes,
// actual vertices and normals are assigned later.
void Sphere::initSphere() {

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereEBO);

	glBindVertexArray(sphereVAO);

	// Bind vertices to layout location 0
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW); // Don't give it any data
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind normals to layout location 1
	glBindBuffer(GL_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_STATIC_DRAW); // Don't give it any data
	glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// This function draws a solid sphere
void Sphere::solidSphere(float radius, int stacks, int slices) {
	// No need to generate the sphere again if the previous rendering already
	// used the same number of stacks and slices
	if (prevStacks != stacks || prevSlices != slices) {
		prevStacks = stacks; prevSlices = slices;
		sphereVertices.clear();
		sphereNormals.clear();
		float fstacks = (float)stacks;
		float fslices = (float)slices;
		for (int i = 0; i < slices; i++) {
			for (int j = 0; j < stacks; j++) {
				// Top left
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
					radius * -cos(pi * (j + 1.0f) / fslices),
					radius * sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
					-cos(pi * (j + 1.0f) / fslices),
					sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices))));
				// Top right
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices),
					radius * -cos(pi * (j + 1.0) / fslices),
					radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices),
					-cos(pi * (j + 1.0) / fslices),
					sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * (j + 1.0) / fslices))));
				// Bottom right
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
					radius * -cos(pi * j / fslices),
					radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
					-cos(pi * j / fslices),
					sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices))));

				// Need to repeat 2 of the vertices since we can only draw triangles. Eliminates the confusion
				// of array indices.
				// Top left
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
					radius * -cos(pi * (j + 1.0f) / fslices),
					radius * sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices),
					-cos(pi * (j + 1.0f) / fslices),
					sin(2.0f * pi * i / fstacks) * sin(pi * (j + 1.0f) / fslices))));
				// Bottom right
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
					radius * -cos(pi * j / fslices),
					radius * sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices),
					-cos(pi * j / fslices),
					sin(2.0f * pi * (i + 1.0) / fstacks) * sin(pi * j / fslices))));
				// Bottom left
				sphereVertices.push_back(glm::vec3(
					radius * -cos(2.0f * pi * i / fstacks) * sin(pi * j / fslices),
					radius * -cos(pi * j / fslices),
					radius * sin(2.0f * pi * i / fstacks) * sin(pi * j / fslices)));
				sphereNormals.push_back(glm::normalize(glm::vec3(
					-cos(2.0f * pi * i / fstacks) * sin(pi * j / fslices),
					-cos(pi * j / fslices),
					sin(2.0f * pi * i / fstacks) * sin(pi * j / fslices))));

			}
		}
		// Now bind this new vertex data
		glBindVertexArray(sphereVAO);

		// Bind vertices
		glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sphereVertices.size(), &sphereVertices[0], GL_STATIC_DRAW);

		// Bind normals
		glBindBuffer(GL_ARRAY_BUFFER, sphereEBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * sphereNormals.size(), &sphereNormals[0], GL_STATIC_DRAW);

		// Done updating the buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void Sphere::update()
{
}

void Sphere::setPosition(glm::vec3 position)
{
	toWorld = glm::translate(glm::mat4(1.0f), position);
}

