#include "ScoreBoard.h"

#include "Actor.h"
#include "TextModel.h"
#include "CentralTimer.h"

#define TOTAL(cool,good,bad,miss) (long)(cool * 300 + good * 100 - (bad * 100 + miss * 200))
#define COMBO_CLEAR_TIME 5 * 1000

COLORREF textColor = RGB(120, 200, 120);

ScoreBoard::ScoreBoard()
	:Actor(SCORE_BOARD_PATH, SCORE_BOARD_POSITION, SCORE_BOARD_Z)
	, coolText(new Text(TEXT("COOL:"), textColor, SCORE_BOARD_STATISTIC_TEXT_SIZE, COOL_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_LEFT))
	, goodText(new Text(TEXT("GOOD:"), textColor, SCORE_BOARD_STATISTIC_TEXT_SIZE, GOOD_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_LEFT))
	, badText(new Text(TEXT("BAD:"), textColor, SCORE_BOARD_STATISTIC_TEXT_SIZE, BAD_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_LEFT))
	, missText(new Text(TEXT("MISS:"), textColor, SCORE_BOARD_STATISTIC_TEXT_SIZE, MISS_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_LEFT))
	, totalText(new Text(TEXT("TOTAL:"), textColor, SCORE_BOARD_STATISTIC_TEXT_SIZE, TOTAL_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_LEFT))
	, comboDescText(new Text(TEXT(""), textColor, SCORE_BOARD_COMBO_TEXT_SIZE, COMBO_DESC_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_CENTER))
	, coolCount(0), goodCount(0), badCount(0), missCount(0), comboCount(0)
	, coolCountText(new Text(TEXT("0"), textColor, SCORE_BOARD_STATISTIC_TEXT_SIZE, COOL_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_RIGHT))
	, goodCountText(new Text(TEXT("0"), textColor, SCORE_BOARD_STATISTIC_TEXT_SIZE, GOOD_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_RIGHT))
	, badCountText(new Text(TEXT("0"), textColor, SCORE_BOARD_STATISTIC_TEXT_SIZE, BAD_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_RIGHT))
	, missCountText(new Text(TEXT("0"), textColor, SCORE_BOARD_STATISTIC_TEXT_SIZE, MISS_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_RIGHT))
	, totalCountText(new Text(TEXT("0"), textColor, SCORE_BOARD_STATISTIC_TEXT_SIZE, TOTAL_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_RIGHT))
	, comboCountText(new Text(TEXT(""), textColor, SCORE_BOARD_COMBO_TEXT_SIZE, COMBO_TEXT_AREA, SCORE_BOARD_TEXT_Z, DT_CENTER))
{
}


const UINT ScoreBoard::RefreshStatisticText() const
{
	((TextModel*)missCountText->GetModel())->SetText(TO_STRING(missCount));
	((TextModel*)badCountText->GetModel())->SetText(TO_STRING(badCount));
	((TextModel*)goodCountText->GetModel())->SetText(TO_STRING(goodCount));
	((TextModel*)coolCountText->GetModel())->SetText(TO_STRING(coolCount));
	((TextModel*)totalCountText->GetModel())->SetText(TO_STRING(TOTAL(coolCount, goodCount, badCount, missCount)));

	//未启用判断
	return 0;
}

const UINT ScoreBoard::RefreshComboText(const HitType& lastHitType) const
{
	if (lastHitType == HitType::AIR) return 0;

	String comboDesc;
	switch (lastHitType)
	{
	case HitType::MISS:
		comboDesc.assign(TEXT("MISS"));
		break;
	case HitType::BAD:
		comboDesc.assign(TEXT("BAD"));
		break;
	case HitType::GOOD:
		comboDesc.assign(TEXT("GOOD"));
		break;
	case HitType::COOL:
		comboDesc.assign(TEXT("COOL"));
		break;
	default:
		break;
	}
	((TextModel*)comboCountText->GetModel())->SetText(TO_STRING(comboCount));
	((TextModel*)comboDescText->GetModel())->SetText(comboDesc);

	//COMBO_CLEAR_TIME后清除文字，每次RefreshComboText都会刷新这个时间（CentralTimer加入重复键值的事务会覆盖
	CentralTimer::Add(TEXT("clearComboText"), COMBO_CLEAR_TIME, 
		[this] 
		{
			ClearComboText();
		}
	);

	//未启用判断
	return 0;
}

const UINT ScoreBoard::ClearComboText() const
{
	((TextModel*)comboCountText->GetModel())->SetText(TEXT(""));
	((TextModel*)comboDescText->GetModel())->SetText(TEXT(""));

	//未启用判断
	return 0;
}

const UINT ScoreBoard::RefreshText(const HitType& lastHitType) const
{
	if (lastHitType == HitType::AIR) return 0;

	RefreshStatisticText();
	RefreshComboText(lastHitType);
	
}

const UINT ScoreBoard::Count(const HitType& hitType, const UINT& count)
{
	if (hitType == HitType::AIR) return 0;

	switch (hitType)
	{
	case HitType::MISS:
		missCount += count;
		comboCount = 0;
		break;
	case HitType::BAD:
		badCount += count;
		comboCount = 0;
		break;
	case HitType::GOOD:
		goodCount += count;
		++comboCount;
		break;
	case HitType::COOL:
		coolCount += count;
		++comboCount;
		break;
	default:
		break;
	}

	RefreshText(hitType);

	//未启用判断
	return 0;
}


const UINT ScoreBoard::Clear()
{
	coolCount = 0;
	goodCount = 0;
	badCount = 0;
	missCount = 0;
	comboCount = 0;

	RefreshStatisticText();
	ClearComboText();

	//未启用判断
	return 0;
}

ScoreBoard::~ScoreBoard()
{
	delete coolText;
	delete goodText;
	delete badText;
	delete missText;
	delete totalText;
	delete coolCountText;
	delete goodCountText;
	delete badCountText;
	delete missCountText;
	delete totalCountText;
	delete comboCountText;
}
