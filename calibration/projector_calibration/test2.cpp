
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
	int BGR = 1;
	cv::Mat split_imgl[3];

	//split color image
	for(int i=0;i<IMAGE_SIZE;i++){
		
		cv::Mat gimg(XI_H, XI_W, CV_8UC1);

		//cv::imshow("r",checker_image_lazer[i]);
		cv::split(checker_image_lazer[i],split_imgl);
		cv::threshold(split_imgl[BGR],gimg,thr,0,cv::THRESH_TOZERO);
		//cv::threshold(split_imgl[BGR],checker_image_lazer[i],thr,0,cv::THRESH_TOZERO);
		//cv::imshow("R",checker_image_lazer[i]);

		//cv::waitKey(0);
		int most_brightness_number[2];
		int most_brightness=0;

		for(int j=0;j<checker_image_lazer[i].rows;j++){
			for(int k=0;k<checker_image_lazer[i].cols;k++){

				unsigned char tmp_brightness = gimg.at<uchar>(j,k);
				//double tmp_brightness = checker_image_lazer[i].at<double>(j,k);

				if((int)tmp_brightness >= most_brightness ){
					most_brightness_number[0] = j;
					most_brightness_number[1] = k;
					most_brightness = tmp_brightness;
				}	
			}
		}


		std::cout << "x : " << most_brightness_number[0] << std::endl;
		cv::Point2f light_point(most_brightness_number[0],most_brightness_number[1]);

		lazer_points.push_back(light_point);	
	}	


	cv::vector <cv::Point3f> camera_points;

	for(int i=0;i<IMAGE_SIZE;i++){
		//translate points at camera axis 
	
		
		cv::Mat r3= (cv::Mat)(rotation_mat.row(2));

		//cv::Mat r3_inv = ((cv::Mat)(rotation_mat.row(2))).inv();

		std::cout << "r3" << r3 << std::endl;


		
		cv::Mat kt = (r3*translations[i]).inv() * r3;	

		std::cout << "kt" << kt << std::endl;
	
		cv::Mat diag = (cv::Mat_<double>(4,3) << 1,0,0,0,1,0,0,0,1,kt.at<double>(0,0),kt.at<double>(0,1),kt.at<double>(0,2));


		std::cout << "diag" << diag << std::endl;

		cv::Mat lazer_point = (cv::Mat_<double>(3,1) << lazer_points[i].x , lazer_points[i].y,1);


		std::cout << "lazer_point" << lazer_point<< std::endl;

			
		cv::Mat camera_point;
		camera_point = diag * I_Mat.inv() * lazer_point;


		std::cout << "camera_point" << camera_point << std::endl;

		camera_points.push_back( cv::Point3f(camera_point.at<double>(0,0),camera_point.at<double>(0,1),camera_point.at<double>(0,2) ));	
			

		//camera_points.push_back(cv::Point3f(1,1,1));	
	}

	Vertex3D plane_points[IMAGE_SIZE]; 	

	for(int i=0;i<IMAGE_SIZE;i++){
		plane_points[i].x = camera_points[i].x;
		std::cout << "x" << camera_points[i].x  << std::endl;
		plane_points[i].y = camera_points[i].y;
		std::cout << "y" <<  camera_points[i].y << std::endl;
		plane_points[i].z = camera_points[i].z;
		std::cout << "z" << camera_points[i].z << std::endl;
	}


	//calculate ax+by+cz+d=0 
	Plane plane;
	CreatePlaneFromPolygon(plane_points[0],plane_points[1],plane_points[2]);
	std::cout << "complete projector plane "<< std::endl;
	std::cout << "a : "<< plane.a << std::endl;
	std::cout << "b : "<< plane.b<<std::endl;
	std::cout << "c : "<< plane.c<<std::endl;
	std::cout << "d : "<< plane.d<<std::endl;


	return 0;
}


