
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#include <iostream> 


template <typename T>


void init(){

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

}





static double s_angle = 130;
static double s_posX = 1.4;
static double s_posY = -0.2;
static double s_posZ = 1.4;



void specialKeyboard(int key, int x, int y) {
	double radian = -s_angle * M_PI / 180.0; // 蜿ｳ謇狗ｳｻ
	switch (key) {
		case 101: // up
			{
				s_posX += sin(radian) * 0.1;
				s_posZ += cos(radian) * 0.1;
				break;
			}
		case 103: // down
			{
				s_posX -= sin(radian) * 0.1;
				s_posZ -= cos(radian) * 0.1;
				break;
			}
		case 100: // left
			s_angle -= 2;
			break;
		case 102: // right
			s_angle += 2;
			break;
		case 'u':
			s_posY += 0.1;
			break;
		case 'i':
			s_posY -= 0.1;
			break;	
	}
	s_angle = int(s_angle) % 360;

	glutPostRedisplay();
}


void draw_cube(){




	static GLfloat vert[][4]={
		{ 1.0, 1.0, 1.0},
		{ 0.0, 1.0, 1.0},
		{ 0.0, 0.0, 1.0},
		{ 1.0, 0.0, 1.0},
		{ 1.0, 1.0, 0.0},
		{ 0.0, 1.0, 0.0},
		{ 0.0, 0.0, 0.0},
		{ 1.0, 0.0, 0.0},
	};

	glColor3d(1.0, 0.0, 1.0);
	//glBindTexture(GL_TEXTURE_2D , textureID1);
	glBegin(GL_QUADS);
	{
		glTexCoord2d(1, 0);
		glVertex3fv(vert[0]);
		glTexCoord2d(0, 0);
		glVertex3fv(vert[1]);
		glTexCoord2d(0, 1);
		glVertex3fv(vert[2]);
		glTexCoord2d(1, 1);
		glVertex3fv(vert[3]);

	}
	glEnd();

	//glBindTexture(GL_TEXTURE_2D , textureID2);
	glBegin(GL_QUADS);
	{
		glTexCoord2d(1, 0);
		glVertex3fv(vert[4]);
		glTexCoord2d(0, 0);
		glVertex3fv(vert[5]);
		glTexCoord2d(0, 1);
		glVertex3fv(vert[6]);
		glTexCoord2d(1, 1);
		glVertex3fv(vert[7]);		

		glVertex3fv(vert[0]);
		glVertex3fv(vert[1]);		
		glVertex3fv(vert[5]);		
		glVertex3fv(vert[4]);

		glVertex3fv(vert[2]);		
		glVertex3fv(vert[3]);
		glVertex3fv(vert[7]);
		glVertex3fv(vert[6]);

		glVertex3fv(vert[3]);		
		glVertex3fv(vert[0]);		
		glVertex3fv(vert[4]);
		glVertex3fv(vert[7]);

		glVertex3fv(vert[1]);		
		glVertex3fv(vert[2]);		
		glVertex3fv(vert[6]);		
		glVertex3fv(vert[5]);
	}
	glEnd();
}


void draw_axes() {
	// draw axes
	glBegin(GL_LINES);
	{
		// x-axis
		glColor3d(1.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(10.0, 0.0, 0.0);

		// y-axis
		glColor3d(0.0, 1.0, 0.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 10.0, 0.0);

		// z-axis
		glColor3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(0.0, 0.0, 10.0);

		glEnd();	
	}
}




void draw_field(){

	double size = 5.0;
	glColor3d(0.5, 0.5, 0.5);
	double panelNum = 10;
	double len = size / panelNum;
	for (int i = 0; i < panelNum; i++) {
		for (int j = 0; j < panelNum; j++) {
			double x = len * i - size / 2.0;
			double z = len * j - size / 2.0;
			int mode = (i + j) % 2 == 0 ? GL_LINES : GL_POLYGON;
			glBegin(mode);
			glVertex3d(x, 0, z);
			glVertex3d(x + len, 0, z);
			glVertex3d(x + len, 0, z + len);
			glVertex3d(x, 0, z + len);
			glEnd();
		}
	}

}

void dataDisplay(T data);
void dataDisplay(double data);




void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, double(w)/double(h), 0.1, 100.0);
	//glMatrixMode(GL_MODELVIEW);
}



void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotated(s_angle,0,1,0);
	glTranslated(s_posX,s_posY,s_posZ);
	//glTranslated(s_posX,-0.1,s_posZ);

	draw_axes();


	dataDisplay(1.0);


	draw_field();
	glFlush();
}


