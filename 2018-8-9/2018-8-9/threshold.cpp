#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
	Mat image = imread("1.jpg");
	Mat out;
	Mat out1, out2, out3;

	//imshow("ԭͼ", image);
	medianBlur(image, out, 3);
	//imshow("�˲�", out);
	cvtColor(out, out, CV_RGB2GRAY);
	//imshow("�Ҷ�ͼ", out);
	threshold(out, out3, 60, 255, THRESH_BINARY); // ������ֵ �����Ϊ��̬��ֵ
	threshold(out(Rect(0, 0, 400, 600)), out1, 60, 255, THRESH_BINARY); // ������ֵ �����Ϊ��̬��ֵ
	threshold(out(Rect(400, 0, 400, 600)), out2, 35, 255, THRESH_BINARY); // ������ֵ �����Ϊ��̬��ֵ

	out1.copyTo(out(Rect(0, 0, 400, 600)));
	out2.copyTo(out(Rect(400, 0, 400, 600)));

	imshow("��ֵ��", out1);
	imshow("��ֵ��", out2);
	imshow("����ֵ", out3);
	imshow("˫��ֵ", out);

	waitKey(0);
	return 0;
}