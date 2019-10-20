/*************************************************
Copyright:
Author:        Lcy
Date:          2019-8-11
Description:   װ��ʶ��
Version:       1.0.1
**************************************************/
#include <opencv2/opencv.hpp>
#include "armor_plate.h"

using namespace cv;
using namespace std;

/*************************************************
Function:       ArmorPlate
Description:    ���캯��
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
Description:    ����ͷ��ʼ��
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
		cout << "�Ӿ���������ͷ��ʧ��" << endl;
		return false;
	}
	else
		return true;
}
/*************************************************
Function:       ImgPreprocess
Description:    ͼ��Ԥ����
Input:          src
Output:         dst
Return:         
Others:         RGB
*************************************************/
void ArmorPlate::ImgPreprocess(const cv::Mat& src, cv::Mat& dst)
{
	std::vector<Mat> img_channels; // ͨ��
	split(src, img_channels); // ͨ������

	if (our_team_) // �ҷ��췽 �Է�����
	{
		Mat img_blue_channels;
		img_blue_channels = img_channels.at(0); // ��ɫͨ��
		imshow("blue_channels", img_blue_channels);
		img_blue_channels = img_blue_channels - img_channels.at(1)*0.4 - img_channels.at(2)*0.4; // ȥ����ɫӰ��
		imshow("blue", img_blue_channels);
	}
	else // �ҷ����� �Է��췽
	{
		Mat img_red_channels;
		img_red_channels = img_channels.at(2); // ��ɫͨ��
		//imshow("img_red_channels", img_red_channels);
		img_red_channels = img_red_channels - img_channels.at(0)*0.4 - img_channels.at(1)*0.4; // ȥ����ɫӰ��
		//img_red_channels = img_red_channels - img_channels.at(0); // �ٷ�˼·
		blur(img_red_channels, img_red_channels, Size(3, 3));
		img_red_channels = img_red_channels * 3;
		//imshow("red_channels", img_red_channels);
		double maxValue_gary;
		minMaxLoc(img_red_channels, 0, &maxValue_gary, 0, 0); // ��ȡ���Ҷ�ֵ
		Mat imgBin;
		threshold(img_red_channels, imgBin, maxValue_gary*0.7, 255, THRESH_BINARY);
		//imshow("��ֵ��", imgBin);
		Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
		dilate(imgBin, dst, element, Point(-1, -1), 3); // ����3��
		erode(dst, dst, element, Point(-1, -1), 4); // ��ʴ4��
		dilate(imgBin, dst, element, Point(-1, -1), 3); // ����3��
		//imshow("RGBԤ����", dst);
	}
}
/*************************************************
Function:       FindArmor
Description:    �������
Input:          src  dst
Output:         ��ҰĿ�� ����Ŀ��
Return:
Others:       
*************************************************/
void  ArmorPlate::FindArmor(Mat& src, Mat& dst, vector<RotatedRect>& all, RotatedRect& target) // ��λװ�װ�
{
	all.clear();
	target.center.x = 0;
	target.center.y = 0;
	target.size.width = 0;
	target.size.height = 0;
	target.angle = 0;

	RotatedRect s, s_fitEllipse, s_minAreaRect; // ����ɸѡ����
	vector<RotatedRect> ss; 
	ss.clear();

	vector<vector<Point>> contours; // ����
	vector<Vec4i> hierarchy; // ���

	findContours(dst, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE); // Ѱ������
	Mat drawing = Mat::zeros(dst.size(), CV_8UC3);
	RNG g_rng(12345);
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point()); // ������������
		imshow("����", drawing);

		if (contours[i].size() >= 10)
		{
			s_fitEllipse = fitEllipse(contours[i]); // ��Բ���
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

	// �б�װ��
	vector<RotatedRect> armors;
	vector<ArmorBuilded> armor_SECOND;
	ArmorBuilded armor_FIRST;
	static float armor_center_x;
	static float armor_center_y;

	armors.clear();
	armor_SECOND.clear();
	int nL, nW;

	if (ss.size() < 2) // ��ת����С��������ֱ�ӷ���
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
				double height_diff = abs(ss[i].size.height - ss[j].size.height); // �߶Ȳ�
				double height_sum = ss[i].size.height + ss[j].size.height; // �߶Ⱥ�
				double width_diff = abs(ss[i].size.width - ss[j].size.width); // ��Ȳ�
				double width_sum = ss[i].size.width + ss[j].size.width; // ��Ⱥ�
				double angle_diff = fabs(ss[i].angle - ss[j].angle); // �ǶȲ�
				double Y_diff = abs(ss[i].center.y - ss[j].center.y); // �������ֵ
				double X_diff = abs(ss[i].center.x - ss[j].center.x); // �������ֵ
				double MH_diff = (min(ss[i].size.height, ss[j].size.height)) * 2 / 3; // �߶Ȳ��޷�
				double height_max = max(ss[i].size.height, ss[j].size.height); // ���߶�

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
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //װ�����ĵ�x����
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //װ�����ĵ�y����
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //װ��������ת���ε���ת�Ƕ�
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //װ�׵ĸ߶�
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); // װ�׵Ŀ�ȵ�������LED������ת������������ľ���
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
						armor_SECOND.push_back(armor_FIRST); //���ҳ���װ�׵���ת���α��浽vector
					}
				}
				else if ((angle_diff < 3 || 180 - angle_diff < 2) &&
					Y_diff < MH_diff * 2 && X_diff < height_max * 4
					//height_diff / height_sum < T_HIGH_RAT_ANGLE
					)
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //װ�����ĵ�x����
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //װ�����ĵ�y����
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //װ��������ת���ε���ת�Ƕ�
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //װ�׵ĸ߶�
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); // װ�׵Ŀ�ȵ�������LED������ת������������ľ���
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
						armor_SECOND.push_back(armor_FIRST); //���ҳ���װ�׵���ת���α��浽vector
					}
				}
				else if ((angle_diff < 3 || 180 - angle_diff < 2) &&
					Y_diff < MH_diff * 3 && X_diff < height_max * 5
					//height_diff / height_sum < T_HIGH_RAT_ANGLE
					)
				{
					armor_FIRST.armorS.center.x = ((ss[i].center.x + ss[j].center.x) / 2); //װ�����ĵ�x����
					armor_FIRST.armorS.center.y = ((ss[i].center.y + ss[j].center.y) / 2); //װ�����ĵ�y����
					armor_FIRST.armorS.angle = (ss[i].angle + ss[j].angle) / 2;   //װ��������ת���ε���ת�Ƕ�
					if (180 - angle_diff < T_ANGLE_THRE180)
						armor_FIRST.armorS.angle += 90;
					nL = (ss[i].size.height + ss[j].size.height) / 2; //װ�׵ĸ߶�
					nW = sqrt((ss[i].center.x - ss[j].center.x) * (ss[i].center.x - ss[j].center.x) + (ss[i].center.y - ss[j].center.y) * (ss[i].center.y - ss[j].center.y)); //װ�׵Ŀ�ȵ�������LED������ת������������ľ���
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
						armor_SECOND.push_back(armor_FIRST); //���ҳ���װ�׵���ת���α��浽vector
					}
				}
			}
		}
		if (armor_SECOND.size() < 1)
		{
			cout << "װ�ײ���" << endl;
			int ss_width = 0;
			int ss_ID = 0;
			for (unsigned int SSS = 0; SSS < ss.size(); SSS++) //������������ת���εļн�
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

							cvtColor(LEFT_rio, Rio_out, COLOR_BGR2GRAY);//ת��Ϊ�Ҷ�ͼ��

							equalizeHist(Rio_out, Rio_out);

							imshow("rio", Rio_out);
							GaussianBlur(Rio_out, Rio_out, Size(3, 3), 0, 0);//��˹�˲�
							threshold(Rio_out, Rio_out1, 0, 255, THRESH_OTSU);//��ֵ��
							imshow("RIO_��ֵ��", Rio_out1);

							vector<vector<Point>> last_contours;
							vector<Vec4i>last_hierarchy;
							cv::RotatedRect s_center;
							std::vector<cv::RotatedRect> ss_center;
							ss_center.clear();
							findContours(Rio_out1, last_contours, last_hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
							//drawContours(drawing_out, last_contours, -1, color, 1);//����ʡȥ
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

							cvtColor(LEFT_rio, Rio_out, COLOR_BGR2GRAY);//ת��Ϊ�Ҷ�ͼ��

							equalizeHist(Rio_out, Rio_out);
							imshow("rio", Rio_out);
							GaussianBlur(Rio_out, Rio_out, Size(3, 3), 0, 0);//��˹�˲�

							threshold(Rio_out, Rio_out1, 0, 255, THRESH_OTSU);//��ֵ��
							imshow("RIO_��ֵ��", Rio_out1);

							vector<vector<Point>> last_contours;
							vector<Vec4i>last_hierarchy;
							cv::RotatedRect s_center;
							std::vector<cv::RotatedRect> ss_center;
							ss_center.clear();
							findContours(Rio_out1, last_contours, last_hierarchy, RETR_LIST, CHAIN_APPROX_NONE);
							//drawContours(drawing_out, last_contours, -1, color, 1);//����ʡȥ
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
			cout << "���װ��" << endl;
			double min_feature = 9999999;
			for (int armor_i = 0; armor_i < armor_SECOND.size(); armor_i++)//�Ը����ƴ����б���
			{
				armors.push_back(armor_SECOND[armor_i].armorS);
				//�����Ȩ����ֵ
				double feature = armor_SECOND[armor_i].build_features[0] * 100 +
					armor_SECOND[armor_i].build_features[1] * 10 +
					armor_SECOND[armor_i].build_features[2] * 100 +
					//armor_SECOND[armor_i].build_features[3] * 0 +
					abs(armor_SECOND[armor_i].armorS.center.x - armor_center_x) * 50 +
					abs(armor_SECOND[armor_i].armorS.center.y - armor_center_y) * 50 -
					armor_SECOND[armor_i].armorS.size.height * 100 -
					armor_SECOND[armor_i].armorS.size.width * 100;
				if (feature < min_feature)//�ҵ���С����ֵ
				{
					min_feature = feature;
					target = armor_SECOND[armor_i].armorS;
				}

			}
			//�洢��һ��װ�����ĵ�
			armor_center_x = target.center.x;
			armor_center_y = target.center.y;
			all = armors;
		}
	}
}

/*************************************************
Function:       DrawAll
Description:    ������ҰĿ��
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
		rect[i].points(pp); // j�����ά���Ӷ���
		line(img, pp[0], pp[1], CV_RGB(255, 255, 255), 1, 8, 0);
		line(img, pp[1], pp[2], CV_RGB(255, 255, 255), 1, 8, 0);
		line(img, pp[2], pp[3], CV_RGB(255, 255, 255), 1, 8, 0);
		line(img, pp[3], pp[0], CV_RGB(255, 255, 255), 1, 8, 0);
	}
}
/*************************************************
Function:       DrawTarget
Description:    ��������Ŀ��
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
Description:    �������
Input:          
Output:         
Return:
Others:         ����������
*************************************************/
void ArmorPlate::AutoShoot()
{
	//armor_image_ = imread("1.png"); // ���ر���ͼƬ

	ImgPreprocess(armor_image_, pre_image_);
	FindArmor(armor_image_, pre_image_, all_target_, target_); // ��λװ�װ�

	DrawAll(all_target_, armor_image_);
	DrawTarget(target_, armor_image_);

	imshow("ԭͼ", armor_image_);
	waitKey(1);
}


