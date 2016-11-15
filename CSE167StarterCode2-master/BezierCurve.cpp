#include "BezierCurve.h"
#include "Window.h"

vector <glm::vec3> bezierVertices;
vector <unsigned int> bezierIndices;
bool DEBUG = false;

BezierCurve::BezierCurve(unsigned int N, glm::mat4x3 controlPts) : N(N), controlPts(controlPts)
{
	toWorld = glm::mat4(1.0f);

	color = { 0.0f,0.0f,0.0f };

	//Calculate the curve on instantiation
	calcBezierCurve();

	//DEBUG
	if (DEBUG)
	{
		for (int x = 0; x < bezierVertices.size(); x++)
			cout << x << ": " << bezierVertices[x].x << " " << bezierVertices[x].y << " " << bezierVertices[x].z << endl;;

		//for (int x = 0; x < bezierIndices.size(); x++)
			//cout << x+1<< ": " << bezierIndices[x] << endl;;
		//cout << bezierIndices.size() << endl;
	}

	glGenVertexArrays(1, &bezierVAO);
	glGenBuffers(1, &bezierVBO);
	glGenBuffers(1, &bezierEBO);
	
	glBindVertexArray(bezierVAO);

	glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);

	glBufferData(GL_ARRAY_BUFFER, bezierVertices.size() * sizeof(glm::vec3), &bezierVertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bezierIndices.size() * sizeof(unsigned int), &bezierIndices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

BezierCurve::~BezierCurve()
{
	glDeleteVertexArrays(1, &bezierVAO);
	glDeleteBuffers(1, &bezierVBO);
	glDeleteBuffers(1, &bezierEBO);
}

void BezierCurve::draw(GLuint shaderProgram)
{ 
	glm::mat4 modelview = Window::V * toWorld;	

	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	
	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);

	GLuint uColorLoc = glGetUniformLocation(shaderProgram, "myColor");
	glUniform3f(uColorLoc, color.x, color.y, color.z);

	glBindVertexArray(bezierVAO);
	
	glDrawElements(GL_LINES, bezierIndices.size(), GL_UNSIGNED_INT, 0);
	
	glBindVertexArray(0);
}

void BezierCurve::update()
{
	//Recalculate the curve anytime we update the parameters
	calcBezierCurve();
}


glm::vec4 BezierCurve::calcBernstein(const float t) {
	glm::vec4 bernVec;
	const unsigned int controlPtNum = 4;	//Number of control points we will use
	float n_choose_k;

	// Note: Even though we can change the controlPtNum to whatever we want, our application will always use 4, 
	// because we need to return the data as a vec4, if we could easily represent vec5,vec6, etc, then maybe we could
	// utilize the controlPtNum.

	//Calculate C0,C1,C2,C3, store into a vec4
	for (unsigned int i = 0; i < controlPtNum; i++)
	{
		//Calc n choose k
		n_choose_k = calcChoose(controlPtNum - 1, i);

		bernVec[i] = n_choose_k * (pow((1 - t), (controlPtNum - 1) - i) * pow(t, i));
		//cout << bernVec[i] << endl;
	}
	 
	//cout << endl;

	//Return the vec4[C0,C1,C2,C3]
	return bernVec;
}

glm::vec3 BezierCurve::calcBezierPoint(const float t) {
	glm::vec4 bern;				//The Bernstein coefficients at t
	glm::vec3 bezierPoint;		//The Bezier point at t

	bern = calcBernstein(t);	//Calculate the bernstein co at t
	bezierPoint = controlPts * bern;	//Calculate the Bezier point at t

	return bezierPoint;		//return the Bezier point at t
}

void BezierCurve::calcBezierCurve() {

	vector <glm::vec3> curveVertices;	//Vector for our vertices to draw the curve
	vector <unsigned int> curveIndices; //Vecotr for our indices to draw at
	glm::vec3 curBezierPoint;			//The current Bezier Point, we will use 2 at a time to draw a segment, and draw all segments to make the curve

	//N = number of segments to draw for the curve

	//We iterate from segments n = 0 to N
	//For each segment, n, we need to calculate the starting Bezier vertex
	//Then push it to our vertices container
	for (unsigned int n = 0; n < N + 1; n++)
	{
		
		//Get the Bezier Point at t = n / N
		curBezierPoint = calcBezierPoint(((float)n) / N);
		//Store the point in our vertices container
		curveVertices.push_back(curBezierPoint);

		//Store the indices we want to draw line segments with
		if (n < N)
		{
			curveIndices.push_back(n);
			curveIndices.push_back(n + 1);
		}
	}

	bezierIndices = curveIndices;
	bezierVertices = curveVertices;

}

float BezierCurve::calcChoose(unsigned int n, unsigned int k) {
	float nFac = 1;
	float n_minus_kFac = 1;
	float kFac = 1;
	float n_choose_k;

	for (int x = 1; x < n + 1; x++) {
		nFac *= x;
	}

	for (int x = 1; x < (n - k) + 1; x++) {
		n_minus_kFac *= x;
	}

	for (int x = 1; x < k + 1; x++) {
		kFac *= x;
	}

	n_choose_k = nFac / (n_minus_kFac * kFac);

	return n_choose_k;
	
}

