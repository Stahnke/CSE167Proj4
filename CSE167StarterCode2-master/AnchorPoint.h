#ifndef _ANCHORPOINT_H
#define _ANCHORPOINT_H

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

class AnchorPoint
{
public:
	AnchorPoint(glm::vec3 anchorPoints, unsigned int ID, unsigned int COLOR);
	~AnchorPoint();

	unsigned int ID;

	void draw(GLuint);
	void drawSelection(GLuint);
	void update(glm::vec3 point);

	glm::vec3 color;

	glm::mat4 toWorld;

	// These variables are needed for the shader program
	GLuint anchorVBO, anchorVAO, anchorEBO;
	GLuint uProjection, uModel, uView;
};

#endif