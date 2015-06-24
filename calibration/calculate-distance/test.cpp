#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#define IMAGE_SIZE 2
#define CHESS_SIZE 21
#define CHESS_ROW 9
#define CHESS_COLUM 6

#define PIXEL_INTERVAL 1
#define XI_W 648
#define XI_H 488


cv::vector<cv::Point2d> DetectBrightLine(cv::Mat image);

int main( int argc, char* argv[])
{

	//load the raw and lazer image 
	cv::vector<cv::Mat> lazer_image;
	/*load inside and outside parameter at camera*/
	cv::Mat I_Mat ; 
	cv::Mat D_Mat ;
	cv::FileStorage fs("camera.xml",CV_STORAGE_READ);
	//cv::FileStorage fs("camera.xml",cv::FileStorage::READ);

	fs["intrinsicMat"] >> I_Mat;
	fs["distCoeffs"] >> D_Mat;

	for(int i=0;i<IMAGE_SIZE;i++)
	{

		//load images
		std::stringstream ss;
		std::string image_name;
		ss <<   "1_.png";
		//ss <<  i << "_.png";
		image_name = ss.str();	
		cv::Mat image = cv::imread(image_name.c_str(),0);
		cv::Mat undistort;
		cv::undistort(image,undistort,I_Mat,D_Mat);

		lazer_image.push_back( undistort );
	}

	cv::imshow("lazer",lazer_image[0]);
	cv::waitKey(0);
	/*load projector parameter */
	double plane_a;
	double plane_b;	
	double plane_c;	
	double plane_d;	

	cv::FileStorage fs_projector("projector.xml",CV_STORAGE_READ);

	fs_projector["plane_a"] >>plane_a; 
	fs_projector["plane_b"] >>plane_b; 
	fs_projector["plane_c"] >>plane_c; 
	fs_projector["plane_d"] >>plane_d; 
	std::cout << "plane_a : " << plane_a<< std::endl;
	std::cout << "plane_b : " << plane_b<< std::endl;
	std::cout << "plane_c : " << plane_c<< std::endl;
	std::cout << "plane_d : " << plane_d<< std::endl;


	//calculate lazer points
	cv::vector<cv::Point2d> lazer_points= DetectBrightLine(lazer_image[0]);

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


	for(int i=0;i<lazer_points.size();i++){

		double location_z = ( l /( (-plane_c/plane_a) - ( ((lazer_points[i].x - u0) - (ganma/beta ) * (lazer_points[i].y - v0) ) / alfa ) - ((plane_b/plane_a)*(lazer_points[i].y - v0))/(beta) ) );
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


cv::vector<cv::Point2d>DetectBrightLine(cv::Mat image)
{

	cv::Mat output_image(image.size(),image.type());

	cv::imshow("r2",image);	
	double threshold = 200;	
	cv::threshold(image,image,threshold,0,cv::THRESH_TOZERO);	

	cv::imshow("r3",image);	
	cv::waitKey(0);

	cv::vector<cv::Point2d> lazer_line;


	
	for(int j=0;j<image.step;j++){
		int edge = 0;
		int pos = 0;
		double pos_edge = 0; 	

		for(int i=0;i<image.rows-1;i+=PIXEL_INTERVAL){
			edge = abs(cv::saturate_cast<int>(image.data[(i+1)*image.step+j]) - cv::saturate_cast<int>(image.data[i*image.step+j]) ) ;
			pos_edge += edge * (i+0.5); 
			pos += edge; 
		}

		if(pos_edge > 0){
			pos_edge = pos_edge/pos;
			
			std::cout << j  << " edge " << pos_edge << std::endl;	
			lazer_line.push_back( cv::Point2d(j,pos_edge) );
			//lazer_line.push_back( cv::Point2d(j,pos_edge) );
		}
		//else
		//lazer_line.push_back( cv::Point2d(0,j) );
	}	
	
	for(int i=0;i<image.rows;i++)	
		for(int j=0;j<image.cols;j++)	
			image.data[i*image.step + j] = 0;

	std::cout << " lazer_line size : " << lazer_line.size() << std::endl;

	for(int i = 0 ; i < lazer_line.size(); i++)
	{

		image.data[( static_cast<int>(lazer_line[i].y)*image.step ) + static_cast<int>(lazer_line[i].x )] = 255;		
	}

	cv::namedWindow("R3");
	imshow("R3",image);
	//imwrite("./output.bmp",image);

	return lazer_line;
}


