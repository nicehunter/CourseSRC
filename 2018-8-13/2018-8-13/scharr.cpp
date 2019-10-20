//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//using namespace cv;
//
//int main()
//{
//	Mat srcImage = imread("1.jpg");
//	Mat dstImage;
//	Mat grad_x, grad_y;
//
//	imshow("ԭʼͼ", srcImage);
//	cvtColor(srcImage, srcImage, CV_BGR2GRAY);
//	blur(srcImage, srcImage, Size(3, 3));
//
//	Scharr(srcImage, grad_x, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
//	convertScaleAbs(grad_x, grad_x);
//	imshow("X", grad_x);
//	Scharr(srcImage, grad_y, CV_16S, 1, 0, 1, 0, BORDER_DEFAULT);
//	convertScaleAbs(grad_y, grad_y);
//	imshow("Y", grad_y);
//
//	addWeighted(grad_x, 0.5, grad_y, 0.5, 0, dstImage);
//	imshow("XY", dstImage);
//
//	waitKey(0);
//	return 0;
//}