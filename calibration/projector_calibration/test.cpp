/*
   6/6/2015
   7/6/2015
   line lazer calibration
 */

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
	cv::vector<cv::Mat> checker_image_lazer;

	for(int i=0;i<IMAGE_SIZE;i++)
	{

		//load images
		std::stringstream ss;
		std::string image_name;
		ss <<  i << ".png";
		image_name = ss.str();	
		checker_image.push_back( cv::imread(image_name.c_str(),0) );

		ss << i << "l.png";
			image_name = ss.str();	
		checker_image_lazer.push_back( cv::imread(image_name.c_str(),0) );

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

	fs["intrinsicMat"] >> I_Mat;
	fs["distCoeffs"] >> D_Mat;
		 
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
		cv::solvePnP(world_points[i],image_points[i],I_Mat,D_Mat,rotations[i],translations[i]);
	}

	/*
	Vec3b intensity = img.at<Vec3b>(x,y);
	unchar blue = intensity.val[0];
	unchar green = intensity.val[1];
	unchar red = intensity.val[2];
	*/
	
	


	//calculate lazer points
	cv::vector<cv::Point2f> lazer_points(IMAGE_SIZE);
   	int thr = 200;
	int BGR = 1;
	 cv::Mat split_imgl[3];
	//split color image
	for(int i=0;i<IMAGE_SIZE;i++){
	
		cv::split(checker_image_lazer[i],split_imgl);
		cv::threshold(split_imgl[BGR],checker_image_lazer[i],thr,0,THRESH_TOZERO);

		int most_brightness_number[2];
		double most_brightness=0;

		for(int j=0;j<checker_image_lazer[i].cols;j++){
			for(int k=0;k<checker_image_lazer[i].rows;k++){

				double tmp_brightness = checker_image_lazer[i].at(j,k);

				if(tmp_brightness > most_brightness ){
					most_brightness_number[0] = j;
					most_brightness_number[1] = k;
					most_brightness = tmp_brightness;
				}	
			}
		}
		cv::Point2f light_point(most_brightness_number[0],most_brightness_number[1]);

		lazer_points.push_back(light_point);	
	}	

	cv::vector <cv::Point3f> camera_points;

	for(int i=0;i<IMAGE_SIZE;i++){
		//translate points at camera axis 
		camera_points.push_back( I_mat.inv()*static_cast<cv::Mat>()lazer_points[i]);	
	}

	Vertex3D plane_points[3]; 	

	//calculate ax+by+cz+d=0 
	Plane plane;
	plane.CreatePlaneFromPolygon(plane_points[0],plane_points[1],plane_points[2]);
	std::cout << "complete projector plane "<< std::endla;
	std::cout << "a : "<< plane.a << std::endl;
	std::cout << "b : "<< plane.b<<std::endl;
	std::cout << "c : "<< plane.c<<std::endl;
	std::cout << "d : "<< plane.d<<std::endl;
	
	
	return 0;
}


