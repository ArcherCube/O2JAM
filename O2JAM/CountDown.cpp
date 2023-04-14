#include "CountDown.h"

#include "Actor.h"
#include "CentralTimer.h"


UINT CountDown::RefreshCountDownText(const Text* const countDownText, const TimestampType& secTime)
{
	return ((TextModel*)countDownText->GetModel())->SetText(secTime ? TO_STRING(secTime) : TEXT(""));
}

UINT CountDown::ShowCountDown(TimestampType time)
{
	const Text* const countDownText = new Text(TEXT(""), RGB(255, 255, 255), 80, AREA(0, 400, 192, 500), SCORE_BOARD_TEXT_Z, DT_CENTER);

	const String&& tagPrefix = TEXT("countDown") + TO_STRING((int)countDownText) + TEXT(": ");

	UINT countDownSec = 0;
	while (time >= 1000)
	{
		CentralTimer::Add(tagPrefix + TO_STRING(time), time,
			[=]
			{
				if (!countDownSec)
				{
					delete countDownText;
				}
				else
				{
					RefreshCountDownText(countDownText, countDownSec);
				}
			});
		++countDownSec;
		time -= 1000;
	}
	if (time > 0)
	{
		CentralTimer::Add(tagPrefix + TO_STRING(time), time,
			[=]
			{
				RefreshCountDownText(countDownText, countDownSec);
			});
		++countDownSec;
	}
	RefreshCountDownText(countDownText, countDownSec);

	//δ�����ж�
	return 0;
}