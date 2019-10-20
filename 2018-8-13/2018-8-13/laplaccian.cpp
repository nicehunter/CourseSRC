//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//using namespace cv;
//
//int main()
//{
//	Mat srcImage = imread("1.jpg");
//	Mat dst, dstImage;
//	imshow("原始图", srcImage);
//
//	cvtColor(srcImage, srcImage, CV_BGR2GRAY);
//	imshow("灰度图", srcImage);
//	blur(srcImage, srcImage, Size(3, 3));
//
//	Laplacian(srcImage, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
//	convertScaleAbs(dst, dstImage);
//	imshow("效果图", dstImage);
//
//	waitKey(0);
//	return 0;
//}
//
//
