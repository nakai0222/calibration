
#ifndef DEBUG
#pragma comment(lib, "opencv_core249d.lib")
#pragma comment(lib, "opencv_highgui249d.lib")
#pragma comment(lib, "opencv_imgproc249d.lib")
#pragma comment(lib, "opencv_calib3d249d.lib")


#else
#pragma comment(lib, "opencv_core249.lib")
#pragma comment(lib, "opencv_highgui249.lib")
#pragma comment(lib, "opencv_imgproc249.lib")
#pragma comment(lib, "opencv_calib3d249.lib")
#endif


#include <ximea/xiApi.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#define HandleResult(res,place) if (res!=XI_OK) {printf("Error after %s (%d)",place,res);}


#define LAZER_POINTS 10
#define CHESS_SIZE 21
#define CHESS_ROW 9
#define CHESS_COLUM 6

#define XI_W 648
#define XI_H 488

#define PIXEL_INTERVAL 2

class Plane{
	public:
		double a;
		double b;
		double c;
		double d;

};

cv::vector<cv::Point2d> DetectBrightLine(cv::Mat image);

cv::vector<cv::Point3d> CalcDistInf(cv::Mat lazer_image,Plane plane,cv::Mat I_Mat,cv::Mat D_Mat);



int main( int argc, char* argv[])
{

	// Sample for XIMEA API V2.10
	DWORD dwNumberOfDevices = 0;
	HANDLE xiH = NULL;
	XI_RETURN stat = XI_OK;



	// image buffer
	XI_IMG image;
	memset(&image,0,sizeof(image));
	image.size = sizeof(XI_IMG);

	// Retrieving number of connected cameras
	stat = xiGetNumberDevices(&dwNumberOfDevices);
	if(!dwNumberOfDevices) { std::cout << "error!! camera was not connected." << std::endl; }

	// Retrieving a handle to the camera device 
	stat = xiOpenDevice(0, &xiH);
	HandleResult(stat,"xiOpenDevice");


	// Setting "exposure" parameter (10ms)

	int time_us = 1000;
	stat = xiSetParam(xiH, XI_PRM_EXPOSURE, &time_us, sizeof(time_us), xiTypeInteger);

	// Start acquisition
	stat = xiStartAcquisition(xiH);



	//load the raw and lazer image 
	cv::Mat lazer_image;

	/*load inside and outside parameter at camera*/
	cv::Mat I_Mat ; 
	cv::Mat D_Mat ;
	cv::FileStorage fs("camera.xml",CV_STORAGE_READ);

	fs["intrinsicMat"] >> I_Mat;
	fs["distCoeffs"] >> D_Mat;

	/*load projector parameter */
	Plane plane;

	cv::FileStorage fs_projector("projector.xml",CV_STORAGE_READ);

	fs_projector["plane_a"] >>plane.a; 
	fs_projector["plane_b"] >>plane.b; 
	fs_projector["plane_c"] >>plane.c; 
	fs_projector["plane_d"] >>plane.d; 
	std::cout << "plane.a : " << plane.a<< std::endl;
	std::cout << "plane.b : " << plane.b<< std::endl;
	std::cout << "plane.c : " << plane.c<< std::endl;
	std::cout << "plane.d : " << plane.d<< std::endl;

	while(1){

		// getting image from the camera
		cv::Mat lazer_image = cv::Mat::zeros(XI_H, XI_W, CV_8UC1);

		stat = xiGetImage(xiH, time_us , &image);


		memcpy(lazer_image.data, image.bp, image.width*image.height);

		cv::imshow("frame", lazer_image);

		//calculate
		cv::vector<cv::Point3f> location_inf = CalcDistInf(lazer_image,plane,I_Mat,D_Mat);


		std::cout << "location : " << location_inf << std::endl;
		cv::waitKey(1);

	}


	return 0;
}



cv::vector<cv::Point3f> CalcDistInf(cv::Mat lazer_image,Plane plane,cv::Mat I_Mat,cv::Mat D_Mat){



	//calculate lazer points
	cv::vector<cv::Point2i> lazer_points(LAZER_POINTS);


	cv::Mat gimg(lazer_image.size(),lazer_image.type()) ;
	gimg = lazer_image;

	int most_brightness_number[LAZER_POINTS][2];
	int most_brightness[LAZER_POINTS];

	for(int i=0;i<LAZER_POINTS;i++){
		most_brightness[i]=0;
		most_brightness_number[i][0]=0;
		most_brightness_number[i][1]=0;
	}	

	for(int j=0;j<gimg.rows;j++){
		for(int k=0;k<gimg.step;k++){

			int tmp_brightness = cv::saturate_cast<int>(gimg.data[j*gimg.step+k]);

			if( tmp_brightness >= most_brightness[0] ){


				for(int t=LAZER_POINTS-1 ;t >= 1;t--){
					most_brightness[t] = most_brightness[t-1];	

					most_brightness_number[t][0] =most_brightness_number[t-1][0];
					most_brightness_number[t][1] =most_brightness_number[t-1][1];


				}

				most_brightness_number[0][0] = j;
				most_brightness_number[0][1] = k;
				most_brightness[0] = tmp_brightness;
			}	
		}
	}

	for(int t= 0; t<  LAZER_POINTS;t++){
		lazer_points[t].x = most_brightness_number[t][0];
		lazer_points[t].y = most_brightness_number[t][1];
	}

	//calculate location information
	cv::vector<cv::Point3d> location_inf;

	double l = plane.d/plane.a;
	double sita = std::atan(-plane.c/plane.a);
	double fai = std::atan(-plane.b/plane.a);

	double alfa = I_Mat.at<double>(0,0);
	double beta = I_Mat.at<double>(1,1);
	double ganma = I_Mat.at<double>(0,1);
	double u0 = I_Mat.at<double>(0,2);
	double v0 = I_Mat.at<double>(1,2);


	for(int i=0;i<LAZER_POINTS;i++){


		double location_z = ( l / (std::tan(sita) - ( ((lazer_points[i].x - u0) - (ganma/beta    ) * (lazer_points[i].y - v0) ) / alfa ) + std::tan(fai)*(lazer_points[i].y - v0)/beta ) );

		double location_x = ( ( (lazer_points[i].x - u0) - (ganma/beta)*(lazer_points[i].y - v0) ) / alfa )*location_z;

		double location_y = ( (lazer_points[i].y - v0) /beta) * location_z;

		location_inf.push_back(cv::Point3f(location_x,location_y,location_z) );

	}

	/*
	   for(int i=0;i<gimg.rows;i++){
	   for(int j=0;j<gimg.step;j++){

	   int check=0;

	   for(int k=0;k<LAZER_POINTS ;k++){
	   if( i == lazer_points[k].x && j == lazer_points[k].y) 
	   check++;
	   }

	   if(check > 0){


	   gimg.data[i*gimg.step+ j] =  255;


	   }

	   else
	   gimg.data[i*gimg.step+ j] =  0;


	   }
	   }

	   cv::imshow("R4",gimg);
	 */



	return location_inf;
}


cv::vector<cv::Point2d>DetectBrightLine(cv::Mat image)
{

	int count =0;

	cv::Mat output_image(image.size(),image.type());
	//cv::Mat output_image2(image.size(),image.type());

	double threshold = 200;	
	cv::threshold(image,output_image,threshold,0,cv::THRESH_TOZERO);	
	//cv::threshold(output_image,output_image,threshold,0,cv::THRESH_TOZERO);	


	//cv::Sobel(output_image,output_image,1,0,3);
	//cv::Sobel(output_image,output_image,CV_32F,1,1);
	cv::vector<cv::Point2d> lazer_line ;

	for(int j=0;j<image.step;j++){
		int up = 0;
		int down = 0;
		int count = 0;

		for(int i=0;i<image.rows;i+=PIXEL_INTERVAL){

			if(cv::saturate_cast<int>(image.data[i*image.step+j]) > 200 ){
				if( up ==0 )up = i;
				count++;
			}


		}
		if(count != 0){
			//push back gravity point
			lazer_line.push_back( cv::Point2d(up+(count/2),j) );
		}
		//else
			//lazer_line.push_back( cv::Point2d(0,j) );

	}	

	return lazer_line;
}




