#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>

#define IMAGE_SIZE 3
#define CHESS_SIZE 21
#define CHESS_ROW 9
#define CHESS_COLUM 6

#define XI_W 648
#define XI_H 488


//平面の定義
class Plane {	//ax+by+cz+d=0
	public:
		double a,b,c,d;

		Plane(){}
		Plane(double a,double b,double c,double d){ this->a = a; this->b = b; this->c = c; this->d = d; }
};

//ベクトルの定義
class Vector3D{
	public:
		double x,y,z;

		Vector3D(){}
		Vector3D( double x, double y, double z) {this->x = x; this->y = y; this->z = z; }

		//ベクトル引き算( this - v )
		Vector3D operator - ( const Vector3D& v ) const { return Vector3D( x - v.x, y - v.y, z - v.z ); }
		//ベクトル外積( this × vr )
		Vector3D operator * ( const Vector3D& vr ) const { return Vector3D( (y * vr.z) - (z * vr.y), (z * vr.x) - (x * vr.z), (x * vr.y) - (y * vr.x) ); }
		//自身を単位ベクトルにする
		void Normalize() {
			double length = pow( ( x * x ) + ( y * y ) + ( z * z ), 0.5 );//ベクトルの長さ
			x /= length;
			y /= length;
			z /= length;
		}
};
//頂点の定義(ベクトルと同じ)
#define Vertex3D Vector3D

//頂点abcで作られたポリゴンから法線を計算する。
Vector3D CreatePolygonNormal( Vertex3D a, Vertex3D b, Vertex3D c ) {

	Vector3D ab = b - a;
	Vector3D bc = c - b;

	Vector3D normal = ab * bc;	//ab bcの外積
	normal.Normalize();//単位ベクトルにする

	return normal;
}

//ひとつの頂点と法線ベクトルから平面を作成する
Plane CreatePlaneFromPointNormal( Vertex3D p, Vector3D normal )//※normalは単位ベクトルであること
{
	//pとnormalを内積
	double d = p.x * normal.x + p.y * normal.y + p.z * normal.z;

	return Plane( normal.x, normal.y, normal.z, d );
}

//ポリゴンから平面を作成する
Plane CreatePlaneFromPolygon( Vertex3D a, Vertex3D b, Vertex3D c )//※abcは同一でないこと
{	
	//ポリゴンの法線を計算する
	Vector3D normal = CreatePolygonNormal(a,b,c);

	//ポリゴンのどれかひとつの頂点と法線ベクトルから平面を作成する
	return CreatePlaneFromPointNormal( a, normal );
}


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

	std::cout << checker_image_lazer[0].rows << std::endl;
	std::cout << checker_image_lazer[0].cols<< std::endl;


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


	std::cout << "rotation : " <<  rotations[0]<< std::endl;

	cv::Mat rotation_mat;
	cv::Rodrigues(rotations[0],rotation_mat);
	std::cout << "rotation mat : " <<  rotation_mat<< std::endl;
	std::cout << "translation : " <<  translations[0]<< std::endl;


	//calculate lazer points
	cv::vector<cv::Point2f> lazer_points(IMAGE_SIZE);
	int thr = 85;
	int BGR = 0;
	cv::Mat split_imgl[3];

	//split color image
	for(int i=0;i<IMAGE_SIZE;i++){
		
		cv::Mat gimg;

		cv::split(checker_image_lazer[i],split_imgl);
		//cv::threshold(checker_image_lazer[i],gimg,thr,0,cv::THRESH_TOZERO);
		//cv::threshold(split_imgl[BGR],checker_image_lazer[i],thr,0,cv::THRESH_TOZERO);
		cv::imshow("R",checker_image_lazer[i]);
	
		cv::imshow("r",split_imgl[BGR]);

		cv::waitKey(0);

		//checker_image_lazer[i] = split_imgl[BGR];	
		gimg = split_imgl[BGR];	
		
		int most_brightness_number[2];
		int most_brightness=0;

		for(int j=0;j<checker_image_lazer[i].rows;j++){
			for(int k=0;k<checker_image_lazer[i].cols;k++){

				unsigned char tmp_brightness = gimg.at<uchar>(j,k);
				//unsigned char tmp_brightness = checker_image_lazer[i].at<uchar>(j,k);

				if((int)tmp_brightness >= most_brightness ){
					most_brightness_number[0] = j;
					most_brightness_number[1] = k;
					most_brightness = tmp_brightness;
				}	
			}
		}


		std::cout << "x : " << most_brightness_number[0] << std::endl;
		std::cout << "y : " << most_brightness_number[1] << std::endl;
		cv::Point2f light_point(most_brightness_number[0],most_brightness_number[1]);

		//lazer_points.push_back(light_point);
		lazer_points[i].x = light_point.x;
		lazer_points[i].y = light_point.y;
		//std::cout << lazer_points[i].x << std::endl;
	}	


	cv::vector <cv::Point3f> camera_points;

	for(int i=0;i<IMAGE_SIZE;i++){
		//translate points at camera axis 
	
		/*		
		cv::Mat r3= (cv::Mat)(rotation_mat.col(2));

	
		std::cout << "r3" << r3 << std::endl;


		
		cv::Mat kt = (r3.t()*translations[i]).inv() * r3.t();	

		std::cout << "kt" << kt << std::endl;
	
		cv::Mat diag = (cv::Mat_<double>(4,3) << 1,0,0,0,1,0,0,0,1,kt.at<double>(0,0),kt.at<double>(0,1),kt.at<double>(0,2));


		std::cout << "diag" << diag << std::endl;

		//std::cout << "lazer_x : " << lazer_points[i].x << std::endl;
		//std::cout << "lazer_y : " << lazer_points[i].y << std::endl;
		
		cv::Mat lazer_point = (cv::Mat_<double>(3,1) << lazer_points[i].x , lazer_points[i].y,1);
		std::cout << "lazer_point" << lazer_point<< std::endl;
				
		cv::Mat camera_point;
		camera_point = kt * I_Mat.inv() * lazer_point; 


		std::cout << "camera_point" << camera_point << std::endl;

		camera_points.push_back( cv::Point3f(camera_point.at<double>(0,0),camera_point.at<double>(1,0),camera_point.at<double>(2,0) ));	
		*/	


		


		cv::Mat r0 = rotation_mat.col(0);	
		cv::Mat r1 = rotation_mat.col(1);	
		cv::Mat t = translations[i]; 



		cv::Mat q = (cv::Mat_<double>(3,3)<<  r0.at<double>(0,0),  r1.at<double>(0,0),  t.at<double>(0,0),  r0.at<double>(1,0),  r1.at<double>(1,0),  t.at<double>(1,0), r0.at<double>(2,0),  r1.at<double>(2,0),  t.at<double>(2,0)) ;




		cv::Mat k = (cv::Mat_<double>(4,3) << q.at<double>(0,0)  , q.at<double>(0,1)  , q.at<double>(0,2)  , q.at<double>(1,0)  , q.at<double>(1,1)  , q.at<double>(1,2)  , q.at<double>(2,0)  , q.at<double>(2,1)  , q.at<double>(2,2)  , 0 , 0, 1  );

		
	

	
		std::cout << "q" << q << std::endl;
		std::cout << "k" << k << std::endl;

		
		cv::Mat lazer_point = (cv::Mat_<double>(3,1) << lazer_points[i].x , lazer_points[i].y,1);
		std::cout << "lazer_point" << lazer_point<< std::endl;
		

		
			
		cv::Mat camera_point;
		camera_point = k*q.inv()*I_Mat.inv()*lazer_point; 


		std::cout << "camera_point" << camera_point << std::endl;


		double div = camera_point.at<double>(3,0);
		std::cout << "div : " << div << std::endl;
		
		camera_points.push_back( cv::Point3f(camera_point.at<double>(0,0)/div,camera_point.at<double>(1,0)/div,camera_point.at<double>(2,0)/div ));	
		//camera_points.push_back( cv::Point3f(camera_point.at<double>(0,0),camera_point.at<double>(1,0),camera_point.at<double>(2,0) ));	
			

		//camera_points.push_back(cv::Point3f(1,1,1));	
	}

	Vertex3D plane_points[IMAGE_SIZE]; 	

	for(int i=0;i<IMAGE_SIZE;i++){
		plane_points[i].x = camera_points[i].x;
		std::cout << "x" << camera_points[i].x  << std::endl;
		plane_points[i].y = camera_points[i].y;
		std::cout << "y" <<  camera_points[i].y << std::endl;
		plane_points[i].z = camera_points[i].z;
		std::cout << "z" << camera_points[i].z << std::endl << std::endl;
	}


	//calculate ax+by+cz+d=0 
	Plane plane;
	CreatePlaneFromPolygon(plane_points[0],plane_points[1],plane_points[2]);
	std::cout << "complete projector plane "<< std::endl;
	std::cout << "a : "<< plane.a << std::endl;
	std::cout << "b : "<< plane.b<<std::endl;
	std::cout << "c : "<< plane.c<<std::endl;
	std::cout << "d : "<< plane.d<<std::endl<<std::endl;



	std::cout << "c/a : "<< plane.c/plane.a<<std::endl;
	std::cout << "d/a : "<< plane.d/plane.a<<std::endl;
	std::cout << "b/a : "<< plane.b/plane.a<<std::endl;


	return 0;
}


