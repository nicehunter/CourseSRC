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
//	//imshow("����", img);
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
//			data.push_back(tmp.reshape(0, 1)); // ��ͼ��ת��Ϊһά������뵽data������
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
//	// ʹ��KNN
//	int k = 10;
//	Ptr<TrainData> tData = TrainData::create(trianData, ROW_SAMPLE, trainLabel); // ROW_SAMPLE��ʾһ��һ��
//	Ptr<KNearest> model = KNearest::create();
//	model->setDefaultK(k); // k�������
//	model->setIsClassifier(true); // trueΪ���࣬ falseΪ�ع�
//	model->train(tData); // ѵ��
//	model->save("number.xml");
//
//	double train_hr = 0, test_hr = 0;
//	Mat reponse;
//	for (int i = 0; i < sampleNum; i++)
//	{
//		Mat sample = data.row(i);
//		float r = model->predict(sample);
//		r = abs(r - labels.at<int>(i));
//		if (r <= FLT_EPSILON) // FLT_EPSILON��ʾ��С��float��������С���������ǵ���0
//			r = 1.f;
//		else
//			r = 0.f;
//		if (i < trainNum)
//			train_hr = train_hr + r;
//		else
//			test_hr = test_hr + r;
//	}
//	cout << test_hr << " " << test_hr << endl;
//	cout << "Knnģ����ѵ�����ϵ�׼ȷ��" << train_hr / trainNum * 100 <<
//		"%,�ڲ��Լ��ϵ�׼ȷ��Ϊ" << test_hr / (data.rows - trainNum) * 100 << "%" << endl;
//
//	Mat src = imread("5.2.jpg", 0);
//	resize(src, src, Size(20, 20));
//	imshow("ʶ��ͼƬ", src);
//	waitKey(0);
//	src = src.reshape(1, 1); // ����ͼƬ���л�
//	Mat input;
//	input.push_back(src);
//	input.convertTo(input, CV_32FC1);
//
//	float res = model->predict(input);
//	cout << "����ǣ�" << res << endl;
//
//	system("pause");
//	return 0;
//}
