#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include "GLSpace.h"

//$BIA2h4X?t$N%*!<%P!<%i%$%I(B
void Display(cv::vector<cv::Point3d> 3d_points);

class 3dPoints{
	public:
	cv::vector<cv::Point3d> data;
};


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
	GLSpace::startGL();

	//$B%k!<%W(B
	for(int i=0;i<image_size;i++){
	
	//$B?^I=<((B
	Display(3d_points[i]);
	cv::waitKey(30);
	}

    	return 0;
}


void Display(cv::vector<cv::Point3d> 3d_points) {

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
		
	
		glColor3d(0.0,0.0,0.0);
	

		   glVertex3d(0.0,0.0,0.0);//$B86E@$N%N%$%:=|5n(B
		   for(int i=0;i<3d_points.size();i++){
		   glColor3d(1.0,1.0,0.0);
		   //hsv2rgbColor(z_global[i]);
		   hsv2rgbColor(3d_points[i].z);
		   //glVertex3d(x_global[i]/l_scale,y_global[i]/l_scale,z_global[i]/z_scale-z_slide);
		   glVertex3d(3d_points[i].x/l_scale,3d_points[i].y/l_scale,3d_points[i].z/z_scale-z_slide);
		   }
		 

		glEnd();
		Ground();
		glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)$B$G%@%V%k%P%C%U%!%j%s%0$rMxMQ2D(B

}



