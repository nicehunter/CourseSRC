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
//	imshow("ԭͼ", image);
//
//	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3)); // ����˺���
//	erode(image, out, element, Point(-1, -1), 20);
//    dilate(out, out, element, Point(-1, -1), 20);
//	
//	
//	imshow("�˲�", out);
//
//	waitKey(0);
//	return 0;
//}