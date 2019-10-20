#pragma once

#define img_center_x 320 // ͼ������xֵ
#define img_center_y 240 // ͼ������yֵ

#define T_ANGLE_THRE 5   //�����������ǶȲ�
#define T_ANGLE_THRE180 3   //�����������ǶȲ�
#define T_ANGLE_THREMIN 3   //����������С�ǶȲ�
#define T_ANGLE_THRE180MIN 2   //����������С�ǶȲ�

#define T_HIGH_RAT 0.2   //�ƴ����߲�� �����߶Ȳ�����������1/3
#define T_HIGH_RAT_ANGLE 0.34   //�ƴ��Ƕȷ�����С�Ƕȸ߲�� �����߶Ȳ�����������1/2


#define T_WHIDTH_RAT 0.4  //�ƴ�������   ������Ȳ�����������3/5
#define T_WHIDTH_RAT_ANGLE 0.55  //�ƴ�������

#define L_WH_RAT 0.8 // ������߱�


#define TEAMBLUE 0
constexpr auto TEAMRED = 1;

class ArmorPlate
{
public:
	cv::Mat armor_image_; // �Ӿ�����ԭͼ
	cv::Mat pre_image_; // Ԥ����ͼ��
	cv::VideoCapture capture_armor_; // �Ӿ���������ͷ

	int our_team_; // ����

	ArmorPlate(); // ���캯��
	bool CamaraInit(int device); // ��������ͷ��ʼ��
	void AutoShoot(); // �ⲿ���ú���

private:
	std::vector<cv::RotatedRect> all_target_;
	cv::RotatedRect target_;

	void ImgPreprocess(const cv::Mat& src, cv::Mat& dst); // ͼ��Ԥ����
	void FindArmor(cv::Mat& src, cv::Mat& dst, std::vector<cv::RotatedRect>& all, cv::RotatedRect& target); // ��λװ�װ�
};

class ArmorBuilded // װ�׹���
{
public:
	cv::RotatedRect armorS;
	int build1_No = 0;
	int build2_No = 0;
	int build_features[4]; // �ǶȲ� �߶������߶Ȳ��Ȳ�
	int vot = 0;
};


