#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

#include "GLSpace.h"

//描画関数のオーバーライド
void Display(cv::vector<cv::Point3d> 3d_points);

class 3dPoints{
	public:
	cv::vector<cv::Point3d> data;
};


int main(int argc, char *argv[])
{


	//点群データ読み込み：ベクトル
	cv::FileStorage fs_3d_data("3d-data.xml",CV_STORAGE_READ);


	static int imageSize;
	fs_3d_data["image_size"] >>image_size; 
	3dPoints 3d_points[image_size];

	
	for(int i=0;i<image_size;i++){
	fs_3d_data["data"] >>3d_points; 
	std::cout << "data: " << 3d_points[0].data[0].x<< std::endl;
	}	

	//描画関数初期化
	GLSpace::startGL();

	//ループ
	for(int i=0;i<image_size;i++){
	
	//図表示
	Display(3d_points[i]);
	cv::waitKey(30);
	}

    	return 0;
}


void Display(cv::vector<cv::Point3d> 3d_points) {

		glClearColor(0.0,0.0,0.0,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去
		//モデルビュー変換行列の設定--------------------------
		glMatrixMode(GL_MODELVIEW);//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
		glLoadIdentity();//行列の初期化
		glViewport(0, 0, WindowWidth, WindowHeight);
		//----------------------------------------------
		glPushMatrix();
		//視点の設定------------------------------
		gluLookAt(
				-100.0, 80.0, -100.0, // 視点の位置x,y,z;
				0.0, 0.0,  camera_z_pos,   // 視界の中心位置の参照点座標x,y,z
				0.0, 0.0, -1.0);  //視界の上方向のベクトルx,y,z
		gluLookAt(
				0.0, dist, 0.0, // 視点の位置x,y,z;
				0.0, 0.0,  camera_z_pos,   // 視界の中心位置の参照点座標x,y,z
				0.0, 0.0, -1.0);  //視界の上方向のベクトルx,y,z
		//----------------------------------------
		////回転///////////////////////////////////////////////
		glMultMatrixd(rt);
		///////////////////////////////////////////////////////
		glPointSize(plot);
		glBegin(GL_POINTS);
		
	
		glColor3d(0.0,0.0,0.0);
	

		   glVertex3d(0.0,0.0,0.0);//原点のノイズ除去
		   for(int i=0;i<3d_points.size();i++){
		   glColor3d(1.0,1.0,0.0);
		   //hsv2rgbColor(z_global[i]);
		   hsv2rgbColor(3d_points[i].z);
		   //glVertex3d(x_global[i]/l_scale,y_global[i]/l_scale,z_global[i]/z_scale-z_slide);
		   glVertex3d(3d_points[i].x/l_scale,3d_points[i].y/l_scale,3d_points[i].z/z_scale-z_slide);
		   }
		 

		glEnd();
		Ground();
		glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可

}



