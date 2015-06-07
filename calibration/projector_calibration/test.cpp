/*
6/6/2015
line lazer calibration
*/

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>


#define IMAGE_SIZE 2
#define CHESS_SIZE_Y 10
#define CHESS_SIZE_X 10
#define CHESS_ROW 7
#define CHESS_COLUM 7

int main( int argc, char* argv[])
{

	//read the raw and lazer image 
	cimg = imread(img_name);
	cimgl = imread(img_namel);
	

	/*read inside and outside parameter at camera*/
	cv::Mat I_Mat = imread(); 
	cv::Mat D_Mat = imread(); 
	cv::Mat R;
	cv::Mat T;		
	
	cv::Mat gimgl(XI_H, XI_W, CV_8UC1);
	/*detect lazer line*/
	//split color image
	cv::split(cimgl,split_imgl);
	cv::threshold(split_imgl[BGR],gimgl,thr,0,THRESH_TOZERO);
	
	/*match lazer line and camera local position*/

	cv::solvePnP(objectv,cornerv,I_mat,D_mat,R,T);

	/*guess projector date*/
	//
	
	return 0;
}


