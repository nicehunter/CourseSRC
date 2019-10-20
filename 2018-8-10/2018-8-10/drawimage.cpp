//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//using namespace cv;
//
//#define WINDOW_WIFTH 600
//
//int main()
//{
//	Mat atomImage = Mat::zeros(WINDOW_WIFTH, WINDOW_WIFTH, CV_8UC3);
//	Mat rookImage = Mat::zeros(WINDOW_WIFTH, WINDOW_WIFTH, CV_8UC3);
//
//	Point start = { 0, 0 };
//	line(atomImage, start, {600, 600}, Scalar(255, 0, 0), 5);
//
//	Rect rect = Rect(100, 100, 200, 200);
//	Scalar color = Scalar(0, 255, 0);
//	rectangle(atomImage, rect, color, 2);
//
//	ellipse(atomImage, Point(atomImage.cols / 2, atomImage.rows / 2), Size(atomImage.cols / 4, atomImage.rows / 8), 0, 0, 360, color, 2, LINE_8);
//
//	imshow("ͼ", atomImage);
//	waitKey(0);
//	return 0;
//}
//
