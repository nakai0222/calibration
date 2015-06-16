
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
#define PIXEL_INTERVAL 4

cv::vector<cv::Point2d> DetectBrightLine(cv::Mat image);
cv::Mat Patch(cv::Mat image);


int main(){

	cv::Mat cimgl(XI_H,XI_W,CV_8UC1);

	cv::Mat gimgl(cimgl.size(),cimgl.type());

	std::stringstream ss;
	ss << "4_.png";
	std::string filename = ss.str();

	cimgl = cv::imread(filename.c_str(),0);
	gimgl = cimgl;

	cv::imshow("R",gimgl);
	cv::imshow("R2",cimgl);

	cv::Mat output_image; 
	//output_image= Patch(gimgl);

	double threshold = 50;	
	cv::Sobel(gimgl,output_image,CV_32F,1,1);
	//cv::threshold(split_imgl[BGR],gimgl,thr,0,THRESH_TOZERO);
	cv::threshold(output_image,output_image,threshold,0,cv::THRESH_TOZERO);	
	cv::imshow("r3",output_image);	
	/*
	   cv::vector<cv::Point2d> lazer_line = DetectBrightLine(gimgl);
	   for(int i=0;i<gimgl.rows;i++){
	   for(int j=0;j<gimgl.step;j++){

	   int check=0;

	   for(int k=0;k<POINTNUM ;k++){
	   if( i == lazer_line[k].x && j == lazer_line[k].y) 
	   check++;
	   }

	   if(check > 0){

	   gimgl.data[i*gimgl.step+ j] =  255;

	   }

	   else
	   gimgl.data[i*gimgl.step+ j] =  0;

	   }
	   }
	 */
	cv::namedWindow("R3");
	imshow("R3",gimgl);

	std::cout << "finish" << std::endl;

	cv::waitKey(0);
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

cv::vector<cv::Point2d>DetectBrightLine(cv::Mat image)
{


	int count =0;

	cv::Mat output_image(image.size(),image.type());

	cv::Mat patch = (cv::Mat_<double>(3,9) << 1,1,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,0,1,1,0,0,0,0,0,1,1);

	for(int i=1;i<image.rows-1;i++){
		for(int j=4;j<image.step-4;j++){
			//int pixel = cv::saturate_cast<int>(image.data[i*image.step+j]);

			output_image.data[i*image.step+j]  = 0;

			//if(i >= 1 && i <= image.rows-1 && j >= 4 && j <= image.cols-4 )
			//[-1,2),(-4,5]
			cv::Mat image_cut = image(cv::Rect(j-4,i-1,patch.cols,patch.rows));
			image_cut.convertTo(image_cut,CV_64FC1);

			cv::Mat tmp = patch.mul(image_cut);

			for(int k=0;k<tmp.cols*tmp.rows;k++){	
				output_image.data[i*image.step+j]  += tmp.data[k];
			}
		}
	}

	cv::vector<cv::Point2d> lazer_line ;
	return lazer_line;
}




