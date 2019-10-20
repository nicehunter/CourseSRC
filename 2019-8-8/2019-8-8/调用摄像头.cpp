//#include <opencv2/opencv.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//using namespace cv;
//
//int main()
//{
//	VideoCapture capture(0);
//
//	capture.set(CV_CAP_PROP_EXPOSURE, -20);
//
//	while (1)
//	{
//		Mat frame;
//		capture >> frame;
//
//		Mat edge, grayImage;
//		cvtColor(frame, grayImage, CV_BGR2GRAY);
//		blur(grayImage, edge, Size(3, 3));
//
//		Canny(edge, edge, 1, 1, 3);
//
//		if (frame.empty())
//			break;
//
//		imshow("°æ∂¡»° ”∆µ°ø", edge);
//		
//		if (waitKey(1) == 'c')
//			break;
//	}
//
//	return 0;
//}