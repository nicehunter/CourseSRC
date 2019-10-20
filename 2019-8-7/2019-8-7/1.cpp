#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main()
{
	Mat srcImage = imread("ͼƬ/1.jpg");
	imshow("����Ϧţ��֯Ů���ͼ��", srcImage);

	Mat edge, grayImage;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
	blur(grayImage, edge, Size(3, 3));

	//Canny(edge, edge, 1, 1, 3);
	imshow("Ч��ͼ", edge);

	waitKey(0);

	return 0;
}


