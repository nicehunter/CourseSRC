//#include<opencv2/opencv.hpp>
//#include<opencv2/imgproc/imgproc.hpp>
//
//using namespace cv;
//
//int main()
//{
//	Mat srcImage = imread("1.jpg", 0);
//	imshow("ԭʼͼ", srcImage);
//
//	Mat cornor;
//	cornerHarris(srcImage, cornor, 2, 3, 0.01);
//
//	Mat harris;
//	threshold(cornor, harris, 0.00001, 255, THRESH_BINARY);
//	imshow("�ǵ���", harris);
//
//
//	waitKey(0);
//	return 0;
//}