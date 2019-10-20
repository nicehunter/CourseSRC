/*************************************************
Copyright:
Author:        Lcy
Date:          2019-8-11
Description:   装甲识别
Version:       1.0.1
**************************************************/
#include <opencv2/opencv.hpp>
#include "armor_plate.h"

using namespace cv;
using namespace std;

/*************************************************
Function:       ArmorPlate
Description:    构造函数
Input:
Output:
Return:
Others:
*************************************************/
ArmorPlate::ArmorPlate()
{
	our_team_ = TEAMBLUE;
}
/*************************************************
Function:       CameraInit
Description:    摄像头初始化
Input:          device
Output:
Return:         false or true
Others:         bool
*************************************************/
bool ArmorPlate::CamaraInit(int device)
{
	capture_armor_.open(device);
	if (!capture_armor_.isOpened())
	{
		cout << "视觉辅助摄像头打开失败" << endl;
		return false;
	}
	else
		return true;
}
/*************************************************
Function:       ImgPreprocess
Description:    图像预处理
Input:          src
Output:         dst
Return:         
Others:         RGB
*************************************************/
void ArmorPlate::ImgPreprocess(const cv::Mat& src, cv::Mat& dst)
{
	std::vector<Mat> img_channels; // 通道
	split(src, img_channels); // 通道分离

	if (our_team_) // 我方红方 对方蓝方
	{
		Mat img_blue_channels;
		img_blue_channels = img_channels.at(0); // 蓝色通道
		imshow("blue_channels", img_blue_channels);
		img_blue_channels = img_blue_channels - img_channels.at(1)*0.4 - img_channels.at(2)*0.4; // 去除白色影响
		imshow("blue", img_blue_channels);
	}
	else // 我方蓝方 对方红方
	{
		Mat img_red_channels;
		img_red_channels = img_channels.at(2); // 红色通道
		//imshow("img_red_channels", img_red_channels);
		img_red_channels = img_red_channels - img_channels.at(0)*0.4 - img_channels.at(1)*0.4; // 去除白色影响
		//img_red_channels = img_red_channels - img_channels.at(0); // 官方思路
		blur(img_red_channels, img_red_channels, Size(3, 3));
		img_red_channels = img_red_channels * 3;
		//imshow("red_channels", img_red_channels);
		double maxValue_gary;
		minMaxLoc(img_red_channels, 0, &maxValue_gary, 0, 0); // 获取最大灰度值
		Mat imgBin;
		threshold(img_red_channels, imgBin, maxValue_gary*0.7, 255, THRESH_BINARY);
		//imshow("二值化", imgBin);
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(imgBin, dst, element, Point(-1, -1), 3); // 膨胀3次
		erode(dst, dst, element, Point(-1, -1), 4); // 腐蚀4次
		dilate(imgBin, dst, element, Point(-1, -1), 3); // 膨胀3次
		//imshow("RGB预处理", dst);
	}
}
/*************************************************
Function:       FindArmor
Description:    辅助射击
Input:          src  dst
Output:         视野目标 击打目标
Return:
Others:       
*************************************************/
void  ArmorPlate::FindArmor(Mat& src, Mat& dst, vector<RotatedRect>& all, RotatedRect& target) // 定位装甲板
{
	all.clear();
	target.center.x = 0;
	target.center.y = 0;
	target.size.width = 0;
	target.size.height = 0;
	target.angle = 0;

	RotatedRect s, s_fitEllipse, s_minAreaRect; // 用于筛选轮廓
	vector<RotatedRect> ss; 
	ss.clear();

	vector<vector<Point>> contours; // 轮廓
	vector<Vec4i> hierarchy; // 层次

	findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE); // 寻找轮廓
	Mat drawing = Mat::zeros(dst.size(), CV_8UC3);
	RNG g_rng(12345);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point()); // 画出所有轮廓
		imshow("轮廓", drawing);

		if (contours[i].size() >= 10)
		{
			s_fitEllipse = fitEllipse(contours[i]); // 椭圆拟合
			s_minAreaRect = minAreaRect(contours[i]);

			s.angle = s_fitEllipse.angle;
			s.center = s_fitEllipse.center;
			if (s_minAreaRect.size.width > s_minAreaRect.size.height)
			{
				s.size.height = s_minAreaRect.size.width;
				s.size.width = s_minAreaRect.size.height;
			}
			else
			{
				s.size.height = s_minAreaRect.size.height;
				s.size.width = s_minAreaRect.size.width;
			}
			if ((s.size.width / s.size.height) > L_WH_RAT)
				continue;
			int x = s.center.x - s.size.width;
			if (x < 0)
				continue;
			int y = s.center.y - s.size.height;
			if (y < 0)
				continue;
			int w = s.size.width + s.size.width;
			if (w > dst.cols - x)
				continue;
			int h = s.size.height + s.size.height;
			if (h > dst.rows - y)
				continue;

			if ((s.angle < 45 || s.angle > 135) && (s.size.height > 10) && (s.size.height < 150))
				ss.push_back(s);
		}
	}

	// 判别装甲
	vector<RotatedRect> armors;
	vector<ArmorBuilded> armor_SECOND;
	ArmorBuilded armor_FIRST;
	static float armor_center_x;
	static float armor_center_y;

	armors.clear();
	armor_SECOND.clear();
	int nL, nW;

	if (ss.size() < 2) // 旋转矩形小于两个，直接返回
	{
		target.center.x = 0;
		target.center.y = 0;
		target.size.width = 0;
		target.size.height = 0;
		target.angle = 0;
		all.push_back(target);
		armor_center_x = 0;
		armor_center_y = 0;
	}
	else
	{
		for (int i = 0; i < ss.size() - 1; i++)
		{
			for (int j = i + 1; j < ss.size(); j++)
			{
				double height_diff = abs(ss[i].size.height - ss[j].size.height); // 高度差
				double height_sum = ss[i].size.height + ss[j].size.height; // 高度和
				double width_diff = abs(ss[i].size.width - ss[j].size.width); // 宽度差
				double width_sum = ss[i].size.width + ss[j].size.width; // 宽度和
				double angle_diff = fabs(ss[i].angle - ss[j].angle); // 角度差
				double Y_diff = abs(ss[i].center.y - ss[j].center.y); // 纵坐标差值
				double X_diff = abs(ss[i].center.x - ss[j].center.x); // 横坐标差值
				double MH_diff = (min(ss[i].size.height, ss[j].size.height)) * 2 / 3; // 高度差限幅
				double height_max = max(ss[i].size.height, ss[j].size.height); // 最大高度

				if (Y_diff < MH_diff && X_diff < height_max * 4 &&
					(angle_diff < T_ANGLE_THRE || 180 - angle_diff < T_ANGLE_THRE180) &&
					height_diff / height_sum < T_HIGH_RAT &&
					width_diff / width_sum < T_WHIDTH_RAT)
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2);
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2);
					armor_FIRST.armorS.angle = ((ss[i].angle + ss[j].angle) / 2);
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2;
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x)
						+ (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y));
					if (nL < nW)
					{
						armor_FIRST.armorS.size.height = nL;
						armor_FIRST.armorS.size.width = nW;
					}
					else
					{
						armor_FIRST.armorS.size.height = nW;
						armor_FIRST.armorS.size.width = nL;
					}
					if (Y_diff < nW / 3)
					{
						armor_FIRST.build1_No = i;
						armor_FIRST.build2_No = j;
						armor_FIRST.build_features[0] = angle_diff;
						armor_FIRST.build_features[1] = Y_diff;
						armor_FIRST.build_features[2] = height_diff;
						armor_FIRST.build_features[3] = width_diff;
						armor_SECOND.push_back(armor_FIRST);
					}
				}
				else if ((angle_diff < T_ANGLE_THREMIN || 180 - angle_diff < T_ANGLE_THRE180MIN) &&
					Y_diff < MH_diff * 3 / 2 && X_diff < height_max * 4 &&
					height_diff / height_sum < T_HIGH_RAT_ANGLE &&
					width_diff / width_sum < T_WHIDTH_RAT_ANGLE
					)
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //装甲中心的x坐标
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //装甲中心的y坐标
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //装甲所在旋转矩形的旋转角度
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //装甲的高度
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); // 装甲的宽度等于两侧LED所在旋转矩形中心坐标的距离
					if (nL < nW)
					{
						armor_FIRST.armorS.size.height = nL;
						armor_FIRST.armorS.size.width = nW;
					}
					else
					{
						armor_FIRST.armorS.size.height = nW;
						armor_FIRST.armorS.size.width = nL;
					}
					if (Y_diff < nW / 2)
					{
						armor_FIRST.build1_No = i;
						armor_FIRST.build2_No = j;
						armor_FIRST.build_features[0] = angle_diff;
						armor_FIRST.build_features[1] = Y_diff;
						armor_FIRST.build_features[2] = height_diff;
						armor_FIRST.build_features[3] = width_diff;
						armor_SECOND.push_back(armor_FIRST); //将找出的装甲的旋转矩形保存到vector
					}
				}
				else if ((angle_diff < 3 || 180 - angle_diff < 2) &&
					Y_diff < MH_diff * 2 && X_diff < height_max * 4
					//height_diff / height_sum < T_HIGH_RAT_ANGLE
					)
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //装甲中心的x坐标
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //装甲中心的y坐标
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //装甲所在旋转矩形的旋转角度
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //装甲的高度
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); // 装甲的宽度等于两侧LED所在旋转矩形中心坐标的距离
					if (nL < nW)
					{
						armor_FIRST.armorS.size.height = nL;
						armor_FIRST.armorS.size.width = nW;
					}
					else
					{
						armor_FIRST.armorS.size.height = nW;
						armor_FIRST.armorS.size.width = nL;
					}
					if ((abs(ss[i].center.y - ss[j].center.y) < nW / 2))
					{
						armor_FIRST.build1_No = i;
						armor_FIRST.build2_No = j;
						armor_FIRST.build_features[0] = angle_diff;
						armor_FIRST.build_features[1] = Y_diff;
						armor_FIRST.build_features[2] = height_diff;
						armor_FIRST.build_features[3] = width_diff;
						armor_SECOND.push_back(armor_FIRST); //将找出的装甲的旋转矩形保存到vector
					}
				}
				else if ((angle_diff < 3 || 180 - angle_diff < 2) &&
					Y_diff < MH_diff * 3 && X_diff < height_max * 5
					//height_diff / height_sum < T_HIGH_RAT_ANGLE
					)
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //装甲中心的x坐标
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //装甲中心的y坐标
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //装甲所在旋转矩形的旋转角度
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //装甲的高度
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); //装甲的宽度等于两侧LED所在旋转矩形中心坐标的距离
					if (nL < nW)
					{
						armor_FIRST.armorS.size.height = nL;
						armor_FIRST.armorS.size.width = nW;
					}
					else
					{
						armor_FIRST.armorS.size.height = nW;
						armor_FIRST.armorS.size.width = nL;
					}
					if (Y_diff < nW / 2)
					{
						armor_FIRST.build1_No = i;
						armor_FIRST.build2_No = j;
						armor_FIRST.build_features[0] = angle_diff;
						armor_FIRST.build_features[1] = Y_diff;
						armor_FIRST.build_features[2] = height_diff;
						armor_FIRST.build_features[3] = width_diff;
						armor_SECOND.push_back(armor_FIRST); //将找出的装甲的旋转矩形保存到vector
					}
				}
			}
		}
		if (armor_SECOND.size() < 1)
		{
			cout << "装甲补充" << endl;
			int ss_width = 0;
			int ss_ID = 0;
			for (unsigned int SSS = 0; SSS < ss.size(); SSS++) //求任意两个旋转矩形的夹角
			{
				if (ss[SSS].size.width > ss_width && (ss[SSS].size.width / ss[SSS].size.height) < 0.4 && (ss[SSS].size.width / ss[SSS].size.height) > 0.15)
				{
					ss_width = ss[SSS].size.width;
					ss_ID = SSS;
				}

			}
			int WIDTH = 3 * ss[ss_ID].size.height;
			int HEIGHT = 3 * ss[ss_ID].size.height;
			int XX_RIGHT = ss[ss_ID].center.x;
			int XX_LEFT = ss[ss_ID].center.x - WIDTH;
			int YY = ss[ss_ID].center.y - ss[ss_ID].size.height * 3 / 2;
			//
			if (XX_RIGHT + WIDTH > 1024)
			{
				WIDTH = 1024 - XX_RIGHT;
			}
			if (XX_RIGHT < 0)
			{
				XX_RIGHT = 0;
			}
			if (XX_LEFT < 0)
			{
				XX_LEFT = 0;
			}
			if (XX_LEFT + WIDTH > 1024)
			{
				WIDTH = 1024 - XX_LEFT;
			}
			if (YY + HEIGHT > 768)
			{
				HEIGHT = 768 - YY;
			}
			if (YY < 0)
			{
				YY = 0;
			}
			/*
						if (ss[ss_ID].angle > 45)
						{

							Mat LEFT_rio = src(Rect(XX_RIGHT, YY, WIDTH, HEIGHT));
							Mat  Rio_out, Rio_out1;

							cvtColor(LEFT_rio, Rio_out, COLOR_BGR2GRAY);//转化为灰度图像

							equalizeHist(Rio_out, Rio_out);

							imshow("rio", Rio_out);
							GaussianBlur(Rio_out, Rio_out, Size(3, 3), 0, 0);//高斯滤波
							threshold(Rio_out, Rio_out1, 0, 255, THRESH_OTSU);//二值化
							imshow("RIO_二值化", Rio_out1);

							vector<vector<Point>> last_contours;
							vector<Vec4i>last_hierarchy;
							cv::RotatedRect s_center;
							std::vector<cv::RotatedRect> ss_center;
							ss_center.clear();
							findContours(Rio_out1, last_contours, last_hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
							//drawContours(drawing_out, last_contours, -1, color, 1);//可以省去
							int	centerX_num = 0;

							for (size_t ii = 0; ii < last_contours.size(); ii++)
							{
								if (last_contours[ii].size() > 10)
								{
									s_center = minAreaRect(last_contours[ii]);
									ss_center.push_back(s_center);
								}
							}
							for (size_t iii = 0; iii < ss_center.size() - 1; iii++)
							{
								if (ss_center[iii].size.height > ss[ss_ID].size.height / 2 && ss_center[iii].size.width / ss_center[iii].size.width > 0.5)
								{
									for (size_t jjj = iii + 1; jjj < ss_center.size(); jjj++)
									{
										int centerX_sum = abs(ss_center[iii].center.x - ss_center[jjj].center.x);
										int centerY_sum = abs(ss_center[iii].center.y - ss_center[jjj].center.y);
										if (centerX_sum < 5 && centerY_sum < 5)
											centerX_num++;
									}
								}
							}
							if (centerX_num > 3)
							{
								ss[ss_ID].center.x = ss[ss_ID].center.x + cos((double)(180.0 - ss[ss_ID].angle) / 180.0*PI)*ss[ss_ID].size.height;
								ss[ss_ID].center.y = ss[ss_ID].center.y - sin((double)(180.0 - ss[ss_ID].angle) / 180.0*PI)*ss[ss_ID].size.height;

								target.center = ss[ss_ID].center;
								target.size.width = ss[ss_ID].size.height * 2;
								target.size.height = ss[ss_ID].size.height;

								target.angle = ss[ss_ID].angle;

								all.push_back(ss[ss_ID]);
							}
						}
						else
						{
							Mat LEFT_rio = src(Rect(XX_LEFT, YY, WIDTH, HEIGHT));
							Mat  Rio_out, Rio_out1;

							cvtColor(LEFT_rio, Rio_out, COLOR_BGR2GRAY);//转化为灰度图像

							equalizeHist(Rio_out, Rio_out);
							imshow("rio", Rio_out);
							GaussianBlur(Rio_out, Rio_out, Size(3, 3), 0, 0);//高斯滤波

							threshold(Rio_out, Rio_out1, 0, 255, THRESH_OTSU);//二值化
							imshow("RIO_二值化", Rio_out1);

							vector<vector<Point>> last_contours;
							vector<Vec4i>last_hierarchy;
							cv::RotatedRect s_center;
							std::vector<cv::RotatedRect> ss_center;
							ss_center.clear();
							findContours(Rio_out1, last_contours, last_hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
							//drawContours(drawing_out, last_contours, -1, color, 1);//可以省去
							int	centerX_num = 0;

							for (size_t ii = 0; ii < last_contours.size(); ii++)
							{
								if (last_contours[ii].size() > 10)
								{
									s_center = minAreaRect(last_contours[ii]);
									ss_center.push_back(s_center);
								}
							}
							for (size_t iii = 0; iii < ss_center.size() - 1; iii++)
							{
								if (ss_center[iii].size.height > ss[ss_ID].size.height / 2 && ss_center[iii].size.width / ss_center[iii].size.width > 0.5)
								{
									for (size_t jjj = iii + 1; jjj < ss_center.size(); jjj++)
									{
										int centerX_sum = abs(ss_center[iii].center.x - ss_center[jjj].center.x);
										int centerY_sum = abs(ss_center[iii].center.y - ss_center[jjj].center.y);
										if (centerX_sum < 5 && centerY_sum < 5)
										{
											centerX_num++;
										}
									}
								}
							}
							if (centerX_num > 3)
							{
								ss[ss_ID].center.x = ss[ss_ID].center.x - cos((double)(ss[ss_ID].angle) / 180.0*PI)*ss[ss_ID].size.height;
								ss[ss_ID].center.y = ss[ss_ID].center.y - sin((double)(ss[ss_ID].angle) / 180.0*PI)*ss[ss_ID].size.height;

								target.center = ss[ss_ID].center;
								target.size.width = ss[ss_ID].size.height * 2;
								target.size.height = ss[ss_ID].size.height;

								target.angle = ss[ss_ID].angle;

								all.push_back(ss[ss_ID]);
							}
						}*/
		}
		else if (armor_SECOND.size() == 1)
		{
			target = armor_SECOND[0].armorS;
			all.push_back(armor_SECOND[0].armorS);
			armor_center_x = target.center.x;
			armor_center_y = target.center.y;
		}
		else
		{
			cout << "多个装甲" << endl;
			double min_feature = 9999999;
			for (int armor_i = 0; armor_i < armor_SECOND.size(); armor_i++)//对各个灯带进行遍历
			{
				armors.push_back(armor_SECOND[armor_i].armorS);
				//计算加权特征值
				double feature = armor_SECOND[armor_i].build_features[0] * 100 +
					armor_SECOND[armor_i].build_features[1] * 10 +
					armor_SECOND[armor_i].build_features[2] * 100 +
					//armor_SECOND[armor_i].build_features[3] * 0 +
					abs(armor_SECOND[armor_i].armorS.center.x - armor_center_x) * 50 +
					abs(armor_SECOND[armor_i].armorS.center.y - armor_center_y) * 50 -
					armor_SECOND[armor_i].armorS.size.height * 100 -
					armor_SECOND[armor_i].armorS.size.width * 100;
				if (feature < min_feature)//找到最小特征值
				{
					min_feature = feature;
					target = armor_SECOND[armor_i].armorS;
				}

			}
			//存储上一次装甲中心点
			armor_center_x = target.center.x;
			armor_center_y = target.center.y;
			all = armors;
		}
	}
}

/*************************************************
Function:       DrawAll
Description:    画出视野目标
Input:          all_target_  img
Output:
Return:
Others:         
*************************************************/
void DrawAll(vector<RotatedRect> rect, Mat img)
{
	for (int i = 0; i < rect.size(); i++)
	{
		Point2f pp[4];
		rect[i].points(pp); // j计算二维盒子顶点
		line(img, pp[0], pp[1], CV_RGB(255, 255, 255), 1, 8, 0);
		line(img, pp[1], pp[2], CV_RGB(255, 255, 255), 1, 8, 0);
		line(img, pp[2], pp[3], CV_RGB(255, 255, 255), 1, 8, 0);
		line(img, pp[3], pp[0], CV_RGB(255, 255, 255), 1, 8, 0);
	}
}
/*************************************************
Function:       DrawTarget
Description:    画出击打目标
Input:          target_  img
Output:
Return:
Others:
*************************************************/
void DrawTarget(RotatedRect box, Mat img)
{
	Point2f pts[8];
	pts[0].x = box.center.x;
	pts[0].y = box.center.y - 10;
	pts[1].x = box.center.x;
	pts[1].y = box.center.y + 10;
	pts[2].x = box.center.x - 10;
	pts[2].y = box.center.y;
	pts[3].x = box.center.x + 10;
	pts[3].y = box.center.y;

	pts[4].x = img_center_x;
	pts[4].y = img_center_y - 10;
	pts[5].x = img_center_x;
	pts[5].y = img_center_y + 10;
	pts[6].x = img_center_x - 10;
	pts[6].y = img_center_y;
	pts[7].x = img_center_x + 10;
	pts[7].y = img_center_y;
	line(img, pts[0], pts[1], CV_RGB(0, 255, 0), 2, 8, 0);
	line(img, pts[2], pts[3], CV_RGB(0, 255, 0), 2, 8, 0);
	line(img, pts[4], pts[5], CV_RGB(255, 255, 255), 2, 8, 0);
	line(img, pts[6], pts[7], CV_RGB(255, 255, 255), 2, 8, 0);
}
/*************************************************
Function:       AutoShoot
Description:    辅助射击
Input:          
Output:         
Return:
Others:         主函数调用
*************************************************/
void ArmorPlate::AutoShoot()
{
	//armor_image_ = imread("1.png"); // 加载本地图片

	ImgPreprocess(armor_image_, pre_image_);
	FindArmor(armor_image_, pre_image_, all_target_, target_); // 定位装甲板

	DrawAll(all_target_, armor_image_);
	DrawTarget(target_, armor_image_);

	imshow("原图", armor_image_);
	waitKey(1);
}


