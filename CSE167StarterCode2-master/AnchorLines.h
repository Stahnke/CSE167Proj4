#ifndef _ANCHORLINE_H
#define _ANCHORLINE_H

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

class AnchorLines
{
public:
	AnchorLines(glm::vec3 anchorPoint1, glm::vec3 anchorPoint2);
	~AnchorLines();

	unsigned int ID;

	void draw(GLuint);
	void drawSelection(GLuint);
	void update(glm::vec3 point1, glm::vec3 point2);

	glm::vec3 color;

	glm::mat4 toWorld;

	// These variables are needed for the shader program
	GLuint linesVBO, linesVAO, linesEBO;
	GLuint uProjection, uModelview;
};

#endif