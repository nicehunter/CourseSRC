//#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//using namespace cv;
//using namespace std;
//
//int main()
//{
//	Mat srcImage = imread("1.jpg");
//	imshow("ԭʼͼ", srcImage);
//
//	cvtColor(srcImage, srcImage, CV_BGR2GRAY);
//	imshow("�Ҷ�ͼ", srcImage);
//	blur(srcImage, srcImage, Size(3, 3));
//	Canny(srcImage, srcImage, 200, 100, 3);
//	imshow("Ч��ͼ", srcImage);
//
//	vector<Vec2f> lines; // ����һ��ʸ���ṹlines�����ڴ��ʸ���ϼ�
//	HoughLines(srcImage, lines, 1, CV_PI/180, 150, 0, 0);
//
//	for (size_t i = 0; i < lines.size(); i++)
//	{
//		float rho = lines[i][0], theta = lines[i][1];
//		Point pt1, pt2;
//		double a = cos(theta), b = sin(theta);
//		double x0 = a * rho, y0 = b * rho;
//		pt1.x = cvRound(x0 + 1000*(-b));
//		pt1.y = cvRound(y0 + 1000 * (a));
//		pt2.x = cvRound(x0 + 1000 * (-b));
//		pt2.y = cvRound(y0 + 1000 * (a));
//		line(srcImage, pt1, pt2, Scalar(255, 0, 0), 5, CV_AA);
//	}
//	//line(srcImage, { 0, 0 }, { 100, 100 }, Scalar(255, 0, 0), 5, CV_AA);
//	imshow("�任ͼ", srcImage);
//	waitKey(0);
//	return 0;
//}
//
//
//
//
//
