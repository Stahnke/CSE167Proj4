#include "window.h"

const char* window_title = "GLFW Starter Project";

Skybox * skybox;
BezierCurve * bezierCurve0;
BezierCurve * bezierCurve1;
BezierCurve * bezierCurve2;
BezierCurve * bezierCurve3;
BezierCurve * bezierCurve4;
BezierCurve * bezierCurve5;
BezierCurve * bezierCurve6;
BezierCurve * bezierCurve7;
BezierCurve * bezierCurve8;
BezierCurve * bezierCurve9;
AnchorPoints * anchorPointsObj;
PullPoints * pullPointsObj;

//Vars for bezier curves
const unsigned int N = 150;
glm::vec3 p00, p01, p02, p03;
glm::vec3 p10, p11, p12, p13;
glm::vec3 p20, p21, p22, p23;
glm::vec3 p30, p31, p32, p33;
glm::vec3 p40, p41, p42, p43;
glm::vec3 p50, p51, p52, p53;
glm::vec3 p60, p61, p62, p63;
glm::vec3 p70, p71, p72, p73;
glm::vec3 p80, p81, p82, p83;
glm::vec3 p90, p91, p92, p93;
glm::mat4x3 controlPts0, controlPts1, controlPts2, controlPts3, controlPts4, controlPts5, controlPts6, controlPts7, controlPts8, controlPts9;
vector <glm::vec3> anchorPoints;
vector <glm::vec3> pullPoints;

GLint shaderProgram;
GLint skyboxShaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

#define SKYBOX_VERTEX_SHADER_PATH "../skyboxShader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skyboxShader.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, -1.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

glm::vec3 NEARPPos = cam_pos;
glm::vec3 FARPPos = { cam_pos.x - 0.0f, cam_pos.y - 0.0f, cam_pos.z - 1000.0f };
glm::vec3 TOPPos = { 0.0f, 20.0f, 0.0f };
glm::vec3 BOTTOMPos = { 0.0f, -20.0f, 0.0f };
glm::vec3 LEFTPos = { -20.0f, 0.0f, 0.0f };
glm::vec3 RIGHTPos = { 20.0f, 0.0f, 0.0f };

glm::vec3 NEARPNorm = { 0.0f, 0.0f, 1.0f };
glm::vec3 FARPNorm = { 0.0f,0.0f,-1.0f };
glm::vec3 TOPNorm = { 0.0f, 1.0f, 0.0f };
glm::vec3 BOTTOMNorm = { 0.0f, -1.0f, 0.0f };
glm::vec3 LEFTNorm = { -1.0f, 0.0f, 0.0f };
glm::vec3 RIGHTNorm = { 1.0f, 0.0f, 0.0f };


int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;
glm::mat4 Window::camTransform;
glm::mat4 Window::camRotate;
glm::mat4 Window::camRotatePos;

int Movement;
const int NONE = 0;
const int ROTATE = 1;
const int TRANSLATE = 2;
const float m_ROTSCALE = 1.0f;
const float m_ZOOMSCALE = 1.0f;
const float m_TRANSSCALE = 0.1f;

int Mode = 1;
const int CAMERA = 1;

glm::vec2 mouse_point;
glm::vec3 lastPoint;

void Window::initialize_objects()
{

	//Bezier Curves

	//Bezier Curve 0
	p00 = { 0.0f,0.0f,5.0f };
	p01 = { 0.0f,5.0f,5.0f };
	p02 = { 5.0f,5.0f,5.0f };
	p03 = { 5.0f,0.0f,5.0f };
	controlPts0 = { p00,p01,p02,p03 };
	bezierCurve0 = new BezierCurve(N, controlPts0);

	//Bezier Curve 1
	p10 = { 5.0f,0.0f,5.0f };
	p11 = { 5.0f,-5.0f,5.0f };
	p12 = { 10.0f,-5.0f,0.0f };
	p13 = { 10.0f,0.0f,0.0f };
	controlPts1 = { p10,p11,p12,p13 };
	bezierCurve1 = new BezierCurve(N, controlPts1);

	//Bezier Curve 2
	p20 = { 10.0f,0.0f,0.0f };
	p21 = { 10.0f,5.0f,0.0f };
	p22 = { 15.0f,5.0f,-5.0f };
	p23 = { 15.0f,0.0f,-5.0f };
	controlPts2 = { p20,p21,p22,p23 };
	bezierCurve2 = new BezierCurve(N, controlPts2);

	//Bezier Curve 3
	p30 = { 15.0f,0.0f,-5.0f };
	p31 = { 15.0f,-5.0f,-5.0f };
	p32 = { 10.0f,-5.0f,-10.0f };
	p33 = { 10.0f,0.0f,-10.0f };
	controlPts3 = { p30,p31,p32,p33 };
	bezierCurve3 = new BezierCurve(N, controlPts3);

	//Bezier Curve 4
	p40 = { 10.0f,0.0f,-10.0f };
	p41 = { 10.0f,5.0f,-10.0f };
	p42 = { 5.0f,5.0f,-15.0f };
	p43 = { 5.0f,0.0f,-15.0f };
	controlPts4 = { p40,p41,p42,p43 };
	bezierCurve4 = new BezierCurve(N, controlPts4);

	//Bezier Curve 5
	p50 = { 5.0f,0.0f,-15.0f };
	p51 = { 5.0f,-5.0f,-15.0f };
	p52 = { 0.0f,-5.0f,-10.0f };
	p53 = { 0.0f,0.0f,-10.0f };
	controlPts5 = { p50,p51,p52,p53 };
	bezierCurve5 = new BezierCurve(N, controlPts5);

	//Bezier Curve 6
	p60 = { 0.0f,0.0f,-10.0f };
	p61 = { 0.0f,5.0f,-10.0f };
	p62 = { -5.0f,5.0f,-5.0f };
	p63 = { -5.0f,0.0f,-5.0f };
	controlPts6 = { p60,p61,p62,p63 };
	bezierCurve6 = new BezierCurve(N, controlPts6);

	//Bezier Curve 7
	p70 = { -5.0f,0.0f,-5.0f };
	p71 = { -5.0f,-5.0f,-5.0f };
	p72 = { -10.0f,-5.0f,0.0f };
	p73 = { -10.0f,0.0f,0.0f };
	controlPts7 = { p70,p71,p72,p73 };
	bezierCurve7 = new BezierCurve(N, controlPts7);

	//Bezier Curve 8
	p80 = { -10.0f,0.0f,0.0f };
	p81 = { -10.0f,5.0f,0.0f };
	p82 = { -5.0f,5.0f,5.0f };
	p83 = { -5.0f,0.0f,5.0f };
	controlPts8 = { p80,p81,p82,p83 };
	bezierCurve8 = new BezierCurve(N, controlPts8);

	//Bezier Curve 9
	p90 = { -5.0f,0.0f,5.0f };
	p91 = { -5.0f,-5.0f,5.0f };
	p92 = { 0.0f,-5.0f,5.0f };
	p93 = { 0.0f,0.0f,5.0f };
	controlPts9 = { p90,p91,p92,p93 };
	bezierCurve9 = new BezierCurve(N, controlPts9);

	//Anchor Points
	anchorPoints.push_back(p00);
	anchorPoints.push_back(p03);
	anchorPoints.push_back(p10);
	anchorPoints.push_back(p13);
	anchorPoints.push_back(p20);
	anchorPoints.push_back(p23);
	anchorPoints.push_back(p30);
	anchorPoints.push_back(p33);
	anchorPoints.push_back(p40);
	anchorPoints.push_back(p43);
	anchorPoints.push_back(p50);
	anchorPoints.push_back(p53);
	anchorPoints.push_back(p60);
	anchorPoints.push_back(p63);
	anchorPoints.push_back(p70);
	anchorPoints.push_back(p73);
	anchorPoints.push_back(p80);
	anchorPoints.push_back(p83);
	anchorPoints.push_back(p90);
	anchorPoints.push_back(p93);
	anchorPointsObj = new AnchorPoints(anchorPoints);

	//Pull Points
	pullPoints.push_back(p01);
	pullPoints.push_back(p02);
	pullPoints.push_back(p11);
	pullPoints.push_back(p12);
	pullPoints.push_back(p21);
	pullPoints.push_back(p22);
	pullPoints.push_back(p31);
	pullPoints.push_back(p32);
	pullPoints.push_back(p41);
	pullPoints.push_back(p42);
	pullPoints.push_back(p51);
	pullPoints.push_back(p52);
	pullPoints.push_back(p61);
	pullPoints.push_back(p62);
	pullPoints.push_back(p71);
	pullPoints.push_back(p72);
	pullPoints.push_back(p81);
	pullPoints.push_back(p82);
	pullPoints.push_back(p91);
	pullPoints.push_back(p92);
	pullPointsObj = new PullPoints(pullPoints);

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);

	glm::mat4 toWorld = glm::mat4(1.0f);

	//Set the skybox
	skybox = new Skybox();
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	glDeleteProgram(shaderProgram);
	glDeleteProgram(skyboxShaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	//bezierCurve0 ->update();
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Use the shader of programID
	glUseProgram(skyboxShaderProgram);

	// Render the skybox
	skybox->draw(skyboxShaderProgram);

	// Use the shader of programID
	glUseProgram(shaderProgram);

	glPointSize(100.0f);
	// Render Bezier Curve
	bezierCurve0->draw(shaderProgram);
	bezierCurve1->draw(shaderProgram);
	bezierCurve2->draw(shaderProgram);
	bezierCurve3->draw(shaderProgram);
	bezierCurve4->draw(shaderProgram);
	bezierCurve5->draw(shaderProgram);
	bezierCurve6->draw(shaderProgram);
	bezierCurve7->draw(shaderProgram);
	bezierCurve8->draw(shaderProgram);
	bezierCurve9->draw(shaderProgram);

	// Render Anchor Points
	anchorPointsObj->draw(shaderProgram);
	// Render Pull Points
	pullPointsObj->draw(shaderProgram);


	
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		//Reset camera
		else if (key == GLFW_KEY_R)
		{
			switch (Mode)
			{
			case CAMERA:
				resetCamera();
				break;
			}
		}

		//Camera movement mode
		else if (key == GLFW_KEY_1)
		{
			Mode = CAMERA;
		}
	}

	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{

		//Camera movement controls (FPS style)
		if (key == GLFW_KEY_W)	//Forward
		{
			translateCamera({ 0.0f,0.0f,-1.0f });
			//cout << "Z" << mods << endl;
		}
		else if (key == GLFW_KEY_S) //Back
		{
			switch (Mode)
			{
			case CAMERA:
				translateCamera({ 0.0f,0.0f,1.0f });
				break;
			}
		}
			//cout << "z" << endl;
		else if (key == GLFW_KEY_A) //Left
		{
			translateCamera({ -1.0f,0.0f,0.0f });
			//cout << "X" << mods << endl;
		}
		else if (key == GLFW_KEY_D) //Right
		{
			translateCamera({ 1.0f,0.0f,0.0f });
			//cout << "x" << endl;
		}
		else if (key == GLFW_KEY_SPACE) //Up
		{
			translateCamera({ 0.0f,1.0f,0.0f });
			//cout << "Y" << mods << endl;
		}
		else if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) //Down
		{
			translateCamera({ 0.0f,-1.0f,0.0f });
			//cout << "y" << endl;
		}
	}
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	//Set the last know mouse_point
	mouse_point = { xpos, ypos };

	glm::vec3 direction;
	glm::vec3 curPoint;
	float rot_angle;

	// Handle any necessary mouse movements
	switch (Movement)
	{

		// Left-mouse button is being held down
	case ROTATE:
	{
		curPoint = trackBallMapping(mouse_point);
		direction = curPoint - lastPoint;
		float velocity = glm::length(direction);

		if (velocity > 0.0001) // If little movement - do nothing.
		{
			
			// Rotate about the axis that is perpendicular to the great circle connecting the mouse movements.
			glm::vec3 rotAxis;
			rotAxis = glm::cross(lastPoint, curPoint);
			rot_angle = velocity * m_ROTSCALE;

			//Apply the rotation
			switch (Mode)
			{
			case CAMERA:
				//rotateCamera(rot_angle, rotAxis);
				rotateCamera( direction.x * m_ROTSCALE, {0.0f, 1.0f, 0.0f});
				rotateCamera( -direction.y * m_ROTSCALE, {1.0f, 0.0f, 0.0f});
				break;
			}

		}
		break;
	}

	//Right-mouse button is being held down
	case TRANSLATE:
	{
		curPoint = trackBallMapping(mouse_point);

		//Get the translation vector
		direction = curPoint - lastPoint;
		float velocity = glm::length(direction);
		if (velocity > 0.0001) // If little movement - do nothing.
		{
			//Apply the translation
			switch (Mode)
			{
			case CAMERA:
				translateCamera({ direction.x * m_TRANSSCALE, direction.y * m_TRANSSCALE, 0 });
				break;
			}
		}
		break;
	}
	// Save the location of the current point for the next movement.
	lastPoint = curPoint;
	}
}

void Window::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		glm::vec2 point = mouse_point;
		if (button == GLFW_MOUSE_BUTTON_LEFT)
		{
			//Rotate mode
			Movement = ROTATE;
			lastPoint = trackBallMapping(point);
		}

		else if (button == GLFW_MOUSE_BUTTON_RIGHT)
		{
			//Translate mode
			Movement = TRANSLATE;
			lastPoint = trackBallMapping(point);
		}
	}

	else if (action == GLFW_RELEASE)
	{
		Movement = NONE;
	}
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	switch (Mode)
	{
	case CAMERA:
		translateCamera({ 0,0, -1 * yoffset * m_ZOOMSCALE });
		break;
	}

}

glm::vec3 Window::trackBallMapping(glm::vec2 point)
{
	glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v.x = (2.0f*point.x - Window::width) / Window::width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v.y = (Window::height - 2.0f*point.y) / Window::height;   // this does the equivalent to the above for the mouse Y position
	v.z = 0.0f;   // initially the mouse z position is set to zero, but this will change below
	d = glm::length(v);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d < 1.0f) ? d : 1.0f;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v.z = sqrtf(1.001f - d*d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	glm::normalize(v); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
}

//Camera transformations
void Window::translateCamera(glm::vec3 transVec) {
	V = glm::translate(glm::mat4(1.0f), transVec) * V;
}

void Window::rotateCamera(float rot_angle, glm::vec3 rotAxis) {
	V = glm::rotate(glm::mat4(1.0f), rot_angle / 180.0f * glm::pi<float>(), rotAxis) * V;
}

void Window::resetCamera() {
	V = glm::lookAt(cam_pos, cam_look_at, cam_up);	
}

void Window::printMatrix(glm::mat4x3 mat) {

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			cout << mat[j][i] << " ";
		}
		cout << endl;
	}
	cout << endl;
}