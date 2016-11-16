#include "window.h"

const char* window_title = "GLFW Starter Project";

Skybox * skybox;
Cube * cube;
BezierCurve * bezierCurve0, * bezierCurve1, * bezierCurve2, * bezierCurve3, * bezierCurve4, * bezierCurve5;
BezierCurve * bezierCurve6, * bezierCurve7, * bezierCurve8, * bezierCurve9;
AnchorPoint * anchorPoint0, * anchorPoint1, * anchorPoint2, * anchorPoint3, * anchorPoint4, * anchorPoint5;
AnchorPoint * anchorPoint6, * anchorPoint7, * anchorPoint8, * anchorPoint9, * anchorPoint10, * anchorPoint11;
AnchorPoint * anchorPoint12, * anchorPoint13, * anchorPoint14, * anchorPoint15, * anchorPoint16, * anchorPoint17;
AnchorPoint * anchorPoint18, * anchorPoint19, * anchorPoint20, * anchorPoint21, * anchorPoint22, * anchorPoint23;
AnchorPoint * anchorPoint24, * anchorPoint25, * anchorPoint26, * anchorPoint27, * anchorPoint28, * anchorPoint29;
AnchorLines * anchorLines0, *anchorLines1, *anchorLines2, *anchorLines3, *anchorLines4, *anchorLines5, *anchorLines6;
AnchorLines  * anchorLines7, * anchorLines8, *anchorLines9, *anchorLines10;
vector<AnchorPoint *> points;
vector<BezierCurve *> curves;
vector<AnchorLines *> lines;
unsigned char pix[4];

float a = 0.000001f;
float c = 0.001f;
float maxHeight = std::numeric_limits<int>::min();
float currentHeight = std::numeric_limits<int>::min();
float maxT = 0.0f;
float currentT = 0.0f;

glm::mat4x3 ret;

const unsigned int RED = 0;
const unsigned int YELLOW = 1;

//Vars for bezier curves
const unsigned int N = 150;
glm::vec3 p00, p01, p02, p03;
glm::vec3 p11, p12, p13;
glm::vec3 p21, p22, p23;
glm::vec3 p31, p32, p33;
glm::vec3 p41, p42, p43;
glm::vec3 p51, p52, p53;
glm::vec3 p61, p62, p63;
glm::vec3 p71, p72, p73;
glm::vec3 p81, p82, p83;
glm::vec3 p91, p92;
glm::mat4x3 controlPts0, controlPts1, controlPts2, controlPts3, controlPts4, controlPts5, controlPts6, controlPts7, controlPts8, controlPts9;

vector<glm::vec3> selections;
glm::vec3 selected;
GLint shaderProgram;
GLint skyboxShaderProgram;
GLint selectionShaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"

#define SKYBOX_VERTEX_SHADER_PATH "../skyboxShader.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "../skyboxShader.frag"

#define SELECTION_VERTEX_SHADER_PATH "../selectionShader.vert"
#define SELECTION_FRAGMENT_SHADER_PATH "../selectionShader.frag"

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
const int SELECTION = 2;

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
	p11 = { 5.0f,-5.0f,5.0f };
	p12 = { 10.0f,-5.0f,0.0f };
	p13 = { 10.0f,0.0f,0.0f };
	controlPts1 = { p03,p11,p12,p13 };
	bezierCurve1 = new BezierCurve(N, controlPts1);

	//Bezier Curve 2
	p21 = { 10.0f,5.0f,0.0f };
	p22 = { 15.0f,5.0f,-5.0f };
	p23 = { 15.0f,0.0f,-5.0f };
	controlPts2 = { p13,p21,p22,p23 };
	bezierCurve2 = new BezierCurve(N, controlPts2);

	//Bezier Curve 3
	p31 = { 15.0f,-5.0f,-5.0f };
	p32 = { 10.0f,-5.0f,-10.0f };
	p33 = { 10.0f,0.0f,-10.0f };
	controlPts3 = { p23,p31,p32,p33 };
	bezierCurve3 = new BezierCurve(N, controlPts3);

	//Bezier Curve 4
	p41 = { 10.0f,5.0f,-10.0f };
	p42 = { 5.0f,5.0f,-15.0f };
	p43 = { 5.0f,0.0f,-15.0f };
	controlPts4 = { p33,p41,p42,p43 };
	bezierCurve4 = new BezierCurve(N, controlPts4);

	//Bezier Curve 5
	p51 = { 5.0f,-5.0f,-15.0f };
	p52 = { 0.0f,-5.0f,-10.0f };
	p53 = { 0.0f,0.0f,-10.0f };
	controlPts5 = { p43,p51,p52,p53 };
	bezierCurve5 = new BezierCurve(N, controlPts5);

	//Bezier Curve 6
	p61 = { 0.0f,5.0f,-10.0f };
	p62 = { -5.0f,5.0f,-5.0f };
	p63 = { -5.0f,0.0f,-5.0f };
	controlPts6 = { p53,p61,p62,p63 };
	bezierCurve6 = new BezierCurve(N, controlPts6);

	//Bezier Curve 7
	p71 = { -5.0f,-5.0f,-5.0f };
	p72 = { -10.0f,-5.0f,0.0f };
	p73 = { -10.0f,0.0f,0.0f };
	controlPts7 = { p63,p71,p72,p73 };
	bezierCurve7 = new BezierCurve(N, controlPts7);

	//Bezier Curve 8
	p81 = { -10.0f,5.0f,0.0f };
	p82 = { -5.0f,5.0f,5.0f };
	p83 = { -5.0f,0.0f,5.0f };
	controlPts8 = { p73,p81,p82,p83 };
	bezierCurve8 = new BezierCurve(N, controlPts8);

	//Bezier Curve 9
	p91 = { -5.0f,-5.0f,5.0f };
	p92 = { 0.0f,-5.0f,5.0f };
	controlPts9 = { p83,p91,p92,p00 };
	bezierCurve9 = new BezierCurve(N, controlPts9);


	//Add all points to selection buffer
	selections.push_back(p00);
	selections.push_back(p01);
	selections.push_back(p02);
	selections.push_back(p03);
	selections.push_back(p11);
	selections.push_back(p12);
	selections.push_back(p13);
	selections.push_back(p21);
	selections.push_back(p22);
	selections.push_back(p23);
	selections.push_back(p31);
	selections.push_back(p32);
	selections.push_back(p33);
	selections.push_back(p41);
	selections.push_back(p42);
	selections.push_back(p43);
	selections.push_back(p51);
	selections.push_back(p52);
	selections.push_back(p53);
	selections.push_back(p61);
	selections.push_back(p62);
	selections.push_back(p63);
	selections.push_back(p71);
	selections.push_back(p72);
	selections.push_back(p73);
	selections.push_back(p81);
	selections.push_back(p82);
	selections.push_back(p83);
	selections.push_back(p91);
	selections.push_back(p92);
	
	//Add all curves to curve list
	curves.push_back(bezierCurve0);
	curves.push_back(bezierCurve1);
	curves.push_back(bezierCurve2);
	curves.push_back(bezierCurve3);
	curves.push_back(bezierCurve4);
	curves.push_back(bezierCurve5);
	curves.push_back(bezierCurve6);
	curves.push_back(bezierCurve7);
	curves.push_back(bezierCurve8);
	curves.push_back(bezierCurve9);

	//Anchor Points
	anchorPoint0 = new AnchorPoint(p00, 0, RED);
	anchorPoint1 = new AnchorPoint(p01, 1, YELLOW);
	anchorPoint2 = new AnchorPoint(p02, 2, YELLOW);
	anchorPoint3 = new AnchorPoint(p03, 3, RED);
	anchorPoint4 = new AnchorPoint(p11, 4, YELLOW);
	anchorPoint5 = new AnchorPoint(p12, 5, YELLOW);
	anchorPoint6 = new AnchorPoint(p13, 6, RED);
	anchorPoint7 = new AnchorPoint(p21, 7, YELLOW);
	anchorPoint8 = new AnchorPoint(p22, 8, YELLOW);
	anchorPoint9 = new AnchorPoint(p23, 9, RED);
	anchorPoint10 = new AnchorPoint(p31, 10, YELLOW);
	anchorPoint11 = new AnchorPoint(p32, 11, YELLOW);
	anchorPoint12 = new AnchorPoint(p33, 12, RED);
	anchorPoint13 = new AnchorPoint(p41, 13, YELLOW);
	anchorPoint14 = new AnchorPoint(p42, 14, YELLOW);
	anchorPoint15 = new AnchorPoint(p43, 15, RED);
	anchorPoint16 = new AnchorPoint(p51, 16, YELLOW);
	anchorPoint17 = new AnchorPoint(p52, 17, YELLOW);
	anchorPoint18 = new AnchorPoint(p53, 18, RED);
	anchorPoint19 = new AnchorPoint(p61, 19, YELLOW);
	anchorPoint20 = new AnchorPoint(p62, 20, YELLOW);
	anchorPoint21 = new AnchorPoint(p63, 21, RED);
	anchorPoint22 = new AnchorPoint(p71, 22, YELLOW);
	anchorPoint23 = new AnchorPoint(p72, 23, YELLOW);
	anchorPoint24 = new AnchorPoint(p73, 24, RED);
	anchorPoint25 = new AnchorPoint(p81, 25, YELLOW);
	anchorPoint26 = new AnchorPoint(p82, 26, YELLOW);
	anchorPoint27 = new AnchorPoint(p83, 27, RED);
	anchorPoint28 = new AnchorPoint(p91, 28, YELLOW);
	anchorPoint29 = new AnchorPoint(p92, 29, YELLOW);

	points.push_back(anchorPoint0);
	points.push_back(anchorPoint1);
	points.push_back(anchorPoint2);
	points.push_back(anchorPoint3);
	points.push_back(anchorPoint4);
	points.push_back(anchorPoint5);
	points.push_back(anchorPoint6);
	points.push_back(anchorPoint7);
	points.push_back(anchorPoint8);
	points.push_back(anchorPoint9);
	points.push_back(anchorPoint10);
	points.push_back(anchorPoint11);
	points.push_back(anchorPoint12);
	points.push_back(anchorPoint13);
	points.push_back(anchorPoint14);
	points.push_back(anchorPoint15);
	points.push_back(anchorPoint16);
	points.push_back(anchorPoint17);
	points.push_back(anchorPoint18);
	points.push_back(anchorPoint19);
	points.push_back(anchorPoint20);
	points.push_back(anchorPoint21);
	points.push_back(anchorPoint22);
	points.push_back(anchorPoint23);
	points.push_back(anchorPoint24);
	points.push_back(anchorPoint25);
	points.push_back(anchorPoint26);
	points.push_back(anchorPoint27);
	points.push_back(anchorPoint28);
	points.push_back(anchorPoint29);

	anchorLines0 = new AnchorLines(p92, p01);
	anchorLines1 = new AnchorLines(p02, p11);
	anchorLines2 = new AnchorLines(p12, p21);
	anchorLines3 = new AnchorLines(p22, p31);
	anchorLines4 = new AnchorLines(p32, p41);
	anchorLines5 = new AnchorLines(p42, p51);
	anchorLines6 = new AnchorLines(p52, p61);
	anchorLines7 = new AnchorLines(p62, p71);
	anchorLines8 = new AnchorLines(p72, p81);
	anchorLines9 = new AnchorLines(p82, p91);

	lines.push_back(anchorLines0);
	lines.push_back(anchorLines1);
	lines.push_back(anchorLines2);
	lines.push_back(anchorLines3);
	lines.push_back(anchorLines4);
	lines.push_back(anchorLines5);
	lines.push_back(anchorLines6);
	lines.push_back(anchorLines7);
	lines.push_back(anchorLines8);
	lines.push_back(anchorLines9);

	//recalculate maxHeight
	int i = 0;
	maxHeight = std::numeric_limits<int>::min();
	for each (BezierCurve * curve in curves) {
		if (curve->getMaxHeight()[0] > maxHeight)
		{
			maxHeight = curve->getMaxHeight()[0];
			maxT = curve->getMaxHeight()[1] + i;
		}
		i++;
	}
	//cout <<"MaxHeight = " << maxHeight << "@ " << maxT << endl;

	//OBJECT POSITION SHOULD BE EQUAL TO C(maxT) on initialization
	//TODO:
	currentHeight = maxHeight;
	currentT = maxT;
	cube = new Cube();
	glm::vec3 newPoint = curves[(int)maxT]->calcBezierPoint(maxT - (int)maxT);
	cube->setPosition(newPoint);
	

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	skyboxShaderProgram = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
	selectionShaderProgram = LoadShaders(SELECTION_VERTEX_SHADER_PATH, SELECTION_FRAGMENT_SHADER_PATH);

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
	
	//Update the position of the cube
	float v = std::sqrt(((-2) * a * (currentHeight - maxHeight))) + c;
	if (currentHeight - maxHeight > 0)
	{
		//cout << "CALCULATION ERROR" << endl;
		v = 0 + c;
	}

	currentT += v;
	if (currentT > curves.size() || currentT < 0)
		currentT = 0;
	//cout << currentT << endl;
	glm::vec3 newPoint = curves[(int)currentT]->calcBezierPoint(currentT - (int)currentT);
	currentHeight = newPoint.y;
	cube->setPosition(newPoint);
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

	glPointSize(10.0f);
	// Render Anchor Points
	for each (AnchorPoint * point in points)
		point->draw(shaderProgram);

	for each (AnchorLines * line in lines)
		line->draw(shaderProgram);

	cube->draw(shaderProgram);
	
	glPointSize(1.0f);
	
	// Gets events, including input such as keyboard and mouse or window resizing
	// Swap buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::selectionDraw()
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(selectionShaderProgram);

	glPointSize(10.0f);
	// Render Anchor Points
	for each(AnchorPoint * point in points)
		point->drawSelection(selectionShaderProgram);
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
			case SELECTION:
				translateSelection({ direction.x * m_TRANSSCALE, direction.y * m_TRANSSCALE, 0 });
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


			selectionDraw();

			glReadPixels(point.x, height - point.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &pix);
			//cout << (unsigned int) pix[0] << endl;

			if ((unsigned int)pix[0] < selections.size())
			{
				Movement = TRANSLATE;
				Mode = SELECTION;
				selected = selections[(unsigned int)pix[0]];
				
			}

			else
			{
				//Rotate mode
				Movement = ROTATE;
				Mode = CAMERA;
				lastPoint = trackBallMapping(point);
			}
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
		Mode = CAMERA;
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

//Selection transformations
void Window::translateSelection(glm::vec3 transVec) {
	selections[pix[0]] = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(selections[pix[0]], 1.0f));
	//update point position
	points[pix[0]]->update(selections[pix[0]]);

	if ((pix[0] + 3) % 3 == 0)
	{
		if (pix[0] > 0)
		{
			selections[pix[0] + 1] = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(selections[pix[0] + 1], 1.0f));
			points[pix[0] + 1]->update(selections[pix[0] + 1]);
			selections[pix[0] - 1] = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(selections[pix[0] - 1], 1.0f));
			points[pix[0] - 1]->update(selections[pix[0] - 1]);
			lines[pix[0] / 3]->update(selections[pix[0] + 1], selections[pix[0] - 1]);
		}
		else
		{
			selections[pix[0] + 1] = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(selections[pix[0] + 1], 1.0f));
			points[pix[0] + 1]->update(selections[pix[0] + 1]);
			selections[selections.size() - 1] = glm::vec3(glm::translate(glm::mat4(1.0f), transVec) * glm::vec4(selections[selections.size() - 1], 1.0f));
			points[selections.size() - 1]->update(selections[selections.size() - 1]);
			lines[pix[0] / 3]->update(selections[pix[0] + 1], selections[selections.size() -1]);
		}
	}
	else
	{
		if (((pix[0] * 2) + 2) % 6 == 0)
		{
			if (pix[0] < selections.size() - 1)
			{
				selections[pix[0] + 2] = glm::vec3(glm::translate(glm::mat4(1.0f), -transVec) * glm::vec4(selections[pix[0] + 2], 1.0f));
				points[pix[0] + 2]->update(selections[pix[0] + 2]);
				lines[(pix[0] + 3) / 3]->update(selections[pix[0]], selections[pix[0] + 2]);
			}
			else
			{
				selections[1] = glm::vec3(glm::translate(glm::mat4(1.0f), -transVec) * glm::vec4(selections[1], 1.0f));
				points[1]->update(selections[1]);
				lines[0]->update(selections[pix[0]], selections[1]);
			}
		}
		else
		{
			if (pix[0] > 1)
			{
				selections[pix[0] - 2] = glm::vec3(glm::translate(glm::mat4(1.0f), -transVec) * glm::vec4(selections[pix[0] - 2], 1.0f));
				points[pix[0] - 2]->update(selections[pix[0] - 2]);
				lines[(pix[0]) / 3]->update(selections[pix[0]], selections[pix[0] - 2]);
			}
			else
			{
				selections[selections.size() -1] = glm::vec3(glm::translate(glm::mat4(1.0f), -transVec) * glm::vec4(selections[selections.size() -1], 1.0f));
				points[selections.size() -1]->update(selections[selections.size() -1]);
				lines[0]->update(selections[pix[0]], selections[selections.size() -1]);
			}
		}
	}
	//recalculate curves
	ret = findPointsInCurve(pix[0] / 3);
	curves[pix[0] / 3]->update(ret);

	if((pix[0] + 3) % 3 == 0)
	{
		if ((pix[0] - 3 / 3) > 0)
		{
			ret = findPointsInCurve((pix[0] - 3) / 3);
			curves[(pix[0] - 3) / 3]->update(ret);
		}

		else
		{
			ret = findPointsInCurve(curves.size() - 1);
			curves[(curves.size() - 1)]->update(ret);
		}
	}
	else
	{
		if (((pix[0] * 2) + 2) % 6 == 0)
		{
			if (pix[0] < selections.size() - 1)
			{
				ret = findPointsInCurve((pix[0] + 3) / 3);
				curves[(pix[0] + 3) / 3]->update(ret);
			}
			else
			{
				ret = findPointsInCurve(0);
				curves[0]->update(ret);
			}
		}
		else
		{
			if (pix[0] > 1)
			{
				ret = findPointsInCurve((pix[0] - 3) / 3);
				curves[(pix[0] - 3) / 3]->update(ret);
			}
			else
			{
				ret = findPointsInCurve(curves.size() -1);
				curves[curves.size() -1]->update(ret);
			}
		}
	}

	//recalculate maxHeight
	int i = 0;
	maxHeight = std::numeric_limits<int>::min();
	for each (BezierCurve * curve in curves) {
		if (curve->getMaxHeight()[0] > maxHeight)
		{
			maxHeight = curve->getMaxHeight()[0];
			maxT = curve->getMaxHeight()[1] + i;
		}
		i++;
	}
	//cout <<"MaxHeight = " << maxHeight << "@ " << maxT << endl;
}

void Window::remakePoints() {
	//Anchor Points

}

glm::mat4x3 Window::findPointsInCurve(unsigned int curveNum) {

	if ((((curveNum) * 3) + 3) < selections.size())
		ret = { selections[((curveNum) * 3) + 0], selections[((curveNum) * 3) + 1], selections[((curveNum) * 3) + 2], selections[((curveNum) * 3) + 3] };
	else 
		ret = { selections[((curveNum) * 3) + 0], selections[((curveNum) * 3) + 1], selections[((curveNum) * 3) + 2], selections[0] };
	
	return ret;
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