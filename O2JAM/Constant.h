#pragma once

#include <windows.h>
#include "CommonType.h"

/*全局*/
//字符串最大长度（StringTable）
#define MAX_LOADSTRING				100
//窗口宽度
#define WINDOW_WIDTH				192
//窗口高度
#define WINDOW_HEIGHT				698
//物理引擎刷新周期
#define PHYSICS_ENGINE_UPDATE_TIME	10 //单位：毫秒
//圆周率
#define PI							3.1415926535898


/*返回值类型*/
//函数执行成功
#define SUCCESS 1
//函数执行失败
#define FAIL	0


/*默认属性值*/
//音符“延音”与“短音”的区分界限（‘延音’的下限），delay大于等于该值为“延音”
#define NOTE_DELAY_MIN		100
//对齐时允许的最大时间差，delay小于等于该值的一组按键被认为是“同一时刻按下”
#define NOTE_ALIGN_MAX		50
//音符默认高度（‘短音’音符的高度）
#define NOTE_DEFAULT_HEIGHT	10
//游戏速度
#define DEFAULT_GAME_SPEED	300.0 //单位：像素/秒

/*布局位置信息*/
/*Z轴升序*/
//背景的z高度
#define BACKGROUND_Z		0
//各种音符的z高度
#define NOTE_Z				50
//键盘(底座)的z高度
#define KEYBOARD_Z			100
//计分板的z高度
#define SCORE_BOARD_Z		100
//计分板文字的z高度
#define SCORE_BOARD_TEXT_Z	101
//键盘上白键的z高度
#define WHITE_KEY_Z			101
//键盘上黄键的z高度
#define YELLOW_KEY_Z		101
//键盘上蓝键的z高度
#define BLUE_KEY_Z			102
//游戏内提示文本的z高度
#define MESSAGE_TEXT_Z		1000


/*元件固定位置*/
//键盘上A键轨道的x坐标
#define TRACK_A_POSITION_X		2.0
//键盘上S键轨道的x坐标
#define TRACK_S_POSITION_X		31.0
//键盘上D键轨道的x坐标
#define TRACK_D_POSITION_X		52.0
//键盘上SPACE键轨道的x坐标
#define TRACK_SPACE_POSITION_X	81.0
//键盘上J键轨道的x坐标
#define TRACK_J_POSITION_X		112.0
//键盘上K键轨道的x坐标
#define TRACK_K_POSITION_X		141.0
//键盘上L键轨道的x坐标
#define TRACK_L_POSITION_X		162.0

//键盘上所有按键的y坐标
#define KEY_POSITION_Y			550.0
//键盘底座的y坐标
#define KEYBOARD_POSITION_Y		KEY_POSITION_Y - 1.0
//键盘上A键的x坐标
#define KEY_A_POSITION_X		2.0
//键盘上S键的x坐标
#define KEY_S_POSITION_X		31.0
//键盘上D键的x坐标
#define KEY_D_POSITION_X		42.0
//键盘上SPACE键的x坐标
#define KEY_SPACE_POSITION_X	81.0
//键盘上J键的x坐标
#define KEY_J_POSITION_X		112.0
//键盘上K键的x坐标
#define KEY_K_POSITION_X		141.0
//键盘上L键的x坐标
#define KEY_L_POSITION_X		152.0
//键盘上A键的位置
#define KEY_A_POSITION			POSITION(KEY_A_POSITION_X, KEY_POSITION_Y)
//键盘上S键的位置
#define KEY_S_POSITION			POSITION(KEY_S_POSITION_X, KEY_POSITION_Y)
//键盘上D键的位置
#define KEY_D_POSITION			POSITION(KEY_D_POSITION_X, KEY_POSITION_Y)
//键盘上SPACE键的位置
#define KEY_SPACE_POSITION		POSITION(KEY_SPACE_POSITION_X , KEY_POSITION_Y)
//键盘上J键的位置
#define KEY_J_POSITION			POSITION(KEY_J_POSITION_X, KEY_POSITION_Y)
//键盘上K键的位置
#define KEY_K_POSITION			POSITION(KEY_K_POSITION_X, KEY_POSITION_Y)
//键盘上L键的位置
#define KEY_L_POSITION			POSITION(KEY_L_POSITION_X, KEY_POSITION_Y)
//键盘（底座）的位置
#define KEYBOARD_POSITION		POSITION(0.0, KEYBOARD_POSITION_Y)

//背景的位置
#define BACKGROUND_POSITION		POSITION(0.0, 0.0)


//计分板的y坐标
#define SCORE_BOARD_POSITION_Y	KEYBOARD_POSITION_Y + 46.0