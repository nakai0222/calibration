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


class Plane{
	public:
	double a;
	double b;
	double c;
	double d;

};


cv::vector<cv::Point3f> CalcDistInf(cv::Mat lazer_image,Plane plane,cv::Mat I_Mat,cv::Mat D_Mat);



int main( int argc, char* argv[])
{

	//load the raw and lazer image 

	cv::Mat lazer_image;
	//cv::vector<cv::Mat> lazer_image;

	/*load inside and outside parameter at camera*/
	cv::Mat I_Mat ; 
	cv::Mat D_Mat ;
	cv::FileStorage fs("camera.xml",CV_STORAGE_READ);
	//cv::FileStorage fs("camera.xml",cv::FileStorage::READ);

	fs["intrinsicMat"] >> I_Mat;
	fs["distCoeffs"] >> D_Mat;

	/*load projector parameter */
	Plane plane;

	cv::FileStorage fs_projector("projector.xml",CV_STORAGE_READ);
	//cv::FileStorage fs("camera.xml",cv::FileStorage::READ);

	fs_projector["plane_a"] >>plane.a; 
	fs_projector["plane_b"] >>plane.b; 
	fs_projector["plane_c"] >>plane.c; 
	fs_projector["plane_d"] >>plane.d; 
	std::cout << "plane.a : " << plane.a<< std::endl;
	std::cout << "plane.b : " << plane.b<< std::endl;
	std::cout << "plane.c : " << plane.c<< std::endl;
	std::cout << "plane.d : " << plane.d<< std::endl;


	//catch the image

	lazer_image = cv::imread("0_.png",0);	

	
	//calculate
	cv::vector<cv::Point3f> location_inf = CalcDistInf(lazer_image,plane,I_Mat,D_Mat);

	std::cout << "location : " << location_inf << std::endl;

	return 0;
}



cv::vector<cv::Point3f> CalcDistInf(cv::Mat lazer_image,Plane plane,cv::Mat I_Mat,cv::Mat D_Mat){

//calculate lazer points
	cv::vector<cv::Point2i> lazer_points(LAZER_POINTS);



		cv::Mat gimg(lazer_image.size(),lazer_image.type()) ;
		gimg = lazer_image;

		cv::imshow("R",lazer_image);
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

		//lazer_points.push_back(light_point);
		for(int t= LAZER_POINTS; t<  LAZER_POINTS;t++){
			//lazer_points[t].x = t+ 5;
			lazer_points[t].x = most_brightness_number[t][0];
			//lazer_points[t].y = t;
			lazer_points[t].y = most_brightness_number[t][1];
		}

		

	//calculate location information
	cv::vector<cv::Point3f> location_inf;


	double l = plane.d/plane.a;
	double sita = std::atan(-plane.c/plane.a);
	double fai = std::atan(-plane.b/plane.a);

	double alfa = I_Mat.at<double>(0,0);
	double beta = I_Mat.at<double>(1,1);
	double ganma = I_Mat.at<double>(0,1);
	double u0 = I_Mat.at<double>(0,2);
	double v0 = I_Mat.at<double>(1,2);


	for(int i=0;i<LAZER_POINTS;i++){



		double location_z = ( l / (std::tan(sita) - ( ((lazer_points[i].x - u0) - (ganma/beta    ) * (lazer_points[i].y - v0) ) / alfa ) + std::tan(fai)*(lazer_points[i].y - v0)/beta ) );
		//location_inf.z = ( l / (std::tan(sita) - ( ((lazer_points[i].x - u0) - (ganma/beta    ) * (lazer_points[i].y - v0) ) / alfa ) + std::tan(fai)*(lazer_points[i].y - v0)/beta ) );

		double location_x = ( ( (lazer_points[i].x - u0) - (ganma/beta)*(lazer_points[i].y - v0) ) / alfa )*location_z;
		//location_inf.x = ( ( (lazer_points[i].x - u0) - (ganma/beta)*(lazer_points[i].y - v0) ) / alfa )*location_inf.z;

		double location_y = ( (lazer_points[i].y - v0) /beta) * location_z;
		//location_inf.y = ( (lazer_points[i].y - v0) /beta) * location_inf.z;

		location_inf.push_back(cv::Point3f(location_x,location_y,location_z) );


	}

	return location_inf;
}






