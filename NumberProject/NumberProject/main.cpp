#include<iostream>
#include<opencv2/ml/ml.hpp>
#include<highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace ml;
using namespace std;

int main()
{
	VideoCapture capture(0);

	Ptr<SVM> mysvm = StatModel::load<SVM>("number_svm.xml");
	Ptr<KNearest> model = StatModel::load<KNearest>("number.xml");

	while (1)
	{
		Mat src_test;
		capture >> src_test;
		//src_test = imread("number.jpg");

		//imshow("原图", src_test);
		Mat gray_test, thres_test, blur_test;
		cvtColor(src_test, gray_test, COLOR_BGR2GRAY);
		GaussianBlur(gray_test, blur_test, Size(3, 3), 3, 3);
		adaptiveThreshold(blur_test, thres_test, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 151, 10);
		Mat morph_test, predict_mat;
		Mat kernerl = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
		morphologyEx(thres_test, morph_test, MORPH_OPEN, kernerl, Point(-1, -1));
		//imshow("Bin", morph_test);
		//waitKey(0);
		vector<vector<Point>> contours_test;
		vector<Vec4i> hiearachy_test;
		findContours(morph_test, contours_test, hiearachy_test, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		for (int i = 0; i < contours_test.size(); i++)
		{
			Rect minrect_test = boundingRect(contours_test[i]);
			float area_test = contourArea(contours_test[i]);
			float ckbi_test = minrect_test.width / minrect_test.height;
			//if (ckbi_test < 4 && ckbi_test > 50)
			//{
				rectangle(src_test, minrect_test, Scalar(0, 255, 0), 1, 8);
				Rect ROI_test = minrect_test;
				Mat ROI_img_test = morph_test(ROI_test);

				resize(ROI_img_test, ROI_img_test, Size(20, 20));
				//imshow("识别图片", ROI_img_test);
				//waitKey(0);
				ROI_img_test = ROI_img_test.reshape(1, 1); // 输入图片序列化
				Mat input;
				input.push_back(ROI_img_test);
				input.convertTo(input, CV_32FC1);

				float res = mysvm->predict(input);

				stringstream stream;
				stream << res;
				string str;
				stream >> str;
				putText(src_test, str, ROI_test.tl(), FONT_HERSHEY_PLAIN, 1, Scalar(255, 0, 0), 1, 0);
			//}
		}
		imshow("test", src_test);
		waitKey(1);
	}

	return 0;
}