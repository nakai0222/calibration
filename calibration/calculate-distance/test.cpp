#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#define IMAGE_SIZE 1
#define CHESS_SIZE 21
#define CHESS_ROW 9
#define CHESS_COLUM 6

#define XI_W 648
#define XI_H 488

int main( int argc, char* argv[])
{

	//load the raw and lazer image 

	cv::vector<cv::Mat> lazer_image;
	//cv::vector<cv::Mat> checker_image(XI_H,XI_W,CV_8UC1);

	for(int i=0;i<IMAGE_SIZE;i++)
	{

		//load images
		std::stringstream ss;
		std::string image_name;
		ss <<  i << "_.png";
		image_name = ss.str();	
		lazer_image.push_back( cv::imread(image_name.c_str(),0) );

	}

	/*load inside and outside parameter at camera*/
	cv::Mat I_Mat ; 
	cv::Mat D_Mat ;
	cv::FileStorage fs("camera.xml",CV_STORAGE_READ);
	//cv::FileStorage fs("camera.xml",cv::FileStorage::READ);

	fs["intrinsicMat"] >> I_Mat;
	fs["distCoeffs"] >> D_Mat;

	/*load projector parameter */
	double plane_a;
	double plane_b;	
	double plane_c;	
	double plane_d;	

	cv::FileStorage fs_projector("projector.xml",CV_STORAGE_READ);
	//cv::FileStorage fs("camera.xml",cv::FileStorage::READ);

	fs_projector["plane_a"] >>plane_a; 
	fs_projector["plane_b"] >>plane_b; 
	fs_projector["plane_c"] >>plane_c; 
	fs_projector["plane_d"] >>plane_d; 
	std::cout << "plane_a : " << plane_a<< std::endl;
	std::cout << "plane_b : " << plane_b<< std::endl;
	std::cout << "plane_c : " << plane_c<< std::endl;
	std::cout << "plane_d : " << plane_d<< std::endl;


	//calculate lazer points
	cv::vector<cv::Point2f> lazer_points(IMAGE_SIZE);
	int thr = 85;
	int BGR = 0;
	cv::Mat split_imgl[3];

	//split color image
	for(int i=0;i<IMAGE_SIZE;i++){

		cv::Mat gimg;

		cv::split(lazer_image[i],split_imgl);
		//cv::threshold(checker_image_lazer[i],gimg,thr,0,cv::THRESH_TOZERO);
		//cv::threshold(split_imgl[BGR],checker_image_lazer[i],thr,0,cv::THRESH_TOZERO);
		cv::imshow("R",lazer_image[i]);

		cv::imshow("r",split_imgl[BGR]);

		cv::waitKey(0);

		//checker_image_lazer[i] = split_imgl[BGR];	
		gimg = split_imgl[BGR];	

		int most_brightness_number[2];
		int most_brightness=0;

		for(int j=0;j<gimg.rows;j++){
			for(int k=0;k<gimg.cols;k++){

				unsigned char tmp_brightness = gimg.at<uchar>(j,k);
				//unsigned char tmp_brightness = checker_image_lazer[i].at<uchar>(j,k);

				if((int)tmp_brightness >= most_brightness ){
					most_brightness_number[0] = j;
					most_brightness_number[1] = k;
					most_brightness = tmp_brightness;
				}	
			}
		}


		std::cout << "x : " << most_brightness_number[0] << std::endl;
		std::cout << "y : " << most_brightness_number[1] << std::endl;
		cv::Point2f light_point(most_brightness_number[0],most_brightness_number[1]);

		//lazer_points.push_back(light_point);
		lazer_points[i].x = light_point.x;
		lazer_points[i].y = light_point.y;
		//std::cout << lazer_points[i].x << std::endl;
	}	




	//calculate location information
	cv::vector<cv::Point3f> location_inf;

	for(int i=0;i<IMAGE_SIZE;i++){

		double l = plane_d/plane_a;
		double sita = std::atan(-plane_c/plane_a);
		double fai = std::atan(-plane_b/plane_a);

		double alfa = I_Mat.at<double>(0,0);
		double beta = I_Mat.at<double>(1,1);
		double ganma = I_Mat.at<double>(0,1);
		double u0 = I_Mat.at<double>(0,2);
		double v0 = I_Mat.at<double>(1,2);



		double location_z = ( l / (std::tan(sita) - ( ((lazer_points[i].x - u0) - (ganma/beta    ) * (lazer_points[i].y - v0) ) / alfa ) + std::tan(fai)*(lazer_points[i].y - v0)/beta ) );
		//location_inf.z = ( l / (std::tan(sita) - ( ((lazer_points[i].x - u0) - (ganma/beta    ) * (lazer_points[i].y - v0) ) / alfa ) + std::tan(fai)*(lazer_points[i].y - v0)/beta ) );

		double location_x = ( ( (lazer_points[i].x - u0) - (ganma/beta)*(lazer_points[i].y - v0) ) / alfa )*location_z;
		//location_inf.x = ( ( (lazer_points[i].x - u0) - (ganma/beta)*(lazer_points[i].y - v0) ) / alfa )*location_inf.z;

		double location_y = ( (lazer_points[i].y - v0) /beta) * location_z;
		//location_inf.y = ( (lazer_points[i].y - v0) /beta) * location_inf.z;

		location_inf.push_back(cv::Point3f(location_x,location_y,location_z) );

	}

	return 0;
}










