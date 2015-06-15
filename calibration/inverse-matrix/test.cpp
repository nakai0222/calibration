

#include <opencv2/core/core.hpp>
#include <iostream>



int main( int argc, char* argv[])
{

	// 逆行列 (LU分解)
	cv::Mat m1 = (cv::Mat_<double>(3,3) << 10, -9, -12, 7, -12, 11, -10, 10, 3);  
	std::cout << "m1=" << m1 << std::endl << std::endl;
	std::cout << "inverse matrix (LU decompression):" << std::endl << std::endl;
	std::cout << "m1^-1=" << m1.inv() << std::endl << std::endl;
	std::cout << "m1*m1^-1=" << m1*m1.inv() << std::endl << std::endl;

	// 逆行列 (SVD)
	std::cout << "inverse matrix (SVD):" << std::endl << std::endl;
	std::cout << "m1^-1=" << m1.inv(cv::DECOMP_SVD) << std::endl << std::endl;
	std::cout << "m1*m1^-1=" << m1*m1.inv(cv::DECOMP_SVD) << std::endl << std::endl;

	// 擬似逆行列
	cv::Mat m2 = (cv::Mat_<double>(2,3) << 10, -9, -12, 7, -12, 11);
	std::cout << "m2=" << m2 << std::endl << std::endl;
	std::cout << "m2^-1=" << m2.inv(cv::DECOMP_SVD) << std::endl << std::endl;
	std::cout << "m2*m2^-1=" << m2*m2.inv(cv::DECOMP_SVD) << std::endl << std::endl;

	return 0;
}


