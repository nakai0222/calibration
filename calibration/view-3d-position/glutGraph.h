#ifndef INC_GLUTGRAPH
#define INC_GLUTGRAPH

#define _USE_MATH_DEFINES

#include <vector>
#include <stdexcept>
#include <process.h>
#include <Windows.h>
#include <gl/glew.h>
#include <gl/glut.h>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glut32.lib")

#pragma warning(disable:4018)
#pragma warning(disable:4996)

#define VAL_LENGTH 16
#define GLUTGRAPH_2D 0
#define GLUTGRAPH_3D 1
#define GLUTGRAPH_2D3D 2

#define GLUTGRAPH_DISP_AXIS 1
#define GLUTGRAPH_DISP_GRID 2
#define GLUTGRAPH_DISP_FULL 4
#define GLUTGRAPH_DISP_SHDW 8

#define GLUTGRAPH_SPHERE 10

typedef struct Quaternion{
	double w;
	double x;
	double y;
	double z;
} Quaternion;

Quaternion & operator *(Quaternion &q1, Quaternion &q2){
	Quaternion q0 = {
		q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
		q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
		q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
	};
	q1 = q0;
	return q1;
}

typedef struct graphPoint3D{
	double x;
	double y;
	double z;
} graphPoint3D;

typedef struct graphProp2D{
	double col[3];
	double pos;
	double scl;
	std::vector<double> data;
} graphProp2D;

typedef struct graphProp3D{
	double col[3];
	int style;
	std::vector<graphPoint3D> data;
} graphProp3D;

class glutGraph{
private: 
	static int*  pargc;
	static char** pargv;
	static DWORD  th_2D, th_3D;
	static HANDLE hl_2D, hl_3D;
	static int WW_2D, WW_3D, ID_2D;
	static int WH_2D, WH_3D, ID_3D;
	static int dataSize_2D;
	static Quaternion qt_3D, qc_3D;
	static double r_3D[16];
	static double t_3D[3];
	static double s_3D;
	static std::vector<graphProp2D> prop_2D;
	static std::vector<graphProp3D> prop_3D;
	static bool runstop_2D, runstop_3D;
	static double tpos;
	static int mouseX, mouseY;
	static int mode;
	static int dispmode;
	static int zoom3D;
	static float psize3D;
public:
	glutGraph(int* argc, char* argv[], int width_2D, int height_2D, int size_2D);
	glutGraph(int* argc, char* argv[], int width_3D, int height_3D);
	glutGraph(int* argc, char* argv[], int width_2D, int height_2D, int size_2D, int width_3D, int height_3D);
	~glutGraph();
	static unsigned __stdcall glutThread2D(void* param);
	static unsigned __stdcall glutThread3D(void* param);
	static unsigned __stdcall glutThread2D3D(void* param);
	static void  glutIdle(void);
	static void  glutResize2D(int w, int h);
	static void  glutResize3D(int w, int h);
	static void  glutDisplay2D(void);
	static void  glutDisplay3D(void);
	static void  glutKey2D(unsigned char key, int x, int y);
	static void  glutKey3D(unsigned char key, int x, int y);
	static void  glutSKey2D(int key, int x, int y);
	static void  glutSKey3D(int key, int x, int y);
	static void  glutMouse2D(int x, int y);
	static void  glutMouse3D(int button, int state, int x, int y);
	static void  glutMotion3D(int x, int y);
	void pushGraph2D(double colR, double colG, double colB, double pos, double scl);
	void pushGraph3D(double colR, double colG, double colB, int style);
	void pushData2D(std::vector<double> data);
	void pushData3D(int index, std::vector<graphPoint3D> data);
	static void setColor(double colR, double colG, double colB);
	static void qtor(Quaternion q);
};

int*  glutGraph::pargc;
char** glutGraph::pargv;
DWORD glutGraph::th_2D;
DWORD glutGraph::th_3D;
HANDLE glutGraph::hl_2D;
HANDLE glutGraph::hl_3D;
int glutGraph::WW_2D;
int glutGraph::WH_2D;
int glutGraph::WW_3D;
int glutGraph::WH_3D;
int glutGraph::ID_2D;
int glutGraph::ID_3D;
int glutGraph::dataSize_2D;
Quaternion glutGraph::qt_3D;
Quaternion glutGraph::qc_3D;
double glutGraph::r_3D[16];
double glutGraph::t_3D[3];
double glutGraph::s_3D;
std::vector<graphProp2D> glutGraph::prop_2D;
std::vector<graphProp3D> glutGraph::prop_3D;
bool glutGraph::runstop_2D;
bool glutGraph::runstop_3D;
double glutGraph::tpos;
int glutGraph::mouseX;
int glutGraph::mouseY;
int glutGraph::mode;
int glutGraph::dispmode;
int glutGraph::zoom3D;
float glutGraph::psize3D;

glutGraph::glutGraph(int* argc, char* argv[], int width_2D, int height_2D, int size_2D){
	pargc = argc;
	pargv = argv;
	WW_2D = width_2D;
	WH_2D = height_2D;
	dataSize_2D = size_2D;
	runstop_2D = true;
	tpos = 1.0;
	mode = 0;

	hl_2D = (HANDLE)_beginthreadex(NULL, 0, glutGraph::glutThread2D, NULL, 0, (unsigned int*)&th_2D);
}

glutGraph::glutGraph(int* argc, char* argv[], int width_3D, int height_3D){
	pargc = argc;
	pargv = argv;
	WW_3D = width_3D;
	WH_3D = height_3D;
	runstop_3D = true;
	qc_3D = { 1.0, 0.0, 0.0, 0.0 };
	qtor(qt_3D);
	t_3D[0] = 0; t_3D[1] = 0; t_3D[2] = 0;
	s_3D = 1.0;
	mode = 1;
	dispmode = 3;
	zoom3D = 250;
	psize3D = 1;
	

	hl_3D = (HANDLE)_beginthreadex(NULL, 0, glutGraph::glutThread3D, NULL, 0, (unsigned int*)&th_3D);
}

glutGraph::glutGraph(int* argc, char* argv[], int width_2D, int height_2D, int size_2D, int width_3D, int height_3D){
	pargc = argc;
	pargv = argv;
	WW_2D = width_2D;
	WH_2D = height_2D;
	dataSize_2D = size_2D;
	runstop_2D = true;
	tpos = 1.0;

	WW_3D = width_3D;
	WH_3D = height_3D;
	runstop_3D = true;
	qc_3D = { 1.0, 0.0, 0.0, 0.0 };
	qtor(qt_3D);
	t_3D[0] = 0; t_3D[1] = 0; t_3D[2] = 0;
	s_3D = 1.0;
	mode = 2;
	dispmode = 3;
	zoom3D = 250;
	psize3D = 1;

	hl_3D = (HANDLE)_beginthreadex(NULL, 0, glutGraph::glutThread2D3D, NULL, 0, (unsigned int*)&th_3D);
}

glutGraph::~glutGraph(){
};

void glutGraph::glutIdle(void){
	if (mode % 2 == 0){
		glutSetWindow(ID_2D);
		glutPostRedisplay();
	}
	if (mode > 0){
		glutSetWindow(ID_3D);
		glutPostRedisplay();
	}
}

void glutGraph::glutResize2D(int w, int h){
	WW_2D = w;
	WH_2D = h;
	glutSetWindow(ID_2D);
	glutPostRedisplay();
}

void glutGraph::glutResize3D(int w, int h){
	WW_3D = w;
	WH_3D = h;
	glutSetWindow(ID_3D);
	glutPostRedisplay();
}

void glutGraph::glutDisplay2D(void){
	glClear(GL_COLOR_BUFFER_BIT);
	if(runstop_2D){
		glColor3d(1.0,1.0,1.0);
		glRasterPos2f(-0.95, 0.9);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'U');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'N');
	}else{
		glColor3d(0.5,0.5,0.5);
		glRasterPos2f(-0.95, 0.9);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'T');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'O');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'P');
	}

	for(int i=0; i<prop_2D.size(); i++){
		glColor3d(prop_2D[i].col[0]*0.5,prop_2D[i].col[1]*0.5,prop_2D[i].col[2]*0.5);
		glBegin(GL_LINES);
		glVertex2d(-1,prop_2D[i].pos);
		glVertex2d( 1,prop_2D[i].pos);
		glColor3d(0.5,0.5,0.5);
		glVertex2d(tpos, 1.0);
		glVertex2d(tpos,-1.0);
		glEnd();

		glColor3d(prop_2D[i].col[0],prop_2D[i].col[1],prop_2D[i].col[2]);
		glBegin(GL_LINE_STRIP);
		for(int j=0; j<dataSize_2D-1; j++){
			glVertex2d((j*2.0)/dataSize_2D-1.0, prop_2D[i].data[j]*prop_2D[i].scl + prop_2D[i].pos);
		}
		glEnd();
		
		glRasterPos2f(i*2.0/prop_2D.size()-1.0, -0.95);
		char string[VAL_LENGTH];
		sprintf(string,"%4.4f",prop_2D[i].data[max(min((int)((tpos+1.0)/2*(dataSize_2D-1)),dataSize_2D-2),0)]);
		for(int j=0; j<VAL_LENGTH; j++){
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[j]);
		}
	}
	glutSwapBuffers();
}

void glutGraph::glutDisplay3D(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (runstop_3D){
		glColor3d(1.0, 1.0, 1.0);
		glWindowPos2f(15, WH_3D-25);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'R');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'U');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'N');
	}
	else{
		glColor3d(0.5, 0.5, 0.5);
		glWindowPos2f(15, WH_3D-25);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'T');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'O');
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'P');
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)WW_3D / (double)WH_3D, 1.0, 1000.0);
	glTranslatef(t_3D[0], t_3D[1], t_3D[2]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, WW_3D, WH_3D);
	gluLookAt(   0.0,    0.0, zoom3D,
				 0.0,    0.0,    0.0,
				 0.0,    1.0,    0.0);
	

	glMultMatrixd(r_3D);

	if (dispmode & GLUTGRAPH_DISP_SHDW){
		GLfloat light[] = { 0.0, 0.0, 100.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light);
		setColor(1.0, 1.0, 1.0);
	}

	if (dispmode & GLUTGRAPH_DISP_GRID){
		glLineWidth(1);
		setColor(0.5, 0.5, 0.5);
		glBegin(GL_LINES);
		for (int x = -4; x <= 4; x++){
			glVertex3d(x * 10, -40, 0.0);
			glVertex3d(x * 10, 40, 0.0);
		}
		for (int y = -4; y <= 4; y++){
			glVertex3d(-40, y * 10, 0.0);
			glVertex3d(40, y * 10, 0.0);
		}
		glEnd();
	}
	if (dispmode & GLUTGRAPH_DISP_AXIS){
		glLineWidth(1);
		glBegin(GL_LINES);
		setColor(1.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(50.0, 0.0, 0.0);
		setColor(0.0, 1.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 50.0, 0.0);
		setColor(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 50.0);
		glEnd();
	}

	glScalef(s_3D, s_3D, s_3D);
	
	for (int i = 0; i<prop_3D.size(); i++){
		setColor(prop_3D[i].col[0], prop_3D[i].col[1], prop_3D[i].col[2]);
		if (prop_3D[i].style < 10){
			switch (prop_3D[i].style){
			case 0:
				glPointSize(psize3D);
				glBegin(GL_POINTS);
				break;
			case 1:
				glLineWidth(psize3D);
				glBegin(GL_LINES);
				break;
			case 2:
				glLineWidth(psize3D);
				glBegin(GL_LINE_LOOP);
				break;
			case 3:
				glLineWidth(psize3D);
				glBegin(GL_LINE_STRIP);
				break;
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				glBegin(prop_3D[i].style);
				break;
			default:
				glPointSize(psize3D);
				glBegin(GL_POINTS);
				break;
			}

			graphPoint3D tmp;
			for (int j = 0; j < prop_3D[i].data.size(); j++){
				try{
					tmp = prop_3D[i].data.at(j);
				}
				catch (const std::out_of_range& oor){
					continue;
				}
				glVertex3d(tmp.x, tmp.y, tmp.z);
			}
			glEnd();
		} else {
			graphPoint3D tmp;
			for (int j = 0; j < prop_3D[i].data.size(); j++){
				try{
					tmp = prop_3D[i].data.at(j);
				}
				catch (const std::out_of_range& oor){
					continue;
				}
				glTranslatef(tmp.x, tmp.y, tmp.z);
				glutSolidSphere(psize3D, 8, 8);
				glTranslatef(-tmp.x, -tmp.y, -tmp.z);
			}
		}
	}
	glutSwapBuffers();
}

void glutGraph::glutKey2D(unsigned char key, int x, int y){
	switch(key){
		case    'q':
		case    'Q':
		case '\033':
			runstop_2D = !runstop_2D;
			break;
		case    's':
		case    'S':
			for(int i=0; i<prop_2D.size(); i++){
				double sum=0,ave,vsum=0,var,sig;
				for(int j=0; j<dataSize_2D; j++){
					sum += prop_2D[i].data[j];
				}
				ave = sum/dataSize_2D;
				for(int j=0; j<dataSize_2D; j++){
					vsum += (prop_2D[i].data[j]-ave)*(prop_2D[i].data[j]-ave);
				}
				var = vsum/dataSize_2D;
				sig = sqrt(var);
				printf("Statistics of %d: average %4.4f, variance %4.4f, deviation %4.4f\n",i,ave,var,sig);
			}
			printf("\n");
			break;
		default:
			break;
	}
}

void glutGraph::glutKey3D(unsigned char key, int x, int y){
	switch (key){
	case    'q':
	case    'Q':
	case '\033':
		runstop_3D = !runstop_3D;
		break;
	case    'a':
	case    'A':
		dispmode = dispmode^GLUTGRAPH_DISP_AXIS;
		break;
	case    'g':
	case    'G':
		dispmode = dispmode^GLUTGRAPH_DISP_GRID;
		break;
	case    'f':
	case    'F':
		if (dispmode & GLUTGRAPH_DISP_FULL){
			dispmode = dispmode^GLUTGRAPH_DISP_FULL;
			glutSetWindow(ID_3D);
			glutReshapeWindow(512, 512);
			glutPositionWindow(WW_2D+30, 50);
		}
		else{
			dispmode = dispmode^GLUTGRAPH_DISP_FULL;
			glutSetWindow(ID_3D);
			glutFullScreen();
		}
		break;
	case    'l':
	case    'L':
		if (dispmode & GLUTGRAPH_DISP_SHDW){
			dispmode = dispmode^GLUTGRAPH_DISP_SHDW;
			glDisable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
		}
		else{
			dispmode = dispmode^GLUTGRAPH_DISP_SHDW;
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
		}
		break;
	case    'r':
	case    'R':
		qt_3D = { 1.0, 0.0, 0.0, 0.0 };
		qc_3D = qt_3D;
		qtor(qt_3D);
		break;
	case    't':
	case    'T':
		t_3D[0] = 0; t_3D[1] = 0; t_3D[2] = 0;
		break;
	case    's':
		if (s_3D > 0.1) s_3D -= 0.1;
		break;
	case    'S':
		s_3D += 0.1;
		break;
	case    'z':
	case    'Z':
		zoom3D = 250;
		break;
	case    'p':
		if (psize3D>1)psize3D--;
		break;
	case    'P':
		psize3D++;
		break;
	default:
		break;
	}
}

void glutGraph::glutSKey2D(int key, int x, int y){
	switch(key){
		case GLUT_KEY_RIGHT:
			tpos = min(tpos+2.0/dataSize_2D, 1.0);break;
		case GLUT_KEY_LEFT:
			tpos = max(tpos-2.0/dataSize_2D,-1.0);break;
		default:
			break;
	}
}

void glutGraph::glutSKey3D(int key, int x, int y){
	switch (key){
	case GLUT_KEY_PAGE_UP:
		zoom3D -= zoom3D / 2;
		break;
	case GLUT_KEY_PAGE_DOWN:
		zoom3D += zoom3D;
		break;
	case GLUT_KEY_RIGHT:
		t_3D[0] += 5;
		break;
	case GLUT_KEY_LEFT:
		t_3D[0] -= 5;
		break;
	case GLUT_KEY_UP:
		t_3D[1] += 5;
		break;
	case GLUT_KEY_DOWN:
		t_3D[1] -= 5;
		break;
	default:
		break;
	}
}

void glutGraph::glutMouse2D(int x, int y){
	tpos = x*1.0/WH_2D-1.0;
}

void glutGraph::glutMouse3D(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		switch (state){
		case GLUT_DOWN:
			mouseX = x;
			mouseY = y;
			break;
		case GLUT_UP:
			qc_3D = qt_3D;
			break;
		default:
			break;
		}
	}
}

void glutGraph::glutMotion3D(int x, int y){
	double dx = (x - mouseX) * 1.0 / WW_3D;
	double dy = (y - mouseY) * 1.0 / WH_3D;
	double length = sqrt(dx * dx + dy * dy);

	if (length != 0.0) {
		Quaternion qx = { cos(dx), 0.0, sin(dx), 0.0 };
		Quaternion qy = { cos(dy), sin(dy), 0.0, 0.0 };
		qt_3D = qx*qy * qc_3D;
		qtor(qt_3D);
	}
}

void glutGraph::pushGraph2D(double colR, double colG, double colB, double pos, double scl){
	std::vector<double> tmpData;
	for(int i=0; i<dataSize_2D; i++){
		tmpData.push_back(0.0);
	}
	graphProp2D tmpProp = {{colR,colG,colB},pos,scl,tmpData};
	prop_2D.push_back(tmpProp);
}

void glutGraph::pushData2D(std::vector<double> data){
	if(runstop_2D){
		for(int i=0; i<prop_2D.size(); i++){
			prop_2D[i].data.erase(prop_2D[i].data.begin());
			prop_2D[i].data.push_back(data[i]);
		}
	}
}

void glutGraph::pushGraph3D(double colR, double colG, double colB, int style){
	std::vector<graphPoint3D> tmpData;
	graphProp3D tmpProp = { { colR, colG, colB }, style, tmpData };
	prop_3D.push_back(tmpProp);
}

void glutGraph::pushData3D(int index, std::vector<graphPoint3D> data){
	if (runstop_3D){
		prop_3D[index].data.reserve(data.size());
		prop_3D[index].data = data;
	}
}

unsigned __stdcall glutGraph::glutThread2D(void* param){
	glutInit(pargc, pargv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WW_2D, WH_2D);
	glutGraph::ID_2D = glutCreateWindow("glutGraph2D");
	glutDisplayFunc(glutGraph::glutDisplay2D);
	glutIdleFunc(glutGraph::glutIdle);
	glutReshapeFunc(glutGraph::glutResize2D);
	glutKeyboardFunc(glutGraph::glutKey2D);
	glutSpecialFunc(glutGraph::glutSKey2D);
	glutMotionFunc(glutGraph::glutMouse2D);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glewInit();
	glutMainLoop();

	return NULL;
}

unsigned __stdcall glutGraph::glutThread3D(void* param){
	glutInit(pargc, pargv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WW_3D, WH_3D);
	glutGraph::ID_3D = glutCreateWindow("glutGraph3D");
	glutDisplayFunc(glutGraph::glutDisplay3D);
	glutIdleFunc(glutGraph::glutIdle);
	glutReshapeFunc(glutGraph::glutResize3D);
	glutKeyboardFunc(glutGraph::glutKey3D);
	glutSpecialFunc(glutGraph::glutSKey3D);
	glutMouseFunc(glutGraph::glutMouse3D);
	glutMotionFunc(glutGraph::glutMotion3D);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glewInit();
	glutMainLoop();

	return NULL;
}

unsigned __stdcall glutGraph::glutThread2D3D(void* param){
	glutInit(pargc, pargv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(WW_2D, WH_2D);
	glutInitWindowPosition(0, 50);
	glutGraph::ID_2D = glutCreateWindow("glutGraph2D");
	glutDisplayFunc(glutGraph::glutDisplay2D);
	glutIdleFunc(glutGraph::glutIdle);
	glutReshapeFunc(glutGraph::glutResize2D);
	glutKeyboardFunc(glutGraph::glutKey2D);
	glutSpecialFunc(glutGraph::glutSKey2D);
	glutMotionFunc(glutGraph::glutMouse2D);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WW_3D, WH_3D);
	glutInitWindowPosition(WW_2D+30, 50);
	glutGraph::ID_3D = glutCreateWindow("glutGraph3D");
	glutDisplayFunc(glutGraph::glutDisplay3D);
	glutIdleFunc(glutGraph::glutIdle);
	glutReshapeFunc(glutGraph::glutResize3D);
	glutKeyboardFunc(glutGraph::glutKey3D);
	glutSpecialFunc(glutGraph::glutSKey3D);
	glutMouseFunc(glutGraph::glutMouse3D);
	glutMotionFunc(glutGraph::glutMotion3D);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glewInit();
	glutMainLoop();

	return NULL;
}

void glutGraph::qtor(Quaternion q){
	double xx = q.x * q.x * 2.0;
	double yy = q.y * q.y * 2.0;
	double zz = q.z * q.z * 2.0;
	double xy = q.x * q.y * 2.0;
	double yz = q.y * q.z * 2.0;
	double zx = q.z * q.x * 2.0;
	double xw = q.x * q.w * 2.0;
	double yw = q.y * q.w * 2.0;
	double zw = q.z * q.w * 2.0;
	double r1[16] = { 1.0 - yy - zz, xy + zw, zx - yw, 0.0,
		xy - zw, 1.0 - zz - xx, yz + xw, 0.0,
		zx + yw, yz - xw, 1.0 - xx - yy, 0.0,
		0.0, 0.0, 0.0, 1.0 };
	for (int i = 0; i < 16; i++) {
		r_3D[i] = r1[i];
	}
}

void glutGraph::setColor(double colR, double colG, double colB){
	glColor3d(colR, colG, colB);
	if (dispmode & GLUTGRAPH_DISP_SHDW){
		GLfloat material[] = { colR, colG, colB, 1.0 };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material);
	}
}
#endif
