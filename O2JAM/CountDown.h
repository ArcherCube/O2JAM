#pragma once

#include "CommonType.h"

class Actor;

typedef Actor Text;

class CountDown
{
private:
	static Text* countDownText;
private:
	inline static UINT RefreshCountDownText(const Text* const countDownText, const TimestampType& secTime);
public:
	static UINT ShowCountDown(TimestampType time);
};

