#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include "glut.h"
#endif
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "tinyxml2.h"
#include <string>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"


using namespace std;
using namespace tinyxml2;

#define TRANSLATE 10284
#define ROTATE 7301
#define SCALE 5401
#define MODELS 7257 
#define GROUP 5793
/**
 Fun��o auxiliar para calcular o valor da opera�ao a usar
 */
int hashF(char* s) {
	int r = 0;
	for (int i = 0; s[i]; i++) {
		r += (s[i] * (10 ^ i));
	}
	return r;
}

/**
Classe que guarda as coordenadas X, Y, Z de um ponto
*/
class Point {
	float x, y, z;
public:

	void set_values(double a, double b, double c) {
		x = a;
		y = b;
		z = c;
	}
	double getX() {
		return x;
	}
	double getY() {
		return y;
	}
	double getZ() {
		return z;
	}
};

/**
Classe que guarda os tr�s pontos de um tri�ngulo
*/
class Triangle {
	Point one, two, three;

public:

	void set_values(Point x, Point y, Point z) {
		one = x;
		two = y;
		three = z;
	}
	Point getOne() {
		return one;
	}
	Point getTwo() {
		return two;
	}
	Point getThree() {
		return three;
	}
};


/**
Classe que guarda a lista de tri�ngulos que comp�em uma figura
*/
class Figure {
	vector<Triangle> triangles;
public:

	void set_values(vector<Triangle> ts) {
		triangles = ts;
	}

	vector<Triangle> get_triangles() {
		return triangles;
	}
};

/* Classe  operation � a superclasse das opera�oes, server para construir um vector com tranlates e rotates,
neste momento possui todo o codigo de ambas as subclasses porque nao estou a conseguir dar cast direito*/
class Operation {
	string op;
	double angle, x, y, z;
public:
	void set_op(string s) {
		op = s;
	}

	string get_op() {
		return op;
	}

	void set_values(double xx, double yy, double zz) {
		x = xx;
		y = yy;
		z = zz;
	}

	void sumTranslate(double xx, double yy, double zz) {
		x += xx;
		y += yy;
		z += zz;
	}

	double getX() {
		return x;
	}

	double getY() {
		return y;
	}

	double getZ() {
		return z;
	}
	void set_values(double a, double xx, double yy, double zz) {
		angle = a;
		x = xx;
		y = yy;
		z = zz;
	}

	void setRotate(double a, double xx, double yy, double zz) {
		angle = a;
		x = xx;
		y = yy;
		z = zz;
	}

	void sumRotate(double a, double xx, double yy, double zz) {
		angle += a;
		x += xx;
		y += yy;
		z += zz;
	}

	double getAngle() {
		return angle;
	}
};

class Translate : public Operation {
	

public:

	
};

class Rotate : public Operation {
	
};

class Group {
	vector<Figure> figures;
	vector<Operation> operations;
	vector<Group> subGroups;

public:
	void set_values(vector<Figure> fig, vector<Operation> ops, vector<Group> g) {
		figures = fig;
		operations = ops;
		subGroups = g;
	}

	vector<Figure> getFigures() {
		return figures;
	}

	vector<Operation> getOperations() {
		return operations;
	}

	vector<Group> getSubGroups() {
		return subGroups;
	}

};


/**
Vari�vel global com a lista de figuras a desenhar
*/
vector<Group> groups;

/**
Fun��o que, partindo de um ficheiro gerado pelo programa 'generator', devolve a lista dos pontos existentes nesse ficheiro.
*/
vector<Point> getPoints(const char *name) {
	string point;
	ifstream file;
	file.open(name);

	string delimiter = ",";
	int delim_len = delimiter.length();
	vector<Point> points;
	

	while (!file.eof()) { // ler ficheiro completo

		getline(file, point); // ler uma linha
        if(!point.compare("")) break; // ultima linha do ficheiro � vazia, n�o deve ser processada
		
        string token;
		float coord[3];
		int i = 0;

		int pos_start = 0, pos_end;

		while (i<3) {
			pos_end = point.find(delimiter, pos_start);
            token = point.substr(pos_start, pos_end-pos_start);
			coord[i++] = stof(token);
			pos_start = pos_end + delim_len;
		}

		Point p;
		p.set_values(coord[0], coord[1], coord[2]);
		points.push_back(p);//adicona novo elemento no fim do vector
		
	}
	file.close();

	return points;
}

/**
Fun��o que constr�i uma lista de tri�ngulos consoante a lista de pontos que recebe como par�metro
*/
vector<Triangle> getTriangles(vector<Point> points) {	
	vector<Triangle> triangles;
	Point pts[3];
	int i = 0;

	for (vector<Point>::const_iterator it = points.begin(); it != points.end(); ++it) {

		if (i == 3) i = 0;
		
		pts[i] = (*it);

		if (i == 2) { // ver se � o 3o ponto e formar triangulo
			Triangle t;
			t.set_values(pts[0], pts[1], pts[2]);
			triangles.push_back(t);
		}

		i++;
	}

	return triangles;
}

/**
Fun��o que desenha um figura recebida como par�metro
*/
void drawModel(Figure f) {
    int color=0;
	vector<Triangle> triangles = f.get_triangles();
    
	glBegin(GL_TRIANGLES);
	for (vector<Triangle>::iterator it = triangles.begin(); it != triangles.end(); ++it) {
		Triangle t = *it;
        
        if(color==0)
            glColor3f(0.49,0.51,0.53);
        else
            glColor3f(0.2,0.2,0.2);
        
		glVertex3d(t.getOne().getX(), t.getOne().getY(), t.getOne().getZ());
		glVertex3d(t.getTwo().getX(), t.getTwo().getY(), t.getTwo().getZ());
		glVertex3d(t.getThree().getX(), t.getThree().getY(), t.getThree().getZ());
        
        
        color = abs(color-1);
	}
	glEnd();
}

int readModels(XMLElement * models, vector<Figure>* fig) {
	if (models != nullptr) {
		XMLElement *model = models->FirstChildElement("model");
		while (model != nullptr) {
			const char * fileName = nullptr;
			fileName = model->Attribute("file");

			if (fileName == nullptr) return XML_ERROR_PARSING_ATTRIBUTE;

			Figure f;
			f.set_values(getTriangles(getPoints(fileName)));
			(*fig).push_back(f);

			model = model->NextSiblingElement("model");
		}

		models = models->NextSiblingElement("models");
		return XML_SUCCESS;
	}
	return XML_NO_TEXT_NODE;
}

int readGroup(XMLElement* element, vector<Figure> *fig, vector<Operation>*ops, vector<Group> *subGroups) {
	XMLElement* child;
	int flag = 0;
	for (child = element->FirstChildElement(); child != NULL && flag == 0; child = child->NextSiblingElement())
	{
		Translate t; t.set_values(0, 0, 0);
		Rotate r; r.set_values(0, 0, 0, 0);
		Operation o;
		vector<Figure> aux;
		vector<Operation> aux2;
		vector<Group> aux3;
		Group g;
		double x = 0, y = 0, z = 0;
		const char* n;
		double angle = 0;
		switch (hashF((char*)child->Value()))
		{
		case TRANSLATE:

			if ((n = child->Attribute("X")) != nullptr) x = atof(n);
			if ((n = child->Attribute("Y")) != nullptr) y = atof(n);
			if ((n = child->Attribute("Z")) != nullptr) z = atof(n);

			t.set_values(x, y, z);
			t.set_op("translate");
			(*ops).push_back(t);
			printf("tamanho do ops: %d\n", (*ops).size());
			break;
		case ROTATE:

			if ((n = child->Attribute("angle")) != nullptr) angle = atof(n);
			if ((n = child->Attribute("axisX")) != nullptr) x = atof(n);
			if ((n = child->Attribute("axisY")) != nullptr) y = atof(n);
			if ((n = child->Attribute("axisZ")) != nullptr) z = atof(n);

			r.setRotate(angle, x, y, z);
			r.set_op("rotate");
			(*ops).push_back(r);
			printf("tamanho do ops: %d\n", (*ops).size());
			break;
		case MODELS:
			readModels(child, fig);
			break;
		case GROUP:
			readGroup(child, &aux, &aux2, &aux3);
			g.set_values(aux,aux2,aux3);
			(*subGroups).push_back(g);
		default:
			break;
		}
	}
	printf("tamanho do ops: %d\n", (*ops).size());
	return XML_SUCCESS;
}



/**
Fun��o que interpreta um cen�rio gr�fico em XML
*/
int readXML(const char *filename) {

	XMLDocument doc;
	XMLError error = doc.LoadFile(filename);
	if (error != XML_SUCCESS) { printf("Error: %i\n", error); return error; }

	XMLNode *scene = doc.FirstChild();
	if (scene == nullptr) return XML_ERROR_FILE_READ_ERROR;

	XMLElement *groups_xml = scene->FirstChildElement("group");

	while (groups_xml != nullptr) {
		Translate t; t.set_values(0, 0, 0);
		Rotate r; r.set_values(0, 0, 0, 0);
		vector<Operation> ops;
		vector<Figure> fig;
		vector<Group> subGroups;
		readGroup(groups_xml, &fig, &ops, &subGroups);
		
		
		Group group; group.set_values(fig, ops,subGroups);
		printf("tamanho do ops: %d\n",ops.size());
		groups.push_back(group);

		groups_xml = groups_xml->NextSiblingElement("group");
	}

	return XML_SUCCESS;
}



// GLUT ------------------------------------------------------------------------------------
void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0) h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawCoordinates() {

	glBegin(GL_LINES);
		// x
		glColor3f(1.0, 0.0, 0.0); // red x	
		glVertex3f(-40.0, 0.0f, 0.0f);
		glVertex3f(40.0, 0.0f, 0.0f);


		// y 
		glColor3f(0.0, 1.0, 0.0); // green y
		glBegin(GL_LINES);
		glVertex3f(0.0, -40.0f, 0.0f);
		glVertex3f(0.0, 40.0f, 0.0f);


		// z 
		glColor3f(0.0, 0.0, 1.0); // blue z
		glVertex3f(0.0, 0.0f, -40.0f);
		glVertex3f(0.0, 0.0f, 40.0f);

	glEnd();
}

float alpha = M_PI/3.4;
float beta = M_PI/6;
float radius = 10;

void renderGroup(vector<Figure> figs, vector<Operation> ops, vector<Group> subGroups) {
	printf("renderizar grupo\n");
	printf("%d\n", ops.size());
	glPushMatrix();

	for (vector<Operation>::iterator it = ops.begin(); it != ops.end(); ++it) {
		Operation o = *it;
		if (o.get_op().compare("rotate") == 0) {
			Rotate r = static_cast<Rotate&>(o);
			printf("fez rotate (%f,%f,%f,%f) - %s\n", r.getAngle(), r.getX(), r.getY(), r.getZ(), o.get_op().c_str());
			glRotatef(r.getAngle(), r.getX(), r.getY(), r.getZ());

		}
		if (o.get_op().compare("translate") == 0) {
			Translate t = reinterpret_cast<Translate&>(o);
			printf("fez translate (%f,%f,%f) - %s\n", t.getX(), t.getY(), t.getZ(), o.get_op().c_str());
			glTranslatef(t.getX(), t.getY(), t.getZ());

		}
	}

	for (vector<Figure>::iterator it = figs.begin(); it != figs.end(); ++it) {
		Figure f = *it;
		drawModel(f);
	}

	for (vector<Group>::iterator it = subGroups.begin(); it != subGroups.end(); ++it) {
		Group g = *it;
		vector<Operation> aux1 = g.getOperations();
		vector<Figure> aux = g.getFigures();
		vector<Group> aux2 = g.getSubGroups();
		renderGroup(aux, aux1, aux2);
	}
	glPopMatrix();
}

void renderScene(void) {

	glClearColor(20.0f, 20.0f, 20.0f, 1);
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
    gluLookAt(radius*cos(beta)*cos(alpha), radius*sin(beta), radius*cos(beta)*sin(alpha),
              0.0, 0.0, 0.0,
              0.0f, 1.0f, 0.0f);

	glColor3b(0, 5, 20);
	


	for (vector<Group>::iterator it = groups.begin(); it != groups.end(); ++it) {
		Group g = *it;
		vector<Operation> ops = g.getOperations();
		vector<Figure> figs = g.getFigures();
		vector<Group> subGroups = g.getSubGroups();
		renderGroup(figs, ops, subGroups);
	}


	drawCoordinates();

	// End of frame
	glutSwapBuffers();
}


void processCamera(unsigned char key, int x, int y) {

	switch (key) {
	case 'A' | 'a':
		alpha += 0.1;
		break;
	case 'D' | 'd':
		alpha -= 0.1;
		break;
	case 'W' | 'w':
		if (beta < 1.5) beta += 0.1;
		break;
	case 'S' | 's':
		if (beta > -1.5) beta -= 0.1;
		break;
	case 'Q' | 'q':
		radius += 0.1;
		break;
	case 'E' | 'e':
		radius -= 0.1;
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char **argv) {

	if (argc == 1) {
		printf("Por favor insira todos os par�metros necess�rios. \n");
		return -1;
	}

	if (readXML(argv[1]) == XML_SUCCESS) {

		// init GLUT and the window
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(800, 800);
		glutCreateWindow("solar-system");

		// Required callback registry 
		glutDisplayFunc(renderScene);
		glutReshapeFunc(changeSize);

		// Callback registration for keyboard processing
		glutKeyboardFunc(processCamera);

		//  OpenGL settings
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		// enter GLUT's main cycle
		glutMainLoop();

		return 1;
	}

	else return 0;
}


#pragma GCC diagnostic pop