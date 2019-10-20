//#include<iostream>
//#include<opencv2/ml/ml.hpp>
//#include<highgui/highgui.hpp>
//#include<opencv2/imgproc/imgproc.hpp>
//
//using namespace cv;
//using namespace ml;
//using namespace std;
//
//int main()
//{
//	Mat img = imread("digits.png", 0);
//	//imshow("数据", img);
//	int boot = 20;
//	int m = img.rows / boot;
//	int n = img.cols / boot;
//	Mat data, labels;
//
//	for (int i = 0; i < n; i++)
//	{
//		int colNum = i * boot;
//		for (int j = 0; j < m; j++)
//		{
//			int rowNum = j * boot;
//			Mat tmp;
//			img(Range(rowNum, rowNum + boot), Range(colNum, colNum + boot)).copyTo(tmp);
//			data.push_back(tmp.reshape(0, 1)); // 将图像转换为一维数组插入到data矩阵中
//			labels.push_back((int)j / 5);
//		}
//	}
//	data.convertTo(data, CV_32F);
//	int sampleNum = data.rows;
//	int trainNum = 3000;
//	Mat trianData, trainLabel;
//	trianData = data(Range(0, trainNum), Range::all());
//	trainLabel = labels(Range(0, trainNum), Range::all());
//
//	// 使用KNN
//	int k = 10;
//	Ptr<TrainData> tData = TrainData::create(trianData, ROW_SAMPLE, trainLabel); // ROW_SAMPLE表示一行一个
//	Ptr<KNearest> model = KNearest::create();
//	model->setDefaultK(k); // k个最近项
//	model->setIsClassifier(true); // true为分类， false为回归
//	model->train(tData); // 训练
//	model->save("number.xml");
//
//	double train_hr = 0, test_hr = 0;
//	Mat reponse;
//	for (int i = 0; i < sampleNum; i++)
//	{
//		Mat sample = data.row(i);
//		float r = model->predict(sample);
//		r = abs(r - labels.at<int>(i));
//		if (r <= FLT_EPSILON) // FLT_EPSILON表示最小的float浮点数，小于它，就是等于0
//			r = 1.f;
//		else
//			r = 0.f;
//		if (i < trainNum)
//			train_hr = train_hr + r;
//		else
//			test_hr = test_hr + r;
//	}
//	cout << test_hr << " " << test_hr << endl;
//	cout << "Knn模型在训练集上的准确率" << train_hr / trainNum * 100 <<
//		"%,在测试集上的准确率为" << test_hr / (data.rows - trainNum) * 100 << "%" << endl;
//
//	Mat src = imread("5.2.jpg", 0);
//	resize(src, src, Size(20, 20));
//	imshow("识别图片", src);
//	waitKey(0);
//	src = src.reshape(1, 1); // 输入图片序列化
//	Mat input;
//	input.push_back(src);
//	input.convertTo(input, CV_32FC1);
//
//	float res = model->predict(input);
//	cout << "结果是：" << res << endl;
//
//	system("pause");
//	return 0;
//}
