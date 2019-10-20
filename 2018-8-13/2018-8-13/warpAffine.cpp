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
//	Mat dstImage_warp;
//
//	Point2f src[3];
//	Point2f dst[3];
//	Mat rot(2, 3, CV_32FC1);
//	Mat warp(2, 3, CV_32FC1);
//
//	imshow("ԭʼͼ", srcImage);
//
//	dstImage = Mat::zeros(srcImage.rows, srcImage.cols, srcImage.type());
//
//	src[0] = Point2f(0, 0);
//	src[1] = Point2f(static_cast<float>(srcImage.cols - 1), 0);
//	src[2] = Point2f(0, static_cast<float>(srcImage.rows - 1));
//
//	dst[0] = Point2f(static_cast<float>(srcImage.cols*0.0), static_cast<float>(srcImage.cols*0.33));
//	dst[1] = Point2f(static_cast<float>(srcImage.cols*0.65), static_cast<float>(srcImage.cols*0.35));
//	dst[2] = Point2f(static_cast<float>(srcImage.cols*0.15), static_cast<float>(srcImage.cols*0.6));
//
//	warp = getAffineTransform(src, dst);
//
//	warpAffine(srcImage, dstImage, warp, dstImage.size());
//
//	imshow("�任", dstImage);
//
//	waitKey(0);
//	return 0;
//}