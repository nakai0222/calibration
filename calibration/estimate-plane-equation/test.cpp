

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
//#include <math.h>
#include <cmath>

int main( int argc, char* argv[])
{


	cv::Point3f a (1,1,2);
	cv::Point3f b (0,-2,1);
	cv::Point3f c (3,-1,0);

	int x_sum = 0;
	int y_sum = 0;
	int z_sum = 0;
	x_sum = a.x+b.x+c.x;
	y_sum = a.y+b.y+c.y;
	z_sum = a.z+b.z+c.z;

	int x_y_sum = a.x*a.y+b.x*b.y+c.x*c.y;
	int x_z_sum = a.x*a.z+b.x*b.z+c.x*c.z;
	int y_z_sum = a.y*a.z+b.y*b.z+c.y*c.z;
	
	int x_sum_2 = a.x*a.x+b.x*b.x+c.x*c.x;		
	int y_sum_2 = a.y*a.y+b.y*b.y+c.y*c.y;		
	int z_sum_2 = a.z*a.z+b.z*b.z+c.z*c.z;		

	std::cout << "x_sum : " << x_sum<< std::endl;
	std::cout << "y_sum : " << y_sum<< std::endl;
	std::cout << "z_sum : " << z_sum<< std::endl;

	cv::Mat M = (cv::Mat_<double>(3,3) << x_sum_2,x_y_sum,x_sum,x_y_sum,y_sum_2,y_sum,x_sum,y_sum,3);
	//cv::Mat M = (cv::Mat_<double>(3,3) << 3 ,x_sum,y_sum,x_sum,x_sum_2,x_sum*y_sum,y_sum,x_sum*y_sum,y_sum_2);

	cv::Mat u = (cv::Mat_<double>(3,1)<< x_z_sum,y_z_sum, z_sum);
	//cv::Mat u = (cv::Mat_<double>(3,1)<< z_sum ,x_sum*z_sum , y_sum*z_sum);


	std::cout << "M : " << M<< std::endl;
	std::cout << "M_inv : " << M.inv() << std::endl;
	std::cout << "u : " << u<< std::endl;

	cv::Mat projector_parametter;  
	projector_parametter = M.inv()*u;

	std::cout << "projector_parametter : " << projector_parametter << std::endl;
	std::cout << "projector_parametter.data[0] : " << cv::saturate_cast<double>(projector_parametter.at<double>(0,0) ) << std::endl;
	//std::cout << "projector_parametter.data[0] : " << cv::saturate_cast<double>(projector_parametter.data[0]) << std::endl;

	//ax+by+cz+d = 0
	std::cout << "sqrt(4): " << sqrt(4.0) << std::endl;
	double plane_c = 1/sqrt(projector_parametter.at<double>(0,0)*projector_parametter.at<double>(0,0) + projector_parametter.at<double>(1,0)*projector_parametter.at<double>(1,0) + 1);
	double plane_a = -projector_parametter.at<double>(0,0)*plane_c;
	double plane_b = -projector_parametter.at<double>(1,0)*plane_c;
	double plane_d = -projector_parametter.at<double>(2,0)*plane_c;


	std::cout << "plane_a : " << plane_a << std::endl;
	std::cout << "plane_b : " << plane_b << std::endl;
	std::cout << "plane_c : " << plane_c << std::endl;
	std::cout << "plane_d : " << plane_d << std::endl;

	return 0;


}


