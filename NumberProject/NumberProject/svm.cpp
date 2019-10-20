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
//	int trainNum = 1000;
//	Mat trianData, trainLabel;
//	trianData = data(Range(0, trainNum), Range::all());
//	trainLabel = labels(Range(0, trainNum), Range::all());
//	
//	// 创建SVM模型
//	Ptr<SVM> SVM_params = SVM::create();
//	SVM_params->setType(SVM::C_SVC); // C_SVC用于分类，C_SVR用于回归
//	SVM_params->setKernel(SVM::LINEAR); // LINEAR线性核函数，SIGMOID为高斯函数
//
//	SVM_params->setDegree(0); // 核函数中的参数degree，针对多项式核函数
//	SVM_params->setGamma(1); // 核函数中的参数gamma，针对多项式/RBF/SIGMOID函数
//	SVM_params->setCoef0(0); // 和函数中的参数，针对多项式/SIGMOID函数
//	SVM_params->setC(1); // SVM最优化问题参数，设置C_SVC, EPS_SVR和NU_SVR的参数
//	SVM_params->setNu(0); // SVM最优化问题参数，设置NU_SVC，ONE_ClASS,NU_SVR的参数
//	SVM_params->setP(0); // SVM最优化问题参数，设置 EPS_SVR中损失函数P的值
//
//	SVM_params->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 0.01));
//
//	Ptr<TrainData> tData = TrainData::create(trianData, ROW_SAMPLE, trainLabel); // ROW_SAMPLE表示一行一个
//
//	SVM_params->train(tData); // 训练
//	SVM_params->save("number_svm.xml");
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
//	float res = SVM_params->predict(input);
//	cout << "结果是：" << res << endl;
//
//	system("pause");
//	return 0;
//}
