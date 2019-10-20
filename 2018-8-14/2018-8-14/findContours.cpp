//#include <opencv2/opencv.hpp>
//#include <opencv2/core.hpp>
//#include <opencv/highgui.h>
//#include <opencv2/imgproc.hpp>
//#include <opencv2/features2d.hpp>
//
//using namespace cv;
//using namespace std;
//
//int main()
//{
//	Mat srcImage = imread("1.jpg", 0);
//	imshow("原始图", srcImage);
//
//	Mat dstImage = Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC3);
//
//	srcImage = srcImage > 30; // srcImage取阈值大于119的部分
//	imshow("取阈值后的原始图", srcImage);
//
//	vector<vector<Point>> contours; // 轮廓
//	vector<Vec4i> hierarchy; // 层次结构
//
//	findContours(srcImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
//
//	int index = 0;
//	for (; index >= 0; index = hierarchy[index][0])
//	{
//		Scalar color(rand()&255, rand()&255, rand()&255);
//		drawContours(dstImage, contours, index, color, CV_FILLED, 8, hierarchy);
//	}
//	imshow("轮廓图", dstImage);
//
//	waitKey(0);
//	return 0;
//}