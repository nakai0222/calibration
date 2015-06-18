
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

#define POINTNUM 10
#define PIXEL_INTERVAL 1

cv::Mat Patch(cv::Mat image);
cv::vector<cv::Point2d> DetectBrightLine(cv::Mat image);



int main(){

	cv::Mat cimgl(XI_H,XI_W,CV_8UC1);

	cv::Mat gimgl(cimgl.size(),cimgl.type());

	std::stringstream ss;
	ss << "0_.png";
	std::string filename = ss.str();

	cimgl = cv::imread(filename.c_str(),0);
	gimgl = cimgl;

	cv::imshow("R",gimgl);
	cv::imshow("R2",cimgl);

	//cv::Mat output_image; 
	//output_image= Patch(gimgl);
	/*
	   cv::Laplacian(gimgl,output_image,CV_32F,3);
	   cv::Canny(gimgl,output_image,50,200);
	 */

	cv::vector<cv::Point2d> lazer_line = DetectBrightLine(gimgl);
	
	for(int i=0;i<gimgl.rows;i++){
		for(int j=0;j<gimgl.step;j++){


			if( lazer_line[j].x != 0 && i == lazer_line[j].x ){ 

				gimgl.data[i*gimgl.step+ j] =  255;

			}
			else 
				gimgl.data[i*gimgl.step+ j] =  0;
		}
	}

	cv::namedWindow("R3");
	imshow("R3",gimgl);
	
	std::cout << "finish" << std::endl;

	cv::waitKey(0);
}



cv::vector<cv::Point2d>DetectBrightLine(cv::Mat image)
{


	cv::Mat output_image(image.size(),image.type());
	//cv::Mat output_image2(image.size(),image.type());

	double threshold = 100;	
	cv::threshold(image,output_image,threshold,0,cv::THRESH_TOZERO);	
	//cv::threshold(output_image,output_image,threshold,0,cv::THRESH_TOZERO);	


	//cv::Sobel(output_image,output_image,1,0,3);
	//cv::Sobel(output_image,output_image,CV_32F,1,1);
	cv::imshow("r3",output_image);	
	cv::waitKey(0);

	cv::vector<cv::Point2d> lazer_line ;

	for(int j=0;j<image.step;j++){
		int up = 0;
		int down = 0;
		int count = 0;

		for(int i=0;i<image.rows;i+=PIXEL_INTERVAL){

			if(cv::saturate_cast<int>(image.data[i*image.step+j]) > threshold){
				if( up ==0 )up = i;
				count++;
			}


		}
		if(count >= 5){
			//push back gravity point
			lazer_line.push_back( cv::Point2d(up+(count/2),j) );
		}
		else
			lazer_line.push_back( cv::Point2d(0,j) );

	}	

	return lazer_line;
}




cv::Mat Patch(cv::Mat image)
{

	cv::Mat output_image(image.size(),image.type());

	cv::Mat patch  = (cv::Mat_<double>(3,9) << 1,1,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,1,1);

	for(int i=1;i<image.rows-1;i++){
		for(int j=4;j<image.step-4;j++){
			//int pixel = cv::saturate_cast<int>(image.data[i*image.step+j]);

			output_image.data[i*image.step+j]  = 0;

			//[-1,2),(-4,5]
			cv::Mat image_cut = image(cv::Rect(j-4,i-1,patch.cols,patch.rows));
			image_cut.convertTo(image_cut,CV_64FC1);

			cv::Mat tmp = patch.mul(image_cut);

			for(int k=0;k<tmp.cols*tmp.rows;k++){	
				output_image.data[i*image.step+j]  += tmp.data[k];
			}

		}
	}

	return output_image;
}
