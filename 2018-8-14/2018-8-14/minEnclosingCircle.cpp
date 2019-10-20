//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//
//using namespace cv;
//using namespace std;
//
//int main()
//{
//	Mat image(600, 600, CV_8UC3);
//	RNG& rng = theRNG();
//
//	while (1)
//	{
//		char key;
//		int count = (unsigned)rng % 100 + 3; // 随机生成点的数量
//		vector<Point> points; 
//
//		for (int i = 0; i < count; i++)
//		{
//			Point point;
//			point.x = rng.uniform(image.cols / 4, image.cols * 3 / 4);
//			point.y = rng.uniform(image.rows / 4, image.rows * 3 / 4);
//
//			points.push_back(point);
//		}
//
//		Point2f center;
//		float radius = 0;
//		minEnclosingCircle(points, center, radius);
//
//
//		image = Scalar::all(0);
//		for (int i = 0; i < count; i++)
//			circle(image, points[i], 3, Scalar(rand() & 255, rand() & 255, rand() & 255), CV_FILLED, CV_AA);
//		
//		circle(image, center, cvRound(radius), Scalar(255, 255, 255), 2, CV_AA);
//
//		imshow("圆形", image);
//
//		key = (char)waitKey();
//		if (key == 27 || key == 'q')
//			break;
//	}
//	return 0;
//}