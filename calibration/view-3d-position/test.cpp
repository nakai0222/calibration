#include "GLSpace.h"



int main(int argc, char *argv[])
{
	

	#define IMAGESIZE 1 

	//$BE@72%G!<%?FI$_9~$_!'%Y%/%H%k(B
	cv::FileStorage fs_3d_data("distance.xml",CV_STORAGE_READ);

	cv::vector<cv::Point3d> image_points_data;

	int points_size;
	fs_3d_data["datasize"] >> points_size; 
	std::cout << "ok " << std::endl;
	
	cv::FileNode data_file = fs_3d_data["data"];
	cv::FileNodeIterator it = data_file.begin(), it_end = data_file.end();


	points_size = 300;	
	int idx = 0;
	//std::vector<uchar> lbpval;

		std::cout <<  static_cast<double>(data_file[0]["x"]) << std::endl;
		std::cout <<  static_cast<double>(data_file[1]["x"]) << std::endl;
	for(int i=0;i<IMAGESIZE;i++){

		//for(; it!= it_end; ++it,idx++){
		//(*it)["data"] >>image_points_data;
		//}
		double x,y,z;
		for(int j=0;j<points_size;j++){
		x = static_cast<double>(data_file[j]["x"]);
		//image_points_data[j].x = static_cast<double>(data_file[j]["x"]);
		y = static_cast<double>(data_file[j]["y"]);
		//image_points_data[j].y = static_cast<double>(data_file[j]["y"]);
		z = static_cast<double>(data_file[j]["z"]);
		//image_points_data[j].z = static_cast<double>(data_file[j]["z"]);
	
		image_points_data.push_back(cv::Point3d(x,y,z));

	
		//image_points_data[i].y = (double)(*it)["y"];
		//image_points_data[i].z = (double)(*it)["z"];
		//std::cout << "x=" << (double)(*it)["x"] << ", y=" << (double)(*it)["y"] <<  ", z=" << (double)(*it)["z"] 
		}
		//fs_3d_data["data"] >> image_points_data;
		//for(int j=0;i<image_points_data.size();j++)
		
		for(int i=0;i<points_size;i++){
		std::cout << "x=" << image_points_data[i].x << ", y=" << image_points_data[i].y << ", z=" << image_points_data[i].z << std::endl;
		}	
		//std::cout << "data: " << image_points_data[j].x<< std::endl;
	}
	fs_3d_data.release();



	//$B%k!<%W(B
	//for(int i=0;i<IMAGESIZE;i++){

	//$B2hA|JQ99(B
	GLSpace::d_points_draw.pull(image_points_data);	


	//$BIA2h4X?t=i4|2=(B
	GLSpace::startGL(0,argc,argv);


	/*
	while(1){
		//$B?^I=<((B
	
		glutPostRedisplay(); //glutDisplayFunc()$B$r#12s<B9T$9$k(B
		cv::waitKey(0);
	
	}
	*/	
	return 0;
}




