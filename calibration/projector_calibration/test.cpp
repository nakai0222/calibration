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

#define IMAGE_SIZE 21
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

	//read the raw and lazer image 
	checker_image = imread(img_name);
	checker_image_lazer = imread(img_name);

	/*read inside and outside parameter at camera*/
	cv::Mat I_Mat = imread(); 
	cv::Mat D_Mat = imread(); 
	cv::Mat rotation;
	cv::Mat translation;		

	cv::vector< cv::Point2f > lazer_points;
		
	cv::Mat gimgl(XI_H, XI_W, CV_8UC1);
	/*detect lazer line*/
	//split color image
	cv::split(cimgl,split_imgl);
	cv::threshold(split_imgl[BGR],gimgl,thr,0,THRESH_TOZERO);
	
	lazer_points = ();	

	
	/*match lazer line and camera local position*/

	//number of intersection point 
	cv::Size checker_pattern_size(CHESS_ROW,CHESS_COLUM);
	//image points
	cv::vector<cv::Point2f> image_point;
	//world points
	cv::vector<cv::Point3f> world_point;
	
	//find corner
	if(cv::findChessboardCorners (checker_image,checker_pattern_size, image_point , cv::CALIB_CB_ADAPTIVE_THRESH + cv::CALIB_CB_NORMALIZE_IMAGE + cv::CALIB_CB_FAST_CHECK ) )
		{
			cv::cornerSubPix(checker_image, image_point, cv::Size(11, 11), cv::Size(-1, -1),criteria);	
			cv::drawChessboardCorners( checker_image, checker_pattern_size, (cv::Mat)(image_point),true );
			cv::imshow("image",checker_image);
			cv::waitKey(20);
		}
		else
		{
			std::cout << "at least 1 corner not found."<< std::endl;
			std::cout << "number : "<<i << std::endl;	
			return -1;
		}
	}

	//registrate world points
	for(int j=0;j<checker_pattern_size.area();j++){

	world_points[i].push_back(cv::Point3f( static_cast<float>( (j%checker_pattern_size.width) *CHESS_SIZE),static_cast<float>( (j /checker_pattern_size.width) *CHESS_SIZE), 0.0) ) ;
	}

	//calculate outside parameter
	cv::solvePnP(world_point,image_point,I_mat,D_mat,rotation,translation);


	/*guess projector parameter*/
	cv::vector <cv::Point3f> camera_points;

	//translate points at camera axis 
	for(int i=0; i < checker_pattern_size.area(); i++){
	camera_points.push_back( I_mat.inv()*lazer_points[i] );	
	}

	//calculate ax+by+cz+d=0 
	Plane plane;
	plane.CreatePlaneFromPolygon();
	std::cout << "complete projector plane "<< std::endl;
	
	return 0;
}


