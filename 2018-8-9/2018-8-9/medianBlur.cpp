//#include<opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include<opencv2/imgproc/imgproc.hpp>
//
//using namespace cv;
//
//void Salt(Mat &image, int num) 
//{
//	if (!image.data) return;//��ֹ�����ͼ	
//	int i, j;	
//	for (int x = 0; x < num; ++x) 
//	{		
//		i = rand() % image.rows;
//		j = rand() % image.cols;		
//	
//		image.at<Vec3b>(i, j)[0] = 255;		
//		image.at<Vec3b>(i, j)[1] = 255;		
//		image.at<Vec3b>(i, j)[2] = 255;	}
//}
//
//
//int main()
//{
//	Mat image = imread("1.jpg");
//	Mat out;
//
//	Salt(image, 1000);
//
//	imshow("ԭͼ", image);
//
//	medianBlur(image, out, 3);
//	imshow("��ֵ�˲�", out);
//
//	waitKey(0);
//	return 0;
//}

