
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>


int main(){

	cv::Mat cimgl(XI_H,XI_W,CV_8UC1);

	cv::Mat gimgl(cimgl.size(),cimgl.type());
	

	cv::Mat output_image(cimgl.size(),cimgl.type());

	std::stringstream ss;
	ss << "okami.jpg";
	std::string filename = ss.str();

	cimgl = cv::imread(filename.c_str(),0);
	gimgl = ~cimgl;
	output_image= gimgl+ cimgl;

	cv::imshow("gimgl",gimgl);
	cv::imshow("cimgl",cimgl);
	cv::imshow("output",output_image);

	//imwrite("./output2.bmp",cimgl);
	
	std::cout << "finish" << std::endl;

	cv::waitKey(0);
}


