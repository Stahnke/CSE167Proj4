#ifndef _ANCHORPOINTS_H
#define _ANCHORPOINTS_H

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

class AnchorPoints
{
public:
	AnchorPoints(vector<glm::vec3> anchorPoints);
	~AnchorPoints();

	void draw(GLuint);
	void update();

	glm::vec3 color;

	glm::mat4 toWorld;

	// These variables are needed for the shader program
	GLuint anchorVBO, anchorVAO, anchorEBO;
	GLuint uProjection, uModelview;

};

#endif