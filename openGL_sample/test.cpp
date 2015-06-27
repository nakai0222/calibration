
/*
   6/6/2015

   view 3D point at openGL world

 */
#include <math.h>
#include "opengl.h"

void dataDisplay(double data){

	std::cout << "data : " << data << std::endl;	

	draw_cube();
	
}



int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowPosition(100, 100);

	glutInitWindowSize(640, 480);

	glutCreateWindow("Lighting Cube");
	glutReshapeFunc(reshape);

	glutDisplayFunc(display);

	glutMainLoop();
	
	return 0;

}



