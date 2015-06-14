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


	int thr = 150;
	int BGR = 1;
	//cv::Mat_<unsigned char> cimgl(XI_W,XI_H);
	//cv::Mat cimgl;
	cv::Mat cimgl(XI_H,XI_W,CV_8UC1);
	//cv::Mat_<unsigned char> gimgl(XI_H,XI_W,CV_8UC1);
	//cv::Mat_<uchar> gimgl;
	cv::Mat gimgl(cimgl.size(),cimgl.type());



	cv::Mat split_imgl[3];//={cv::Mat(XI_H, XI_W, CV_8UC1)};

	std::stringstream ss;
	ss << "4_.png";
	//ss << "imgl00.png";
	std::string filename = ss.str();

	cimgl = cv::imread(filename.c_str(),0);
	//cimgl = cv::imread(filename.c_str(),CV_LOAD_IMAGE_COLOR);
	//cv::threshold(cimgl,gimgl,thr,0,cv::THRESH_TOZERO);
	gimgl = cimgl;


	cv::imshow("R",gimgl);
	cv::imshow("R2",cimgl);
	//cv::imshow("R4",split_imgl[0]);
	std::cout << "gimgl.row" << gimgl.rows << std::endl;
	std::cout << "gimgl.cols" << gimgl.cols << std::endl;


	//std::cout << cimgl << std::endl;
	cv::waitKey(0);

	#define POINTNUM 20

	int max[POINTNUM] ;
	int max_num[POINTNUM];
	int max_num_i[POINTNUM];
	int max_num_j[POINTNUM];	


	int count =0;


/*

	for(int i=0;i<gimgl.rows;i++){	
		for(int j=0;j<gimgl.step;j++){
			//unsigned char pixel = gimgl.at<uchar>(i,j);
			//gimgl.data[j] += 100; 
			 //std::cout <<  "data : "<< (gimgl.data[i*gimgl.step+j]) <<std::endl;
			 std::cout <<  "data : "<< static_cast<int>(gimgl.data[i*gimgl.step+j]) <<std::endl;
		}
		}

			 std::cout <<  "c data : "<< (cimgl.data[0]) <<std::endl;
*/	
	for(int i=0;i<gimgl.rows;i++){
		for(int j=0;j<gimgl.step;j++){
			//unsigned char pixel = gimgl.at<uchar>(i,j);
			int pixel = cv::saturate_cast<int>(gimgl.data[i*gimgl.step+j]);
			//int pixel = static_cast<int>(gimgl.data[i*gimgl.step+j]);
			//int pixel = gimgl(i,j);

			if( pixel >=  max[0] ){


				std::cout << pixel << std::endl;

				for(int k=POINTNUM-1;k >= 1;k--){
					max[k] = max[k-1];	
					max_num[k] = max_num[k-1];
					max_num_i[k] = max_num_i[k-1];
					max_num_j[k] = max_num_j[k-1];
				}

				max[0] = pixel;
				max_num_i[0] = i;
				max_num_j[0] = j;

				count++;

			}	
		}
	}


	std::cout <<  "max change number:  "<< count << std::endl;
	std::cout << "max 0 : "<< max_num_i[0] << std::endl;
	std::cout << max_num_j[0]<< std::endl;

	std::cout << "max1 : "<<  max_num_i[1] << std::endl;
	std::cout << max_num_j[1]<< std::endl;

	std::cout <<  "max2 : "<< max_num_i[2] << std::endl;
	std::cout <<  max_num_j[2]<< std::endl;

	std::cout << "max3 : "<< max_num_i[3] << std::endl;
	std::cout << max_num_j[3]<< std::endl;

	for(int i=0;i<POINTNUM;i++){
	std::cout << max[i] << std::endl;	
	}


	for(int i=0;i<gimgl.rows;i++){
		//for(int j=0;j<648*3-100;j++){
		for(int j=0;j<gimgl.step;j++){

			//for(int i=0;i<300;i++){
			int check=0;
				
			   for(int k=0;k<POINTNUM ;k++){
			   if( i == max_num_i[k] && j == max_num_j[k]) 
			//if( i < max_num_i[i] +squ &&  i > max_num_i[i] -squ  && (j < squ+ max_num_j[i] && j >max_num_j[i]-squ ))
				check++;
			}
			 		
			   if(check > 0){

			   //gimgl.at<uchar>(i,j) = 255;

			   gimgl.data[i*gimgl.step+ j] =  255;
		

			   }

			   else
			   gimgl.data[i*gimgl.step+ j] =  0;
			
	
			//gimgl.data[i*gimgl.cols + 
		   //gimgl(i,j) = 100;
			   //gimgl.data[i*gimgl.cols+ j] +=  100;
			   //gimgl.at<uchar>(i,j) += 100;
		}
	}

	std::cout << "step: "<<  gimgl.step << std::endl;
	std::cout << "elemSize: "<< gimgl.elemSize()<< std::endl;
	std::cout << "channels: "<< gimgl.channels()<< std::endl;


	//gimgl = cv::Scalar(100);
	cv::namedWindow("R3");
	imshow("R3",gimgl);

	cv::waitKey(0);
	
}
