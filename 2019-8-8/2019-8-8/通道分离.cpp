//#include <opencv2/opencv.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//using namespace cv;
//
//int main()
//{
//	Mat srcImage = imread("图片/1.jpg");
//	imshow("【七夕牛郎织女相会图】", srcImage);
//
//	std::vector<Mat> channels;
//	Mat blueChannels;
//
//	split(srcImage, channels);
//	blueChannels = channels.at(0);
//
//	Mat edge, grayImage;
//	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
//	blur(grayImage, edge, Size(3, 3));
//
//	//Canny(edge, edge, 1, 1, 3);
//	imshow("效果图", edge);
//	imshow("蓝色通道", blueChannels);
//	
//
//	waitKey(0);
//
//	return 0;
//}


