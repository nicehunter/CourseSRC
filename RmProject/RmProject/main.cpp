/*************************************************
Copyright:     
Author:        Lcy
Date:          2019-8-11
Description:   主函数
Version:       1.0.1
**************************************************/
#include <opencv2/opencv.hpp>
#include "armor_plate.h"

using namespace cv;
using namespace std;

#define ARMOR_DEVICE 0

bool CamerRead(ArmorPlate& armor_param);

/*************************************************
Function:       main
Description:    主函数
Input:
Output:
Return:
Others:
*************************************************/
int main()
{
	ArmorPlate armor;
	//armor.CamaraInit(ARMOR_DEVICE); // 视觉辅助摄像头初始化

	VideoCapture capture("1.wmv"); // 加载本地素材
	armor.capture_armor_ = capture;

	while (1)
	{
		if (!CamerRead(armor))
			continue;
		armor.AutoShoot();
	}	
	//waitKey(0);
	return 0;
}


bool CamerRead(ArmorPlate& armor_param)
{
	armor_param.capture_armor_.read(armor_param.armor_image_);
	if (!armor_param.armor_image_.data)
	{
		cout << "视觉辅助摄像头没有读取到图像！" << endl;
		armor_param.CamaraInit(ARMOR_DEVICE); // 视觉辅助摄像头初始化
		return false;
	}
	else
		return true;
}



