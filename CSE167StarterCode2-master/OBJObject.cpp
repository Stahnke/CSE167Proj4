#include "OBJObject.h"
#include "Window.h"

OBJObject::OBJObject(const char * filePath)
{

	forward_vec = { 0.0f,0.0f,1.0f,0.0f };
	theta = 0.0f;

	toWorld = glm::mat4(1.0f);

	parse1(filePath);
	parse2(filePath);

	vector<glm::vec3> combinedVertices;

	for (int i = 0; i < vertices.size(); i++)
	{
		combinedVertices.push_back(vertices[i]);
		combinedVertices.push_back(normals[i]);
	}

	// Create array object and buffers. Remember to delete your buffers when the object is destroyed!
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	// Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
	// Consider the VAO as a container for all your buffers.
	glBindVertexArray(VAO);

	// Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
	// you want to draw, such as vertices, normals, colors, etc.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
	// the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
	glBufferData(GL_ARRAY_BUFFER, combinedVertices.size() * sizeof(glm::vec3), &combinedVertices[0], GL_STATIC_DRAW);
	// Enable the usage of layout location 0 (check the vertex shader to see what this is)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
		3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
		GL_FLOAT, // What type these components are
		GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
		6 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
		(GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
	// We've sent the vertex data over to OpenGL, but there's still something missing.
	// In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the VAO now so we don't accidentally tamper with it.
	// NOTE: You must NEVER unbind the element array buffer associated with a VAO!
	glBindVertexArray(0);

}

OBJObject::~OBJObject()
{
	// Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a 
	// large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void OBJObject::draw(GLuint shaderProgram)
{ 
	// Calculate the combination of the model and view (camera inverse) matrices
	glm::mat4 modelview = Window::V * toWorld;

	// We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
	// Consequently, we need to forward the projection, view, and model matrices to the shader programs
	// Get the location of the uniform variables "projection" and "modelview"
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModel = glGetUniformLocation(shaderProgram, "model");
	uView = glGetUniformLocation(shaderProgram, "view");
	// Now send these values to the shader program
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);
	glUniformMatrix4fv(uView, 1, GL_FALSE, &Window::V[0][0]);

	// Now draw the OBJObject. We simply need to bind the VAO associated with it.
	glBindVertexArray(VAO);
	// Tell OpenGL to draw with triangles, using indices.size() indices, the type of the indices, and the offset to start from
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
	glBindVertexArray(0);
}

void OBJObject::update()
{
	spin(1.0f);
}

void OBJObject::spin(float deg)
{
	// If you haven't figured it out from the last project, this is how you fix spin's behavior
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), 1.0f / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::parse1(const char *filepath) 
{
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { cerr << "error loading file:\n" << filepath << endl; exit(-1); }  // just in case the file can't be found or is corrupt

	maxX, maxY, maxZ = std::numeric_limits<float>::max() * -1;
	minX, minY, minZ = std::numeric_limits<float>::max();
	avgCenterX, avgCenterY, avgCenterZ, avgScaleX, avgScaleY, avgScaleZ = 0;
	objectSize = 0;

	int count_v = 0;
	int count_vn = 0;
	cout << filepath << endl;
	while ((c1 = fgetc(fp)) != EOF)
	{
		if ((c1 == 'v'))
		{
			c2 = fgetc(fp);
			if (c2 == ' ')
			{
				count_v++;
				fscanf(fp, "%f %f %f", &x, &y, &z);
				//Get Max/Min values
				if (x > maxX) maxX = x;
				if (y > maxY) maxY = y;
				if (z > maxZ) maxZ = z;

				if (x < minX) minX = x;
				if (y < minY) minY = y;
				if (z < minZ) minZ = z;

				//cout << count_v << "." << "x: " << x << " y: " << y << " z: " << z << endl;
			}
		}
	}
	//cout << "Maxes:" << " x = " << maxX << " y = " << maxY << " z = " << maxZ << endl;
	//cout << "Mins:" << " x = " << minX << " y = " << minY << " z = " << minZ << endl;
	//calculate centering averages
	avgCenterX = (maxX + minX) / 2.0f;
	avgCenterY = (maxY + minY) / 2.0f;
	avgCenterZ = (maxZ + minZ) / 2.0f;
	//cout << "Centering Averages:" << " x = " << avgCenterX << " y = " << avgCenterY << " z = " << avgCenterZ << endl;

	//calculate scaling averages
	avgScaleX = (maxX - minX) / 2.0f;
	avgScaleY = (maxY - minY) / 2.0f;
	avgScaleZ = (maxZ - minZ) / 2.0f;
	//cout << "Scaling Averages:" << " x = " << avgScaleX << " y = " << avgScaleY << " z = " << avgScaleZ << endl;

	//Calculate object box size
	objectSize = std::max(avgScaleX, avgScaleY);
	objectSize = std::max(objectSize, avgCenterZ);
	//cout << "Box Size = " << objectSize << endl;
	
	cout << endl;
	fclose(fp);   // make sure you don't forget to close the file when done
}

void OBJObject::parse2(const char *filepath)
{
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { cerr << "error loading file:\n" << filepath << endl; exit(-1); }  // just in case the file can't be found or is corrupt

	int count_v = 0;
	int count_vn = 0;
	int count_f = 0;
	//cout << filepath << endl;
	while ((c1 = fgetc(fp)) != EOF)
	{
		if ((c1 == 'v'))
		{
			c2 = fgetc(fp);
			if (c2 == ' ')
			{
				count_v++;
				fscanf(fp, "%f %f %f", &x, &y, &z);

				
				x = (x - avgCenterX) / objectSize;
				y = (y - avgCenterY) / objectSize;
				z = (z - avgCenterZ) / objectSize;
				

				vertices.push_back({ x,y,z });
				//cout << count_v << "." << "x: " << x << " y: " << y << " z: " << z << endl;
			}

			else if (c2 == 'n')
			{
				count_vn++;
				fscanf(fp, "%f %f %f", &x, &y, &z);
				
				/*
				x = (x - avgCenterX) / objectSize;
				y = (y - avgCenterY) / objectSize;
				z = (z - avgCenterZ) / objectSize;
				*/

				normals.push_back({ x,y,z });
				//cout << count_vn << "." << "r: " << r << " g: " << g << " b: " << b << endl;
			}
		}

		else if ((c1 == 'f'))
		{
			c2 = fgetc(fp);
			if (c2 == ' ')
			{
				int a = 0;
				count_f += 3;
				fscanf(fp, "%f//%f %f//%f %f//%f", &x, &y, &z, &r, &g, &b);
				indices.push_back(x - 1);
				indices.push_back(z - 1);
				indices.push_back(g - 1);
				//cout << count_f << ". " << "f1: " << x - 1 << " f2: " << z - 1 << " f3: " << g - 1 << endl;
			}
		}

		else if ((c1 == '#'))
		{
			c2 = fgetc(fp);
			if (c2 == ' ')
			{
				c3 = fgetc(fp);
				if (c3 == 'E')
				{
					break;
				}
			}
		}
	}

	fclose(fp);   // make sure you don't forget to close the file when done
}

void OBJObject::translate(glm::vec3 transVec)
{
	toWorld = glm::translate(glm::mat4(1.0f), transVec) * toWorld;
}

void OBJObject::resize(float change)
{
	this->size += change;
	if (size < 1) size = 1;
	glPointSize(size);
}

void OBJObject::scale(glm::vec3 scaleVector)
{
	toWorld = toWorld * glm::scale(glm::mat4(1.0f), scaleVector);
}

void OBJObject::orbit(float deg)
{
	// This creates the matrix to rotate the object
	toWorld = glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)) * toWorld;
}

void OBJObject::reset()
{
	toWorld = glm::mat4(1.0f);
}

std::vector<glm::vec3> OBJObject::getVertices() {
	return vertices;
}

std::vector<glm::vec3> OBJObject::getNormals() {
	return normals;
}

glm::mat4 OBJObject::getToWorld() {
	return toWorld;
}

float OBJObject::getPointSize() {
	return size;
}

void OBJObject::initPosition(glm::vec3 position) {
	currentPos = position;
	toWorld = glm::translate(glm::mat4(1.0f), position);
}

void OBJObject::setPosition(glm::vec3 position)
{
	//Calculate the next forward vector
	//Get angle between the current and next forward vector
	newForward_vec = glm::vec4{position - currentPos, 0.0f };
	glm::vec4 cur_xz_vec = glm::vec4{ forward_vec.x, 0.0f, forward_vec.z, 0.0f };
	glm::vec4 new_xz_vec = glm::vec4{ newForward_vec.x, 0.0f, newForward_vec.z, 0.0f };
	float len_cur_xz = glm::sqrt(glm::pow(cur_xz_vec.x, 2) + glm::pow(cur_xz_vec.y, 2) + glm::pow(cur_xz_vec.z, 2));
	float len_new_xz = glm::sqrt(glm::pow(new_xz_vec.x, 2) + glm::pow(new_xz_vec.y, 2) + glm::pow(new_xz_vec.z, 2));
	float dot_vec = glm::dot(cur_xz_vec, new_xz_vec);
	glm::vec3 cross = glm::cross(glm::vec3(cur_xz_vec), glm::vec3(new_xz_vec));
	float normalizer = len_cur_xz * len_new_xz;
	if (normalizer != 0) {
		float divid = dot_vec / normalizer;
		if (divid > 1) divid = 1;
		if (divid < -1) divid = -1;
		float acos = glm::acos(divid);

		if (cross.y > 0)
			theta += acos;
		else if (cross.y < 0)
			theta -= acos;

		/*cout << "CurrentPos = " << currentPos.x << " " << currentPos.y << " " << currentPos.z << endl;
		cout << " NewPos = " << position.x << " " << position.y << " " << position.z << endl;
		cout << "CurrentVec = " << forward_vec.x << " " << forward_vec.y << " " << forward_vec.z << endl;
		cout << "NewVec = " << newForward_vec.x << " " << newForward_vec.y << " " << newForward_vec.z << endl;
		cout << len_cur_xz << endl;
		cout << len_new_xz << endl;
		cout << dot_vec << endl;
		cout << normalizer << endl;
		cout << dot_vec / normalizer << endl;
		cout << acos << endl;
		cout << "Theta = " << theta << endl << endl;*/
	}


	//set our current position/vector to the new position/vector
	forward_vec = newForward_vec;
	currentPos = position;

	toWorld = glm::translate(glm::mat4(1.0f), position);
	toWorld = toWorld * glm::rotate(glm::mat4(1.0f), theta, { 0.0f,1.0f,0.0f });
}
