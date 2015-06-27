#include "GLSpace.h"



int main(int argc, char *argv[])
{


	//$BE@72%G!<%?FI$_9~$_!'%Y%/%H%k(B
	cv::FileStorage fs_3d_data("3d-data.xml",CV_STORAGE_READ);


	static int imageSize;
	fs_3d_data["image_size"] >>image_size; 
	3dPoints 3d_points[image_size];


	for(int i=0;i<image_size;i++){
		fs_3d_data["data"] >>3d_points; 
		std::cout << "data: " << 3d_points[0].data[0].x<< std::endl;
	}	

	//$BIA2h4X?t=i4|2=(B
	GLSpace::startGL(0,&argc,argv);

	//$B%k!<%W(B
	for(int i=0;i<image_size;i++){

		//$B2hA|JQ99(B
		3d_points_data.pull(3d_points[i]);	
		//$B?^I=<((B
		glutPostRedisplay(); //glutDisplayFunc()$B$r#12s<B9T$9$k(B
		cv::waitKey(0);
	
	}

	return 0;
}






//$B$3$l0J2<$r(BGLSpace.h$B$KB-$9(B
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>
//GLSpace(namespace)$B$NCf$X(B

class 3dPointsDraw{

	public:
	cv::vector<cv::Point3d> 3d_points_data;
	void pull(cv::vector<cv::Point3d> 3d_points);	
	void drawPoints();
	

}3d_points_draw;


void 3dPointsDraw::pull(cv::vector<cv::Point3d> 3d_points){

	for(int i=0;i<3d_points.size();i++)
	this->3d_points_data.push_back(3d_points[i]);

	this->3d_points_data = 3d_points;

}	



//$B#3<!85E@72IA2h4X?t(B
void 3dPointsDraw::drawPoints() {

	glPointSize(PLOTSIZE);
	glBegin(GL_POINTS);


	glColor3d(0.0,0.0,0.0);


	glVertex3d(0.0,0.0,0.0);//$B86E@$N%N%$%:=|5n(B
	for(int i=0;i<3d_points.size();i++){
		glColor3d(1.0,1.0,0.0);
		//hsv2rgbColor(z_global[i]);
		hsv2rgbColor(this->3d_points_data[i].z);
		//glVertex3d(x_global[i]/l_scale,y_global[i]/l_scale,z_global[i]/z_scale-z_slide);
		glVertex3d(this->3d_points_data[i].x/l_scale,this->3d_points_data[i].y/l_scale,this->3d_points_data[i].z/z_scale-z_slide);
	}


	glEnd();
}




//Display$BCV$-49$((B
void Display(void) {

	glClearColor(0.0,0.0,0.0,0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //$B%P%C%U%!$N>C5n(B
	//$B%b%G%k%S%e!<JQ499TNs$N@_Dj(B--------------------------
	glMatrixMode(GL_MODELVIEW);//$B9TNs%b!<%I$N@_Dj!J(BGL_PROJECTION : $BF);kJQ499TNs$N@_Dj!"(BGL_MODELVIEW$B!'%b%G%k%S%e!<JQ499TNs!K(B
	glLoadIdentity();//$B9TNs$N=i4|2=(B
	glViewport(0, 0, WindowWidth, WindowHeight);
	//----------------------------------------------
	glPushMatrix();
	//$B;kE@$N@_Dj(B------------------------------
	gluLookAt(
			-100.0, 80.0, -100.0, // $B;kE@$N0LCV(Bx,y,z;
			0.0, 0.0,  camera_z_pos,   // $B;k3&$NCf?40LCV$N;2>HE@:BI8(Bx,y,z
			0.0, 0.0, -1.0);  //$B;k3&$N>eJ}8~$N%Y%/%H%k(Bx,y,z
	gluLookAt(
			0.0, dist, 0.0, // $B;kE@$N0LCV(Bx,y,z;
			0.0, 0.0,  camera_z_pos,   // $B;k3&$NCf?40LCV$N;2>HE@:BI8(Bx,y,z
			0.0, 0.0, -1.0);  //$B;k3&$N>eJ}8~$N%Y%/%H%k(Bx,y,z
	//----------------------------------------
	////$B2sE>(B///////////////////////////////////////////////
	glMultMatrixd(rt);
	///////////////////////////////////////////////////////
	glPointSize(plot);
	glBegin(GL_POINTS);


	3d_points_draw::drawPoints(); 

	
	Ground();
	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)$B$G%@%V%k%P%C%U%!%j%s%0$rMxMQ2D(B

}
