//#include<opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include<opencv2/imgproc/imgproc.hpp>
//
//using namespace cv;
//
//int main()
//{
//	Mat image = imread("2.jpg");
//	Mat out;
//
//	imshow("原图", image);
//
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); // 构造核函数
//	dilate(image, out, element, Point(-1, -1), 10);
//	imshow("滤波", out);
//
//	waitKey(0);
//	return 0;
//}