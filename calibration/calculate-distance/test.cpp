#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#define IMAGE_SIZE 1
#define LAZER_POINTS 5
#define CHESS_SIZE 21
#define CHESS_ROW 9
#define CHESS_COLUM 6

#define XI_W 648
#define XI_H 488

int main( int argc, char* argv[])
{

	//load the raw and lazer image 

	cv::vector<cv::Mat> lazer_image;
	//cv::vector<cv::Mat> lazer_image(XI_H,XI_W,CV_8UC1);

	for(int i=0;i<IMAGE_SIZE;i++)
	{

		//load images
		std::stringstream ss;
		std::string image_name;
		ss <<  i << "_.png";
		image_name = ss.str();	
		lazer_image.push_back( cv::imread(image_name.c_str(),0) );

	}

	/*load inside and outside parameter at camera*/
	cv::Mat I_Mat ; 
	cv::Mat D_Mat ;
	cv::FileStorage fs("camera.xml",CV_STORAGE_READ);
	//cv::FileStorage fs("camera.xml",cv::FileStorage::READ);

	fs["intrinsicMat"] >> I_Mat;
	fs["distCoeffs"] >> D_Mat;

	/*load projector parameter */
	double plane_a;
	double plane_b;	
	double plane_c;	
	double plane_d;	

	cv::FileStorage fs_projector("projector.xml",CV_STORAGE_READ);
	//cv::FileStorage fs("camera.xml",cv::FileStorage::READ);

	fs_projector["plane_a"] >>plane_a; 
	fs_projector["plane_b"] >>plane_b; 
	fs_projector["plane_c"] >>plane_c; 
	fs_projector["plane_d"] >>plane_d; 
	std::cout << "plane_a : " << plane_a<< std::endl;
	std::cout << "plane_b : " << plane_b<< std::endl;
	std::cout << "plane_c : " << plane_c<< std::endl;
	std::cout << "plane_d : " << plane_d<< std::endl;


	//calculate lazer points
	cv::vector<cv::Point2i> lazer_points(IMAGE_SIZE*LAZER_POINTS);


	for(int i=0;i<IMAGE_SIZE;i++){

		cv::Mat gimg(lazer_image[i].size(),lazer_image[i].type()) ;
		gimg = lazer_image[i];

		cv::imshow("R",lazer_image[i]);
		cv::imshow("R2",gimg);


		int most_brightness_number[LAZER_POINTS][2];
		int most_brightness[LAZER_POINTS];


		for(int i=0;i<LAZER_POINTS;i++){
			most_brightness[i]=0;
			most_brightness_number[i][0]=0;
			most_brightness_number[i][1]=0;
		}	


		for(int j=0;j<gimg.rows;j++){
			for(int k=0;k<gimg.step;k++){

				//unsigned char tmp_brightness = gimg.at<uchar>(j,k);
				int tmp_brightness = cv::saturate_cast<int>(gimg.data[j*gimg.step+k]);
				//unsigned char tmp_brightness = lazer_image_lazer[i].at<uchar>(j,k);

				if( tmp_brightness >= most_brightness[0] ){

					for(int t=LAZER_POINTS-1 ;t >= 1;t--){
						most_brightness[t] = most_brightness[t-1];	

						most_brightness_number[t][0] =most_brightness_number[t-1][0];
						most_brightness_number[t][1] =most_brightness_number[t-1][1];


					}

					most_brightness_number[0][0] = j;
					most_brightness_number[0][1] = k;
					most_brightness[0] = tmp_brightness;
				}	
			}
		}

		cv::Mat gimgl(lazer_image[i].size(),lazer_image[i].type()) ;
		gimgl = lazer_image[i];


		for(int i=0;i<gimgl.rows;i++){
			for(int j=0;j<gimgl.step;j++){

				int check=0;

				for(int k=0;k<LAZER_POINTS;k++){
					if( i == most_brightness_number[k][0] && j == most_brightness_number[k][1]) 
						check++;
				}

				if(check > 0){


					gimgl.data[i*gimgl.step+ j] =  255;


				}

				else
					gimgl.data[i*gimgl.step+ j] =  0;


			}
		}

		cv::imshow("R3",gimgl);

		//lazer_points.push_back(light_point);
		for(int t= i*LAZER_POINTS; t< (i+1) *LAZER_POINTS;t++){
			//lazer_points[t].x = t+ 5;
			lazer_points[t].x = most_brightness_number[t-i*LAZER_POINTS][0];
			//lazer_points[t].y = t;
			lazer_points[t].y = most_brightness_number[t-i*LAZER_POINTS][1];
		}

		cv::waitKey(0);
	}	

	//calculate location information
	cv::vector<cv::Point3f> location_inf;


	double l = plane_d/plane_a;
	double sita = std::atan(-plane_c/plane_a);
	double fai = std::atan(-plane_b/plane_a);

	double alfa = I_Mat.at<double>(0,0);
	double beta = I_Mat.at<double>(1,1);
	double ganma = I_Mat.at<double>(0,1);
	double u0 = I_Mat.at<double>(0,2);
	double v0 = I_Mat.at<double>(1,2);


	for(int i=0;i<IMAGE_SIZE*LAZER_POINTS;i++){



		double location_z = ( l / (std::tan(sita) - ( ((lazer_points[i].x - u0) - (ganma/beta    ) * (lazer_points[i].y - v0) ) / alfa ) + std::tan(fai)*(lazer_points[i].y - v0)/beta ) );
		//location_inf.z = ( l / (std::tan(sita) - ( ((lazer_points[i].x - u0) - (ganma/beta    ) * (lazer_points[i].y - v0) ) / alfa ) + std::tan(fai)*(lazer_points[i].y - v0)/beta ) );

		double location_x = ( ( (lazer_points[i].x - u0) - (ganma/beta)*(lazer_points[i].y - v0) ) / alfa )*location_z;
		//location_inf.x = ( ( (lazer_points[i].x - u0) - (ganma/beta)*(lazer_points[i].y - v0) ) / alfa )*location_inf.z;

		double location_y = ( (lazer_points[i].y - v0) /beta) * location_z;
		//location_inf.y = ( (lazer_points[i].y - v0) /beta) * location_inf.z;

		location_inf.push_back(cv::Point3f(location_x,location_y,location_z) );


	}


	std::cout << "location : " << location_inf << std::endl;

	return 0;
}










