//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//using namespace cv;
//
//
//int main()
//{
//	Mat srcImage, hsvImage;
//	srcImage = imread("1.jpg");
//	cvtColor(srcImage, hsvImage, CV_BGR2HSV);
//	imshow("RGB", srcImage);
//	imshow("HSV", hsvImage);
//
//	int hueBinNum = 30; // 色调的直方图数量
//	int saturationBinNum = 32; // 饱和度的直方图数量
//	int hisSize[] = { hueBinNum, saturationBinNum };
//	float hueRanges[] = { 0,180 };
//	float saturationRanges[] = { 0,255 };
//	const float* ranges[] = { hueRanges, saturationRanges };
//
//	MatND dstHist;
//
//	int channels[] = { 0,1 };
//
//	calcHist(&hsvImage, 1, channels, Mat(), dstHist, 2, hisSize, ranges, true, false);
//
//	double maxValue = 0;
//	minMaxLoc(dstHist, 0, &maxValue, 0, 0);
//	int scale = 10;
//	Mat histImage = Mat::zeros(saturationBinNum*scale, hueBinNum * 10, CV_8UC3);
//
//	for(int hue = 0; hue < hueBinNum; hue++)
//		for (int saturation = 0; saturation < saturationBinNum; saturation++)
//		{
//			float binValue = dstHist.at<float>(hue, saturation);
//			int intensity = cvRound(binValue * 255 / maxValue);
//
//			rectangle(histImage,
//				Point(hue*scale, saturation*scale),
//				Point((hue + 1)*scale - 1, (saturation + 1)*scale - 1),
//					Scalar::all(intensity),
//					CV_FILLED
//				);
//		}
//
//	imshow("H-S直方图", histImage);
//
//	waitKey(0);
//	return 0;
//}
//
//
//
//
//
//
//
