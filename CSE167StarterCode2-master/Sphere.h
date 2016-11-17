#ifndef _SPHERE_H
#define _SPHERE_H

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

class Sphere
{
public:
	Sphere();
	~Sphere();

	glm::mat4 toWorld;

	void draw(GLuint);
	void update();
	void setPosition(glm::vec3 position);
	void initSphere();
	void solidSphere(float radius, int stacks, int slices);

	// These variables are needed for the shader program
	GLuint sphereVBO, sphereVAO, sphereEBO;
	GLuint uProjection, uModel, uView;
};

#endif

