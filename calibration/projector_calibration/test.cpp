

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#define IMAGE_SIZE 5
#define CHESS_SIZE 21
#define CHESS_ROW 9
#define CHESS_COLUM 6

#define POINTS_FOR_ONEIMAGE 2 

#define XI_W 648
#define XI_H 488



int main( int argc, char* argv[])
{

	//load the raw and lazer image 

	cv::vector<cv::Mat> checker_image;
	//cv::vector<cv::Mat> checker_image(XI_H,XI_W,CV_8UC1);
	cv::vector<cv::Mat> checker_image_lazer;
	//cv::vector<cv::Mat> checker_image_lazer(XI_H,XI_W,CV_8UC1);

	for(int i=0;i<IMAGE_SIZE;i++)
	{

		//load images
		std::stringstream ss;
		std::string image_name;
		ss <<  i << ".png";
		image_name = ss.str();	
		checker_image.push_back( cv::imread(image_name.c_str(),0) );


		std::stringstream ss1;
		std::string image_name1;
		ss1 << i << "_.png";
		image_name1 = ss1.str();	
		checker_image_lazer.push_back( cv::imread(image_name1.c_str(),0) );

	}

	//number of intersection point 
	cv::Size checker_pattern_size(CHESS_ROW,CHESS_COLUM);
	//image points
	cv::vector< cv::vector<cv::Point2f> > image_points(IMAGE_SIZE);
	//world points
	cv::vector< cv::vector<cv::Point3f> > world_points(IMAGE_SIZE);
	//world coordinate pattern 
	cv::TermCriteria criteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS,20,0.001);


	/*read inside and outside parameter at camera*/
	cv::Mat I_Mat ; 
	cv::Mat D_Mat ;
	cv::FileStorage fs("camera.xml",CV_STORAGE_READ);
	//cv::FileStorage fs("camera.xml",cv::FileStorage::READ);

	fs["intrinsicMat"] >> I_Mat;
	fs["distCoeffs"] >> D_Mat;


	std::cout << "inner parameter: " << I_Mat << std::endl;
	std::cout << "distCoeffs: " << D_Mat << std::endl;


	cv::vector<cv::Mat> rotations;
	cv::vector<cv::Mat> translations;


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

			world_points[i].push_back(cv::Point3f( static_cast<float>( j%checker_pattern_size.width*10),static_cast<float>(j /checker_pattern_size.width * 10), 0.0) ) ;
		}
	}

	//calculate outside parameter
	for(int i=0;i<IMAGE_SIZE;i++){
		cv::Mat tmp_rotation;
		cv::Mat tmp_translation;
		cv::solvePnP(world_points[i],image_points[i],I_Mat,D_Mat,tmp_rotation,tmp_translation);
		rotations.push_back(tmp_rotation);
		translations.push_back(tmp_translation);	
	}

	cv::vector<cv::Mat> rotations_mat(IMAGE_SIZE);

	for(int i=0;i<IMAGE_SIZE;i++){
		cv::Rodrigues(rotations[i],rotations_mat[i]);
	}	
	std::cout << "rotation : " <<  rotations[0]<< std::endl;
	std::cout << "rotation mat : " <<  rotations_mat[0]<< std::endl;
	std::cout << "translation : " <<  translations[0]<< std::endl;


	//calculate lazer points
	cv::vector<cv::Point2i> lazer_points(POINTS_FOR_ONEIMAGE*IMAGE_SIZE);
	//int thr = 85;
	//int BGR = 0;
	//cv::Mat split_imgl[3];

	//split color image
	for(int i=0;i<IMAGE_SIZE;i++){

		cv::Mat gimg(checker_image_lazer[i].size(),checker_image_lazer[i].type()) ;
		gimg = checker_image_lazer[i];

		//std::cout <<  "gimg.step : "<< gimg.step << std::endl;
		//std::cout <<  "gimg.channel: "<< gimg.channels()<< std::endl;

		//cv::split(checker_image_lazer[i],split_imgl);
		//cv::threshold(checker_image_lazer[i],gimg,thr,0,cv::THRESH_TOZERO);
		//cv::threshold(split_imgl[BGR],checker_image_lazer[i],thr,0,cv::THRESH_TOZERO);
		cv::imshow("R",checker_image_lazer[i]);
		cv::imshow("R2",gimg);

		//cv::imshow("r",split_imgl[BGR]);

		cv::waitKey(0);

		//checker_image_lazer[i] = split_imgl[BGR];	
		//gimg = split_imgl[BGR];	

		int most_brightness_number[POINTS_FOR_ONEIMAGE][2];
		int most_brightness[POINTS_FOR_ONEIMAGE];


		for(int i=0;i<POINTS_FOR_ONEIMAGE;i++){
			most_brightness[i]=0;
			most_brightness_number[i][0]=0;
			most_brightness_number[i][1]=0;
		}	


		for(int j=0;j<gimg.rows;j++){
			for(int k=0;k<gimg.step;k++){

				//unsigned char tmp_brightness = gimg.at<uchar>(j,k);
				int tmp_brightness = cv::saturate_cast<int>(gimg.data[j*gimg.step+k]);
				//unsigned char tmp_brightness = checker_image_lazer[i].at<uchar>(j,k);

				if( tmp_brightness >= most_brightness[0] ){

					for(int t=POINTS_FOR_ONEIMAGE-1 ;t >= 1;t--){
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


		//std::cout << "x : " << most_brightness_number[0] << std::endl;
		//std::cout << "y : " << most_brightness_number[1] << std::endl;
		//cv::Point2f light_point(most_brightness_number[0],most_brightness_number[1]);

		//lazer_points.push_back(light_point);
		for(int t= i*POINTS_FOR_ONEIMAGE ; t< (i+1) *POINTS_FOR_ONEIMAGE;t++){
			//lazer_points[t].x = t+ 5;
			lazer_points[t].x = most_brightness_number[t-i*POINTS_FOR_ONEIMAGE][0];
			//lazer_points[t].y = t;
			lazer_points[t].y = most_brightness_number[t-i*POINTS_FOR_ONEIMAGE][1];
		}
	}	


	cv::vector <cv::Point3f> camera_points;

	for(int i=0;i<IMAGE_SIZE*POINTS_FOR_ONEIMAGE;i++){


		//translate points at camera axis
		int vector_num = static_cast<int>(i/POINTS_FOR_ONEIMAGE); 
		cv::Mat r0 = rotations_mat[vector_num].col(0);	
		cv::Mat r1 = rotations_mat[vector_num].col(1);	
		cv::Mat t = translations[vector_num]; 


		cv::Mat q = (cv::Mat_<double>(3,3)<<  r0.at<double>(0,0),  r1.at<double>(0,0),  t.at<double>(0,0),  r0.at<double>(1,0),  r1.at<double>(1,0),  t.at<double>(1,0), r0.at<double>(2,0),  r1.at<double>(2,0),  t.at<double>(2,0)) ;




		cv::Mat k = (cv::Mat_<double>(4,3) << q.at<double>(0,0)  , q.at<double>(0,1)  , q.at<double>(0,2)  , q.at<double>(1,0)  , q.at<double>(1,1)  , q.at<double>(1,2)  , q.at<double>(2,0)  , q.at<double>(2,1)  , q.at<double>(2,2)  , 0 , 0, 1  );



		cv::Mat q_inv = q.inv();	
		//std::cout << "q" << q << std::endl;
		//std::cout << "k" << k<< std::endl;


		cv::Mat lazer_point = (cv::Mat_<double>(3,1) << lazer_points[i].x , lazer_points[i].y,1);
		std::cout << "lazer_point" << lazer_point << std::endl;



		cv::Mat I_Mat_inv = I_Mat.inv();

		cv::Mat camera_point;
		//camera_point = k*q_inv*I_Mat_inv*lazer_point; 
		camera_point = k*q_inv;
		//std::cout << "camera_point" << camera_point << std::endl;
		camera_point = camera_point*I_Mat_inv;
		//std::cout << "camera_point" << camera_point << std::endl;
		camera_point = camera_point*lazer_point; 
		//std::cout << "camera_point" << camera_point << std::endl;


		double div = camera_point.at<double>(3,0);
		//std::cout << "div : " << div << std::endl;


		std::cout << "camera_point" << camera_point/div << std::endl;

		camera_points.push_back( cv::Point3f(camera_point.at<double>(0,0)/div,camera_point.at<double>(1,0)/div,camera_point.at<double>(2,0)/div ));	
		//camera_points.push_back( cv::Point3f(camera_point.at<double>(0,0),camera_point.at<double>(1,0),camera_point.at<double>(2,0) ));	

		//camera_points.push_back(cv::Point3f(1,1,1));	
	}

	//calculate plane parameter
	//URL:samueruchoi.blogspot.jp/2013/02/matlab_20.html


	int x_sum = 0;
	int y_sum = 0;
	int z_sum = 0;
	int x_squ_sum =0;
	int y_squ_sum =0;
	int z_squ_sum =0;
	int x_y_sum =0;
	int x_z_sum =0;
	int y_z_sum =0;
	

	for(int i=0;i<IMAGE_SIZE*POINTS_FOR_ONEIMAGE;i++){

		x_sum += camera_points[i].x;
		y_sum += camera_points[i].y;
		z_sum += camera_points[i].z;

		x_squ_sum += camera_points[i].x*camera_points[i].x;
		y_squ_sum += camera_points[i].y*camera_points[i].y;
		z_squ_sum += camera_points[i].z*camera_points[i].z;

		x_y_sum += camera_points[i].x*camera_points[i].y;
		x_z_sum += camera_points[i].x*camera_points[i].z;
		y_z_sum += camera_points[i].y*camera_points[i].z;
	
	}

	
	std::cout << "x_sum : " << x_sum<< std::endl;
	std::cout << "y_sum : " << y_sum<< std::endl;
	std::cout << "z_sum : " << z_sum<< std::endl;


	int projector_parametter_num = 3;

	cv::Mat M = (cv::Mat_<double>(projector_parametter_num,projector_parametter_num) << x_squ_sum,x_y_sum,x_sum,x_y_sum,y_squ_sum,y_sum,x_sum,y_sum,IMAGE_SIZE*POINTS_FOR_ONEIMAGE);
	//cv::Mat M = (cv::Mat_<double>(projector_parametter_num,projector_parametter_num) << IMAGE_SIZE*POINTS_FOR_ONEIMAGE,x_sum,y_sum,x_sum,x_squ_sum,x_sum*y_sum,y_sum,x_sum*y_sum,y_squ_sum);

	cv::Mat u = (cv::Mat_<double>(projector_parametter_num,1)<< x_z_sum ,y_z_sum, z_sum);
	//cv::Mat u = (cv::Mat_<double>(projector_parametter_num,1)<< z_sum ,x_sum*z_sum , y_sum*z_sum);

	//cv::Mat projector_parametter = (cv::Mat_<double>(projector_parametter_num,1) << 0,0,0) ;

	std::cout << "M : " << M<< std::endl;
	std::cout << "M_inv : " << M.inv() << std::endl;
	std::cout << "u : " << u<< std::endl;
	


	cv::Mat projector_parametter;  
	projector_parametter = M.inv()*u;


	std::cout << "projector_parametter : " << projector_parametter << std::endl;


	//ax+by+cz+d = 0
	double plane_c = 1/sqrt(projector_parametter.at<double>(0,0)*projector_parametter.at<double>(0,0) + projector_parametter.at<double>(1,0)*projector_parametter.at<double>(1,0) + 1);
	double plane_a = -projector_parametter.at<double>(0,0)*plane_c;
	double plane_b = -projector_parametter.at<double>(1,0)*plane_c;
	double plane_d = -projector_parametter.at<double>(2,0)*plane_c;


	std::cout << "plane_a : " << plane_a << std::endl;
	std::cout << "plane_b : " << plane_b << std::endl;
	std::cout << "plane_c : " << plane_c << std::endl;
	std::cout << "plane_d : " << plane_d << std::endl;

		
	//output file 
	cv::FileStorage fs_projector("projector.xml",cv::FileStorage::WRITE);
	fs_projector << "plane_a" << plane_a ;
	fs_projector << "plane_b" << plane_b ;
	fs_projector << "plane_c" << plane_c ;
	fs_projector << "plane_d" << plane_d ;
	//fs_projector << "plane_d" << projector_parameter.data[3]; 
		

	return 0;


}


