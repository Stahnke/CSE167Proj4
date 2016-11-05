#include "window.h"

const char* window_title = "GLFW Starter Project";

Skybox * skybox;
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
	// Call the update function the cube
	//cube->update();
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