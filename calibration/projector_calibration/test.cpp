/*
6/6/2015
7/6/2015
line lazer calibration
*/

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>


#define IMAGE_SIZE 21
#define CHESS_SIZE 21
#define CHESS_ROW 9
#define CHESS_COLUM 6


class Plane{


public:
	double a,b,c,d;
	Plane(){}
	Plane(double a,double b,double c,double d){this->a = a; this->b = b,this->c = c,this->d = d ;}

};





int main( int argc, char* argv[])
{

	//read the raw and lazer image 
	checker_image = imread(img_name);
	checker_image_lazer = imread(img_name);

	/*read inside and outside parameter at camera*/
	cv::Mat I_Mat = imread(); 
	cv::Mat D_Mat = imread(); 
	cv::Mat rotation;
	cv::Mat translation;		

	cv::vector< cv::Point2f > lazer_points;
		
	cv::Mat gimgl(XI_H, XI_W, CV_8UC1);
	/*detect lazer line*/
	//split color image
	cv::split(cimgl,split_imgl);
	cv::threshold(split_imgl[BGR],gimgl,thr,0,THRESH_TOZERO);
	
	lazer_points = ();	

	
	/*match lazer line and camera local position*/

	//number of intersection point 
	cv::Size checker_pattern_size(CHESS_ROW,CHESS_COLUM);
	//image points
	cv::vector<cv::Point2f> image_point;
	//world points
	cv::vector<cv::Point3f> world_point;
	
	//find corner
	if(cv::findChessboardCorners (checker_image,checker_pattern_size, image_point , cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK ) )
		{
			cv::cornerSubPix(checker_image, image_point, cv::Size(11, 11), cv::Size(-1, -1),criteria);	
			cv::drawChessboardCorners( checker_image, checker_pattern_size, (cv::Mat)(image_point),true );
			cv::imshow("image",checker_image);
			cv::waitKey(20);
		}
		else
		{
			std::cout << "at least 1 corner not found."<< std::endl;
			std::cout << "number : "<<i << std::endl;	
			return -1;
		}
	}

	//registrate world points
	for(int j=0;j<checker_pattern_size.area();j++){

	world_points[i].push_back(cv::Point3f( static_cast<float>( (j%checker_pattern_size.width) *CHESS_SIZE),static_cast<float>( (j /checker_pattern_size.width) *CHESS_SIZE), 0.0) ) ;
	}

	//calculate outside parameter
	cv::solvePnP(world_point,image_point,I_mat,D_mat,rotation,translation);


	/*guess projector parameter*/
	cv::vector <cv::Point3f> camera_points;

	//translate points at camera axis 
	for(int i=0; i < checker_pattern_size.area(); i++){
	camera_points.push_back( I_mat.inv()*lazer_points[i] );	
	}

	//calculate ax+by+cz+d=0 
			
	
	
	return 0;
}


