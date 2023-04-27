#pragma once

#include <windows.h>
#include <filesystem>
#include "CommonType.h"

class Path {
private:
	static const String currentPath;
public:
	static const String modelPath;
	static const String musicPath;
	static const String melodyPath;
};


#define BACKGROUND_PATH TEXT("Background")

#define KEYBOARD_PATH TEXT("Keyboard")
#define WHITE_KEY_LEFT_PATH TEXT("White_Key_Left")
#define WHITE_KEY_RIGHT_PATH TEXT("White_Key_Right")
#define BLUE_KEY_PATH TEXT("Blue_Key")
#define YELLOW_KEY_PATH TEXT("Yellow_Key")

#define WHITE_NOTE_PATH TEXT("White_Note")
#define BLUE_NOTE_PATH TEXT("Blue_Note")
#define YELLOW_NOTE_PATH TEXT("Yellow_Note")

#define SCORE_BOARD_PATH TEXT("Score_Board")
