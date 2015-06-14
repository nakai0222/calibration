

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>


int main( int argc, char* argv[])
{


	cv::Point3f a (1,1,2);
	cv::Point3f b (0,-2,1);
	cv::Point3f c (3,-1,0);

	std::cout << "a.x: " << a.x<< std::endl;
	std::cout << "a.y: " << a.y<< std::endl;
	std::cout << "a.z: " << a.z<< std::endl;
	int x_sum = 0;
	int y_sum = 0;
	int z_sum = 0;
	x_sum = a.x+b.x+c.x;
	y_sum = a.y+b.y+c.y;
	z_sum = a.z+b.z+c.z;

	std::cout << "x_sum : " << x_sum<< std::endl;
	std::cout << "y_sum : " << y_sum<< std::endl;
	std::cout << "z_sum : " << z_sum<< std::endl;



	double a_t = x_sum*x_sum;	
	double b_t = y_sum*y_sum;	
	double c_t = x_sum*y_sum;	
	double d_t = x_sum*z_sum;	
	double e_t = y_sum*z_sum;	
	double f_t = x_sum;	
	double g_t = y_sum;	
	double h_t = z_sum;
	double n = 3;	



	double ue =  (c_t*(g_t*h_t - n*e_t)+ f_t*(e_t*g_t - b_t*h_t) + d_t*(n*b_t - g_t*g_t) );
	double sita = a_t*(g_t*g_t - n*b_t)-2*c_t*f_t*g_t +b_t*f_t*f_t + n*c_t*c_t ;
	double a_v = - (ue/sita);

	std::cout << "ue: "<< ue<< std::endl;	
	std::cout << "sita: "<<sita<< std::endl;	
	std::cout << "a_v: "<< a_v << std::endl;	
	//cv::Mat M = (cv::Mat_<double>(3,3) << 16,x_sum*y_sum,x_sum,x_sum*y_sum,y_sum*y_sum,y_sum,x_sum,y_sum,3);
	cv::Mat M = (cv::Mat_<double>(3,3) << 3 ,x_sum,y_sum,x_sum,x_sum*x_sum,x_sum*y_sum,y_sum,x_sum*y_sum,y_sum*y_sum);

	//cv::Mat u = (cv::Mat_<double>(3,1)<< x_sum*z_sum,y_sum*z_sum, z_sum);
	cv::Mat u = (cv::Mat_<double>(3,1)<< z_sum ,x_sum*z_sum , y_sum*z_sum);

	//cv::Mat ml = (cv::Mat_<double>(3,3) << 10,-9,-12,7,-12,11,-10,10,3);

	//std::cout << "ml_inv : " << ml.inv() << std::endl;
	std::cout << "M : " << M<< std::endl;
	std::cout << "M_inv : " << M.inv() << std::endl;
	std::cout << "u : " << u<< std::endl;

	cv::Mat projector_parametter;  
	projector_parametter = M.inv()*u;
	//projector_parametter = M.inv()*u;

	std::cout << "projector_parametter : " << projector_parametter << std::endl;

	return 0;


}


