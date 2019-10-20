#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
	Mat image = imread("1.jpg");
	Mat out;
	Mat out1, out2, out3;

	//imshow("原图", image);
	medianBlur(image, out, 3);
	//imshow("滤波", out);
	cvtColor(out, out, CV_RGB2GRAY);
	//imshow("灰度图", out);
	threshold(out, out3, 60, 255, THRESH_BINARY); // 设置阈值 最好设为动态阈值
	threshold(out(Rect(0, 0, 400, 600)), out1, 60, 255, THRESH_BINARY); // 设置阈值 最好设为动态阈值
	threshold(out(Rect(400, 0, 400, 600)), out2, 35, 255, THRESH_BINARY); // 设置阈值 最好设为动态阈值

	out1.copyTo(out(Rect(0, 0, 400, 600)));
	out2.copyTo(out(Rect(400, 0, 400, 600)));

	imshow("阈值左", out1);
	imshow("阈值右", out2);
	imshow("单阈值", out3);
	imshow("双阈值", out);

	waitKey(0);
	return 0;
}