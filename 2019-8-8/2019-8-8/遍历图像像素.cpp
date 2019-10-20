#include<iostream>
#include<opencv2/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

void colorReduce0(Mat &image, int div = 64)
{
	int nl = image.rows;
	int nc = image.cols*image.channels(); // 每行元素的总元素数量

	for (int j = 0; j < nl; j++)
	{
		uchar* data = image.ptr<uchar>(j);

		for (int i = 0; i < nc; i++)
		{
			// 开始处理每个像素
			data[i] = data[i] / div * div + div / 2;
			// 结束处理
		} // 处理单行
	}
}



int main()
{
	Mat srcImage = imread("图片/1.jpg");

	imshow("【七夕牛郎织女相会图】", srcImage);

	colorReduce0(srcImage);

	imshow("处理后", srcImage);

	cout << srcImage.channels() << endl;
	cout << srcImage.rows << endl;
	cout << srcImage.cols << endl;

	waitKey(0);


	return 0;
}

