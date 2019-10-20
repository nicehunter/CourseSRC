#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

Mat g_srcImage1;
Mat g_srcImage2;
Mat g_dstImage;
Mat ROI_Image;

int main()
{
	g_srcImage1 = imread("1.jpg");
	g_srcImage2 = imread("2.jpg");

	imshow("ԭͼ1", g_srcImage1);
	imshow("ԭͼ2", g_srcImage2);

	g_dstImage = g_srcImage1;
	//ROI_Image.copyTo(g_dstImage);

	//ROI_Image = g_srcImage2(Rect(0, 0, 220, 440));
	ROI_Image = g_srcImage2(Range(0, 440), Range(220, 440));
	ROI_Image.copyTo(g_dstImage(Range(0, 440), Range(220, 440)));

	imshow("���", g_dstImage);

	imshow("��ͼ1", g_srcImage1);
	imshow("��ͼ2", g_srcImage2);

	waitKey(0);
	return 0;
}
