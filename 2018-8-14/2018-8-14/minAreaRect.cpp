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
//		RotatedRect box = minAreaRect(points);
//		Point2f vertex[4];
//		box.points(vertex);
//
//		image = Scalar::all(0);
//		for (int i = 0; i < count; i++)
//			circle(image, points[i], 3, Scalar(rand() & 255, rand() & 255, rand() & 255), CV_FILLED, CV_AA);
//		
//		for (int i = 0; i < 4; i++)
//			line(image, vertex[i], vertex[(i + 1) % 4], Scalar(255, 255, 255), 2, CV_AA);
//
//		imshow("最小矩形", image);
//
//		key = (char)waitKey();
//		if (key == 27 || key == 'q')
//			break;
//	}
//	return 0;
//}