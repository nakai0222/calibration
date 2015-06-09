
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>


#define IMAGE_SIZE 21
#define CHESS_SIZE 21
#define CHESS_ROW 9
#define CHESS_COLUM 6

int main( int argc, char* argv[])
{


	//checker pattern picture
	cv::vector< cv::Mat > checker_image;

	//number of intersection point 
	cv::Size checker_pattern_size(CHESS_ROW,CHESS_COLUM);
	//image points
	cv::vector< cv::vector<cv::Point2f> > image_points(IMAGE_SIZE);
	//world points
	cv::vector< cv::vector<cv::Point3f> > world_points(IMAGE_SIZE);
	//world coordinate pattern 
	cv::TermCriteria criteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,20,0.001);

	//extraction image points and world points

	//load checker images
	for(int i=0;i<IMAGE_SIZE;i++)
	{

		//load images
		//std::cout << "loding chessboard image..." << std::endl;

		std::stringstream ss;
		ss <<  i << ".png";

		std::string image_name = ss.str();
		//load gray images	
		checker_image.push_back( cv::imread(image_name.c_str(),0) );
	}



	//find checker patter
	for(int i=0;i<IMAGE_SIZE;i++)
	{
		//find corner
		//std::cout << "finding chessboard corners..." << std::endl;
		if(cv::findChessboardCorners (checker_image[i], checker_pattern_size, image_points[i] , cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK ) )
		{

			
			cv::cornerSubPix(checker_image[i], image_points[i], cv::Size(11, 11), cv::Size(-1, -1),criteria);	
			cv::drawChessboardCorners( checker_image[i], checker_pattern_size, (cv::Mat)(image_points[i]),true );
			cv::imshow("image",checker_image[i]);
			cv::waitKey(50);
		}
		else
		{
			std::cout << "at least 1 corner not found."<< std::endl;
			std::cout << "number : "<<i << std::endl;	
			return -1;
		}
	}

	//registrate world points
	for(int i=0;i<IMAGE_SIZE;i++){
		for(int j=0;j<checker_pattern_size.area();j++){

			world_points[i].push_back(cv::Point3f( static_cast<float>( (j%checker_pattern_size.width) *CHESS_SIZE),static_cast<float>( (j /checker_pattern_size.width) *CHESS_SIZE), 0.0) ) ;
		}
	}


	//begin calibration
	cv::Mat camera_matrix;
	cv::Mat dist_coeffs;
	cv::vector<cv::Mat> rotation_vector;
	cv::vector<cv::Mat> translation_vector;

	cv::calibrateCamera(world_points, image_points, checker_image[0].size(), camera_matrix, dist_coeffs, rotation_vector, translation_vector);


	//inside parameter
	std::cout << "inside parameter : "<< std::endl << camera_matrix << std::endl;
	std::cout << "distort parameter : "<< std::endl;
	std::cout << dist_coeffs << std::endl;

	std::cout << "outside parameter : " << std::endl <<  translation_vector[0]<< std::endl;
	std::cout << rotation_vector[0]<< std::endl;
	std::cout << "outside parameter : " << std::endl <<  translation_vector[1]<< std::endl;
	std::cout << rotation_vector[1]<< std::endl;
	std::cout << "outside parameter : " << std::endl <<  translation_vector[2]<< std::endl;
	std::cout << rotation_vector[2]<< std::endl;


	//output file 
	cv::FileStorage fs("camera.xml",cv::FileStorage::WRITE);
	fs << "intrinsicMat" << camera_matrix; 
	fs << "distCoeffs" << dist_coeffs;
	
	cv::Mat undistorted;
	
	//reject distort
	for(int i=0;i<IMAGE_SIZE;i++){

	cv::undistort(checker_image[i],undistorted, camera_matrix,dist_coeffs);
	cv::imshow("undistorted",undistorted);
	cv::imshow("row",checker_image[i]);
	cv::waitKey(0);	

	}


	return 0;

}


