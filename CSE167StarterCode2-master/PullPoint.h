#ifndef _PULLPOINT_H
#define _PULLPOINT_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

using namespace std;

class PullPoint
{
public:
	PullPoint(glm::vec3 pullPoint, unsigned int ID);
	~PullPoint();

	void draw(GLuint);
	void drawSelection(GLuint);
	void update();

	unsigned int ID;

	glm::vec3 color;

	glm::mat4 toWorld;

	// These variables are needed for the shader program
	GLuint pullVBO, pullVAO, pullEBO;
	GLuint uProjection, uModelview;

};

#endif