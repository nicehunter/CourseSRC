//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <stdio.h>
//
//using namespace cv;
//using namespace std;
//
//#define WINDOW_NAME "滑动条线性混合"
//
//Mat g_srcImage1;
//Mat g_srcImage2;
//Mat g_dstImage;
//
//const int g_nMaxAlphaValue = 100;
//int g_nAlphaValue = 70;
//double g_dAlphaValue;
//double g_dBetaValue;
//
//void on_Trackbar(int, void*)
//{
//	g_dAlphaValue = (double)g_nAlphaValue / g_nMaxAlphaValue;
//	g_dBetaValue = (1.0 - g_dAlphaValue);
//
//	addWeighted(g_srcImage1, g_dAlphaValue, g_srcImage2, g_dBetaValue, 0.0, g_dstImage);
//
//	imshow(WINDOW_NAME, g_dstImage);
//
//}
//
//
//int main()
//{
//	g_srcImage1 = imread("1.jpg");
//	g_srcImage2 = imread("2.jpg");
//
//	namedWindow(WINDOW_NAME, 1);
//
//	char TrackbarName[50];
//	sprintf_s(TrackbarName, "透明值 %d", g_nMaxAlphaValue);
//	
//	createTrackbar(TrackbarName, WINDOW_NAME, &g_nAlphaValue, g_nMaxAlphaValue, on_Trackbar);
//
//	if (waitKey(0) == 's')
//		imwrite("混合图.jpg", g_dstImage);
//	return 0;
//}
