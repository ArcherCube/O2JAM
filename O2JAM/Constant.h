#pragma once

#include <windows.h>
#include "CommonType.h"

/*ȫ��*/
//�ַ�����󳤶ȣ�StringTable��
#define MAX_LOADSTRING				100
//���ڿ��
#define WINDOW_WIDTH				192
//���ڸ߶�
#define WINDOW_HEIGHT				698
//��������ˢ������
#define PHYSICS_ENGINE_UPDATE_TIME	10 //��λ������
//Բ����
#define PI							3.1415926535898


/*����ֵ����*/
//����ִ�гɹ�
#define SUCCESS 1
//����ִ��ʧ��
#define FAIL	0


/*Ĭ������ֵ*/
//�������������롰�����������ֽ��ޣ��������������ޣ���delay���ڵ��ڸ�ֵΪ��������
#define NOTE_DELAY_MIN		100
//����ʱ��������ʱ��delayС�ڵ��ڸ�ֵ��һ�鰴������Ϊ�ǡ�ͬһʱ�̰��¡�
#define NOTE_ALIGN_MAX		50
//����Ĭ�ϸ߶ȣ��������������ĸ߶ȣ�
#define NOTE_DEFAULT_HEIGHT	10
//��Ϸ�ٶ�
#define DEFAULT_GAME_SPEED	300.0 //��λ������/��

/*����λ����Ϣ*/
/*Z������*/
//������z�߶�
#define BACKGROUND_Z		0
//����������z�߶�
#define NOTE_Z				50
//����(����)��z�߶�
#define KEYBOARD_Z			100
//�Ʒְ��z�߶�
#define SCORE_BOARD_Z		100
//�Ʒְ����ֵ�z�߶�
#define SCORE_BOARD_TEXT_Z	101
//�����ϰ׼���z�߶�
#define WHITE_KEY_Z			101
//�����ϻƼ���z�߶�
#define YELLOW_KEY_Z		101
//������������z�߶�
#define BLUE_KEY_Z			102
//��Ϸ����ʾ�ı���z�߶�
#define MESSAGE_TEXT_Z		1000


/*Ԫ���̶�λ��*/
//������A�������x����
#define TRACK_A_POSITION_X		2.0
//������S�������x����
#define TRACK_S_POSITION_X		31.0
//������D�������x����
#define TRACK_D_POSITION_X		52.0
//������SPACE�������x����
#define TRACK_SPACE_POSITION_X	81.0
//������J�������x����
#define TRACK_J_POSITION_X		112.0
//������K�������x����
#define TRACK_K_POSITION_X		141.0
//������L�������x����
#define TRACK_L_POSITION_X		162.0

//���������а�����y����
#define KEY_POSITION_Y			550.0
//���̵�����y����
#define KEYBOARD_POSITION_Y		KEY_POSITION_Y - 1.0
//������A����x����
#define KEY_A_POSITION_X		2.0
//������S����x����
#define KEY_S_POSITION_X		31.0
//������D����x����
#define KEY_D_POSITION_X		42.0
//������SPACE����x����
#define KEY_SPACE_POSITION_X	81.0
//������J����x����
#define KEY_J_POSITION_X		112.0
//������K����x����
#define KEY_K_POSITION_X		141.0
//������L����x����
#define KEY_L_POSITION_X		152.0
//������A����λ��
#define KEY_A_POSITION			POSITION(KEY_A_POSITION_X, KEY_POSITION_Y)
//������S����λ��
#define KEY_S_POSITION			POSITION(KEY_S_POSITION_X, KEY_POSITION_Y)
//������D����λ��
#define KEY_D_POSITION			POSITION(KEY_D_POSITION_X, KEY_POSITION_Y)
//������SPACE����λ��
#define KEY_SPACE_POSITION		POSITION(KEY_SPACE_POSITION_X , KEY_POSITION_Y)
//������J����λ��
#define KEY_J_POSITION			POSITION(KEY_J_POSITION_X, KEY_POSITION_Y)
//������K����λ��
#define KEY_K_POSITION			POSITION(KEY_K_POSITION_X, KEY_POSITION_Y)
//������L����λ��
#define KEY_L_POSITION			POSITION(KEY_L_POSITION_X, KEY_POSITION_Y)
//���̣���������λ��
#define KEYBOARD_POSITION		POSITION(0.0, KEYBOARD_POSITION_Y)

//������λ��
#define BACKGROUND_POSITION		POSITION(0.0, 0.0)


//�Ʒְ��y����
#define SCORE_BOARD_POSITION_Y	KEYBOARD_POSITION_Y + 46.0