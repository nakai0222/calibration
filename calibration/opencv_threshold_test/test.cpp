

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


	int thr = 85;
	int BGR = 1;
	cv::Mat cimgl;
	//cv::Mat cimgl(XI_H,XI_W,CV_8UC1);
	cv::Mat gimg(XI_H, XI_W, CV_8UC1);
	cv::Mat gimgl;
	cv::Mat gimgl2(XI_H, XI_W, CV_8UC1);
	cv::Mat gimgl3(XI_H, XI_W, CV_8UC1);


	cv::Mat split_imgl[3];//={cv::Mat(XI_H, XI_W, CV_8UC1)};

	std::stringstream ss;
	ss << "1.png";
	//ss << "imgl00.png";
	std::string filename = ss.str();

	cimgl = cv::imread(filename.c_str());
	//cimgl = cv::imread(filename.c_str(),CV_LOAD_IMAGE_COLOR);
	imshow("row",cimgl);

	cv::split(cimgl,split_imgl);


	//cv::threshold(split_imgl[BGR+1],gimgl,thr,0,THRESH_TOZERO);

	//gimgl = cimgl.clone(); 
	gimgl = split_imgl[BGR];

	cv::namedWindow("R");
	cv::imshow("R",gimgl);
	cv::imshow("R2",cimgl);
	cv::imshow("R4",split_imgl[0]);

#define POINTNUM 20

	int max[POINTNUM] ;
	int max_num[POINTNUM];
	int max_num_i[POINTNUM];
	int max_num_j[POINTNUM];	


	int count =0;

	int squ = 3;

	for(int i=0;i<cimgl.rows;i++){
		for(int j=0;j<cimgl.cols;j++){
			//unsigned char pixel = gimgl.at<uchar>(i,j);
			int pixel = gimgl.at<uchar>(i,j);

			if( (int)pixel >=  max[0] ){


				for(int i=POINTNUM-1 ;i >= 1;i--){
					max[i] = max[i-1];	

					max_num_i[i] = max_num_i[i-1];
					max_num_j[i] = max_num_j[i-1];

				}
				max[0] = pixel;
				max_num_i[0] = i;
				max_num_j[0] = j;

				count++;
				//max_num = i*cimgl.rows+ j;
				//gimgl.at<uchar>(10,0) = 255;	
				//std::cout << "brightness : "<<(int)pixel << std::endl;
			}
		}
	}


	std::cout << count << std::endl;
	std::cout << max_num_i[0] << std::endl;
	std::cout << max_num_j[0]<< std::endl;

	std::cout << max_num_i[1] << std::endl;
	std::cout << max_num_j[1]<< std::endl;

	std::cout << max_num_i[2] << std::endl;
	std::cout << max_num_j[2]<< std::endl;

	std::cout << max_num_i[3] << std::endl;
	std::cout << max_num_j[3]<< std::endl;


	for(int i=0;i<gimgl.rows;i++){
		for(int j=0;j<gimgl.cols;j++){

			int check=0;

			for(int k=0;k<POINTNUM ;k++){
				if( i == max_num_i[k] && j == max_num_j[k]) 
				//if( i < max_num_i[i] +squ &&  i > max_num_i[i] -squ  && (j < squ+ max_num_j[i] && j >max_num_j[i]-squ ))
				check++;
			}


			if(check > 0)

				gimgl.at<uchar>(i,j) = 255;
			
			else

				gimgl.at<uchar>(i,j) = 0;

		}
	}
	cv::namedWindow("R3");
	imshow("R3",gimgl);


	/*	
		std::cout << max << std::endl;
		std::cout << max_num << std::endl;
		std::cout << max_num/gimgl.cols << std::endl;
		std::cout << max_num%gimgl.cols << std::endl;
		divide(split_imgl[(BGR+1)%3],split_imgl[BGR],gimgl2,10);
		threshold(gimgl2,gimgl2,8,255,THRESH_BINARY_INV);
		namedWindow("B/R");
		imshow("B/R",gimgl2);
		divide(split_imgl[(BGR+2)%3],split_imgl[BGR],gimgl3,10);
		threshold(gimgl3,gimgl3,9,255,THRESH_BINARY_INV);
		namedWindow("G/R");
		imshow("G/R",gimgl3);
		gimgl=gimgl.mul(gimgl2,1.0/255);
		gimgl=gimgl.mul(gimgl3,1.0/255);
		imshow("R2",gimgl);
	 */
	cv::waitKey(0);

}
