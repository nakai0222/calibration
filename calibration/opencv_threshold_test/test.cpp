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

cv::vector<cv::Point2d> DetectBrightLine(cv::Mat image);



int main(){


	int thr = 150;


	cv::Mat cimgl(XI_H,XI_W,CV_8UC1);

	cv::Mat gimgl(cimgl.size(),cimgl.type());



	cv::Mat split_imgl[3];

	std::stringstream ss;
	ss << "4_.png";
	std::string filename = ss.str();

	cimgl = cv::imread(filename.c_str(),0);
	gimgl = cimgl;


	cv::imshow("R",gimgl);
	cv::imshow("R2",cimgl);
	std::cout << "gimgl.row" << gimgl.rows << std::endl;
	std::cout << "gimgl.cols" << gimgl.cols << std::endl;


	cv::vector<cv::Point2d> lazer_line = DetectBrightLine(gimgl);

	std::cout << "yet complete" << std::endl;
	

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

	cv::namedWindow("R3");
	imshow("R3",gimgl);

	cv::waitKey(0);
	
}



cv::vector<cv::Point2d> DetectBrightLine(cv::Mat image)
{


	int max[POINTNUM] ;
	int max_num[POINTNUM];
	int max_num_i[POINTNUM];
	int max_num_j[POINTNUM];	


	int count =0;

	
	for(int i=0;i<image.rows;i++){
		for(int j=0;j<image.step;j++){
			int pixel = cv::saturate_cast<int>(image.data[i*image.step+j]);
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


	cv::vector<cv::Point2d> lazer_line ;
	for(int i=0;i<POINTNUM;i++){
	 lazer_line[i].x = max_num_i[i];
	 lazer_line[i].y = max_num_j[i];
		
	}


	return lazer_line;

}


