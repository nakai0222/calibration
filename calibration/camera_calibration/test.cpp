
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>


#define IMAGE_SIZE 2
#define CHESS_SIZE_Y 10
#define CHESS_SIZE_X 10
#define CHESS_ROW 7
#define CHESS_COLUM 7

int main( int argc, char* argv[])
{
	// Verify arguments
	if(argc < 1)
	{
		std::cerr << "Required: image.png imagePoints.txt objectPoints.txt" << std::endl;
		return -1;
	}

	cv::Size patternsize(CHESS_ROW, CHESS_COLUM);
	std::vector<cv::Point2f> corners;

	std::vector< std::vector<cv::Point2f> > imagePoints;
	std::vector< std::vector<cv::Point3f> > objectPoints;

	cv::Mat image; 

	//extraction image points and object points
	bool found = true;
	for(int i=0;i<IMAGE_SIZE;i++)
	{

		//load images
		std::cout << "loding chessboard image..." << std::endl;
	
		//ss.str("");	
		//ss.clear(std::stringstream::goodbit);

		std::stringstream ss;
		ss <<  i << ".png";
		
		std::string filename = ss.str();	
		image = cv::imread(filename.c_str(),CV_LOAD_IMAGE_COLOR);


		//find corner
		std::cout << "finding chessboard corners..." << std::endl;
		found = findChessboardCorners(image, patternsize, corners, cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK);

		//find more precise corners.
		if(found) 
		{
			std::cout << "found. finding corners in sub-pixel accuracy..." << std::endl;
			cv::Mat gray;
			cv::cvtColor(image, gray, CV_BGR2GRAY);
			cv::cornerSubPix(gray, corners, cv::Size(CHESS_ROW, CHESS_COLUM), cv::Size(-1, -1),
					cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			//cv::drawChessboardCorners(image, cv::Size(CHESS_ROW,CHESS_COLUM), corners, found);
			//cv::imshow("image",image);
			//cv::waitKey(0);
		}
		else
		{
			std::cout << "not found."<< std::endl;
			break;
		}

		if(corners.size() == CHESS_ROW * CHESS_COLUM)
		{
			//save 2d cooordenate and world coordinate
			std::vector< cv::Point2f > V_tImgPT;
			std::vector< cv::Point3f > V_t0bjPT;	

			for(int j=0;j<corners.size();j++){
				cv::Point2f tImgPT;
				cv::Point3f t0bjPT;	

				tImgPT.x = corners[j].x;
				tImgPT.y = corners[j].y;

				t0bjPT.x = j%CHESS_ROW*CHESS_SIZE_X;
				t0bjPT.y = j/CHESS_COLUM*CHESS_SIZE_Y;
				t0bjPT.z = 0;

				V_tImgPT.push_back(tImgPT);	
				V_t0bjPT.push_back(t0bjPT);	
			}
			imagePoints.push_back(V_tImgPT);
			objectPoints.push_back(V_t0bjPT);
		}
	}			


	//begin calibration
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	std::vector<cv::Mat> rvecs;
	std::vector<cv::Mat> tvecs;

	cv::calibrateCamera(objectPoints, imagePoints, image.size(), cameraMatrix, distCoeffs, rvecs, tvecs);


	//inside parameter
	std::cout << "inside parameter : "<< std::endl << cameraMatrix << std::endl;
	std::cout << distCoeffs << std::endl;
	//outside parameter
	//std::cout << rvecs << std::endl;
	

	//save camera parameter
	/*
	   for(int i=0;i<distCoeffs.rows;i++){
	   for(int j=0;j<distCoeffs.colum;j++){
	   std::cout << "hoge" << std::endl;

	   }
	   } 
	 */

	

	return 0;

}


