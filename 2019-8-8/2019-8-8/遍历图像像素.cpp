#include<iostream>
#include<opencv2/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void colorReduce0(Mat &image, int div = 64)
{
	int nl = image.rows;
	int nc = image.cols*image.channels(); // ÿ��Ԫ�ص���Ԫ������

	for (int j = 0; j < nl; j++)
	{
		uchar* data = image.ptr<uchar>(j);

		for (int i = 0; i < nc; i++)
		{
			// ��ʼ����ÿ������
			data[i] = data[i] / div * div + div / 2;
			// ��������
		} // ������
	}
}



int main()
{
	Mat srcImage = imread("ͼƬ/1.jpg");

	imshow("����Ϧţ��֯Ů���ͼ��", srcImage);

	colorReduce0(srcImage);

	imshow("�����", srcImage);

	cout << srcImage.channels() << endl;
	cout << srcImage.rows << endl;
	cout << srcImage.cols << endl;

	waitKey(0);


	return 0;
}

