#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Sphere.h"
#include "OBJObject.h"
#include "Skybox.h"
#include "shader.h"
#include "BezierCurve.h"
#include "AnchorPoint.h"
#include "AnchorLines.h"

//from proj1
#include <vector>
#include <stdio.h>
#include <iostream>

//from proj2
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

using namespace std;

class Window
{

public:

	static glm::mat4 camTransform;
	static glm::mat4 camRotate;
	static glm::mat4 camRotatePos;

	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void selectionDraw();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static glm::vec3 trackBallMapping(glm::vec2 point);

	static void translateCamera(glm::vec3 transVec);
	static void rotateCamera(float rot_angle, glm::vec3 transVec);
	static void resetCamera();

	static void translateSelection(glm::vec3 transVec);
	static void remakePoints();
	static glm::mat4x3 findPointsInCurve(unsigned int curveNum);

	static void printMatrix(glm::mat4x3);

};

#endif
