#define _USE_MATH_DEFINES

#ifdef MAC
#include <GL/freeglut.h>
#else
#include <GL/freeglut.h>
#endif

#include <cstdlib>
#include <vector>
#include <cmath>
#include <chrono>
#include <iostream>
#include <random>

using namespace std;

//simple struct to represent a 3d vertex
struct Vertex3D {
	int x;
	int y;
	int z;

	//default ctor, initialize point to origin
	Vertex3D() {
		this->x = 0;
		this->y = 0;
		this->z = 0;
	}

	//3 int ctor, initialize point to specified location
	Vertex3D(int inX, int inY, int inZ) {
		this->x = inX;
		this->y = inY;
		this->z = inZ;
	}

	//copy ctor
	Vertex3D(const Vertex3D& copyMe) {
		this->x = copyMe.x;
		this->y = copyMe.y;
		this->z = copyMe.z;
	}

	//assignment operator redefinition
	void operator=(const Vertex3D& other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}
};

//simple struct to hold individual firework data
struct Firework {
	Vertex3D center;
	int numLines;
	vector<Vertex3D> particles;
	float rgb[3];

	//default ctor, initialize to the "empty" firework
	Firework() {
		Vertex3D defaultCenter(0, 0, 0);
		this->center = defaultCenter;
		this->numLines = 0;
		for (int i = 0; i < 3; i++) {
			this->rgb[i] = 0.0f;
		}

		for (int i = 0; i < this->numLines; i++)
		{
			this->particles.push_back(getPoint());
		}
	}

	//vertex ctor, initialize location to specified point
	//and store a random number of (x, y, z) endpoints at random
	//locations
	Firework(const Vertex3D inpCenter) {
		this->center = inpCenter;
		this->numLines = (rand() % 50) + 100;

		//initialize to random rgb color config
		for (int i = 0; i < 3; i++) {
			this->rgb[i] = (double)rand() / RAND_MAX;
		}

		for (int i = 0; i < this->numLines; i++)
		{
			this->particles.push_back(getPoint());
		}
	}

	//copy ctor
	Firework(const Firework& copyMe) {
		this->center = copyMe.center;
		this->numLines = copyMe.numLines;
		this->particles = copyMe.particles;
		for (int i = 0; i < 3; i++) {
			this->rgb[i] = copyMe.rgb[i];
		}
	}

	//returns random, well-distributed point around sphere of radius 300 centered at firework center
	Vertex3D getPoint() 
	{
		//get random standard normally distributed numbers
		unsigned seed = chrono::system_clock::now().time_since_epoch().count();
		default_random_engine generator(seed);
		normal_distribution<double> d(0.0, 1.0);
		double u = (double)rand();
		double x = d(generator);
		double y = d(generator);
		double z = d(generator);

		//normalize vector
		double magnitude = sqrt((x * x) + (y * y) + (z * z));
		x /= magnitude; 
		y /= magnitude; 
		z /= magnitude;

		//radius size
		double c = 300;

		//scale the radius size by some value between 0.9 - 1.1
		double randScale = (((double)rand() / RAND_MAX) / 5.0) + 0.9;
		c *= randScale;

		//return new location, scaled by new radius size and translated to be relative to firework center instead of origin
		return Vertex3D((x * c) + center.x, (y * c) + center.y, (z * c) + center.z);
	}

	//return rgb color array
	float* getColor()
	{
		return this->rgb;
	}
};

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	//Generate random number of fireworks between 5 - 8
	int numFireworks = (rand() % 3) + 5;
	vector<Firework> fireworks;

	//generate random firework x,y,z coordinates
	for (int i = 0; i < numFireworks; i++) {
		double coinFlip = (double)rand() / RAND_MAX;
		int randx = coinFlip > (double)((int)coinFlip) + 0.5 ? -1 * (rand() % 800) : rand() % 800;
		coinFlip = (double)rand() / RAND_MAX;
		int randy = coinFlip > (double)((int)coinFlip) + 0.5 ? -1 * (rand() % 800) : rand() % 800;
		coinFlip = (double)rand() / RAND_MAX;
		int randz = coinFlip > (double)((int)coinFlip) + 0.5 ? -1 * (rand() % 800) : rand() % 800;
		Vertex3D vert(randx, randy, randz);
		Firework f(vert);
		fireworks.push_back(f);
	}

	//for every firework,
	for (int i = 0; i < numFireworks; i++) {
		Firework f = fireworks[i];
		Vertex3D center = f.center;
		float* rgb = f.getColor();
		glColor3f(rgb[0], rgb[1], rgb[2]);
		glBegin(GL_LINES);
		//for every particle vertex,
		for (int j = 0; j < f.particles.size(); j++)
		{
			//draw a line from the center point to particle
			Vertex3D v = f.particles[j];
			glVertex3f(center.x, center.y, center.z);
			glVertex3f(v.x, v.y, v.z);
		}
		glEnd();
	}

	glFlush();
}

void init() {
	srand(time(NULL));
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1000.0, 1000.0, -1000.0, 1000.0, -1000.0, 1000.0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Fireworks");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}