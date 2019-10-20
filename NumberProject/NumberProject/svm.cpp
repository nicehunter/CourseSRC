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
//	int trainNum = 1000;
//	Mat trianData, trainLabel;
//	trianData = data(Range(0, trainNum), Range::all());
//	trainLabel = labels(Range(0, trainNum), Range::all());
//	
//	// ����SVMģ��
//	Ptr<SVM> SVM_params = SVM::create();
//	SVM_params->setType(SVM::C_SVC); // C_SVC���ڷ��࣬C_SVR���ڻع�
//	SVM_params->setKernel(SVM::LINEAR); // LINEAR���Ժ˺�����SIGMOIDΪ��˹����
//
//	SVM_params->setDegree(0); // �˺����еĲ���degree����Զ���ʽ�˺���
//	SVM_params->setGamma(1); // �˺����еĲ���gamma����Զ���ʽ/RBF/SIGMOID����
//	SVM_params->setCoef0(0); // �ͺ����еĲ�������Զ���ʽ/SIGMOID����
//	SVM_params->setC(1); // SVM���Ż��������������C_SVC, EPS_SVR��NU_SVR�Ĳ���
//	SVM_params->setNu(0); // SVM���Ż��������������NU_SVC��ONE_ClASS,NU_SVR�Ĳ���
//	SVM_params->setP(0); // SVM���Ż�������������� EPS_SVR����ʧ����P��ֵ
//
//	SVM_params->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 0.01));
//
//	Ptr<TrainData> tData = TrainData::create(trianData, ROW_SAMPLE, trainLabel); // ROW_SAMPLE��ʾһ��һ��
//
//	SVM_params->train(tData); // ѵ��
//	SVM_params->save("number_svm.xml");
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
//	float res = SVM_params->predict(input);
//	cout << "����ǣ�" << res << endl;
//
//	system("pause");
//	return 0;
//}
