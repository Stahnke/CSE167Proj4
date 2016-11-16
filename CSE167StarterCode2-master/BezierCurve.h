#ifndef _BEZIERCURVE_H
#define _BEZIERCURVE_H

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

class BezierCurve
{
public:
	BezierCurve(unsigned int N, glm::mat4x3 controlPts);
	~BezierCurve();

	glm::vec3 color;

	glm::mat4 toWorld;

	void draw(GLuint);
	void update(glm::mat4x3 newPoints);

	float maxHeight;
	float maxT;

	// These variables are needed for the shader program
	GLuint bezierVBO, bezierVAO, bezierEBO;
	GLuint uProjection, uModelview;

	//Bezier vars
	glm::mat4x3 controlPts;
	unsigned int N;

	//Bezier functions
	glm::vec4 calcBernstein(const float t);
	glm::vec3 calcBezierPoint(const float t);
	void calcBezierCurve();
	float calcChoose(unsigned int n, unsigned int k);
	glm::vec2 getMaxHeight();
};

#endif

