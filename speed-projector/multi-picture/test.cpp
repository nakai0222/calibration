
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>


#define XI_W 648
#define XI_H 488



int main(){

	cv::Mat cimgl(XI_H,XI_W,CV_8UC1);

	cv::Mat gimgl(cimgl.size(),cimgl.type());
	

	cv::Mat output_image(cimgl.size(),cimgl.type());

	std::stringstream ss;
	ss << "earth.jpg";
	std::string filename = ss.str();

	std::cout << "rows" << cimgl.rows;


	cimgl = cv::imread(filename.c_str(),0);
	gimgl = ~cimgl;
	output_image= gimgl+ cimgl;

	cv::imshow("gimgl",gimgl);
	cv::imshow("cimgl",cimgl);
	cv::imshow("output",output_image);

	imwrite("./earth-gray.jpg",cimgl);
	imwrite("./mearth-gray.jpg",gimgl);
	
	std::cout << "finish" << std::endl;

	cv::waitKey(0);
}


