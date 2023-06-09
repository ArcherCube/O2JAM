#pragma once

#include "Actor.h"
#include "Path.h"
#include "Constant.h"

//计分板的高度
#define SCORE_BOARD_HEIGHT				102.0
//计分板的宽度
#define SCORE_BOARD_WIDTH				192.0
//计分板线框的厚度
#define SCORE_BOARD_BORDER				2.0
//计分板文字的左右Padding
#define SCORE_BOARD_PADDING				3.0
//计分板统计处的字体大小
#define SCORE_BOARD_STATISTIC_TEXT_SIZE	18
//计分板COMBO处的字体大小
#define SCORE_BOARD_COMBO_TEXT_SIZE		24
//计分板的位置
#define SCORE_BOARD_POSITION			POSITION(0.0, SCORE_BOARD_POSITION_Y)
#define COOL_TEXT_AREA					AREA(SCORE_BOARD_BORDER+SCORE_BOARD_PADDING,SCORE_BOARD_POSITION_Y+SCORE_BOARD_BORDER,SCORE_BOARD_WIDTH/2-((SCORE_BOARD_BORDER/2)+SCORE_BOARD_PADDING),SCORE_BOARD_POSITION_Y+(SCORE_BOARD_HEIGHT-(SCORE_BOARD_BORDER*4))/3+SCORE_BOARD_BORDER)
#define GOOD_TEXT_AREA					AREA(SCORE_BOARD_WIDTH/2+(SCORE_BOARD_BORDER/2)+SCORE_BOARD_PADDING,SCORE_BOARD_POSITION_Y+SCORE_BOARD_BORDER,SCORE_BOARD_WIDTH-5.0,SCORE_BOARD_POSITION_Y+(SCORE_BOARD_HEIGHT-(SCORE_BOARD_BORDER*4))/3+SCORE_BOARD_BORDER)
#define BAD_TEXT_AREA					AREA(SCORE_BOARD_BORDER+SCORE_BOARD_PADDING,SCORE_BOARD_POSITION_Y+(SCORE_BOARD_HEIGHT-(SCORE_BOARD_BORDER*4))/3+SCORE_BOARD_BORDER*2,SCORE_BOARD_WIDTH/2-((SCORE_BOARD_BORDER/2)+SCORE_BOARD_PADDING),SCORE_BOARD_POSITION_Y+(SCORE_BOARD_HEIGHT-(SCORE_BOARD_BORDER*4))/3*2+SCORE_BOARD_BORDER*2)
#define MISS_TEXT_AREA					AREA(SCORE_BOARD_WIDTH/2+(SCORE_BOARD_BORDER/2)+SCORE_BOARD_PADDING,SCORE_BOARD_POSITION_Y+(SCORE_BOARD_HEIGHT-(SCORE_BOARD_BORDER*4))/3+SCORE_BOARD_BORDER*2,SCORE_BOARD_WIDTH-5.0,SCORE_BOARD_POSITION_Y+(SCORE_BOARD_HEIGHT-(SCORE_BOARD_BORDER*4))/3*2+SCORE_BOARD_BORDER*2)
#define TOTAL_TEXT_AREA					AREA(SCORE_BOARD_BORDER+SCORE_BOARD_PADDING,SCORE_BOARD_POSITION_Y+(SCORE_BOARD_HEIGHT-(SCORE_BOARD_BORDER*4))/3*2+SCORE_BOARD_BORDER*3,SCORE_BOARD_WIDTH-5.0,SCORE_BOARD_POSITION_Y+SCORE_BOARD_HEIGHT-SCORE_BOARD_BORDER)
#define COMBO_TEXT_AREA					AREA(0,(KEY_POSITION_Y-SCORE_BOARD_COMBO_TEXT_SIZE)/2,SCORE_BOARD_WIDTH,(KEY_POSITION_Y+SCORE_BOARD_COMBO_TEXT_SIZE)/2)
#define COMBO_DESC_TEXT_AREA			AREA(0,(KEY_POSITION_Y+SCORE_BOARD_COMBO_TEXT_SIZE)/2+SCORE_BOARD_BORDER,SCORE_BOARD_WIDTH,(KEY_POSITION_Y+SCORE_BOARD_COMBO_TEXT_SIZE)/2+SCORE_BOARD_BORDER+SCORE_BOARD_COMBO_TEXT_SIZE)

typedef Actor Text;

class ScoreBoard : public Actor
{
private:

	const Text* const coolText, * const goodText, * const badText, * const missText, * const totalText,*const comboDescText;

	UINT coolCount, goodCount, badCount, missCount, comboCount;

	Text* const coolCountText, * const goodCountText, * const badCountText, * const missCountText, * const totalCountText, * const comboCountText;
private:
	const UINT RefreshStatisticText() const;
	const UINT RefreshComboText(const HitType& lastHitType) const;
	const UINT ClearComboText() const;
	const UINT RefreshText(const HitType& lastHitType) const;
public:
	explicit ScoreBoard();

	const UINT Count(const HitType& hitType, const UINT& count);

	const UINT Clear();

	virtual ~ScoreBoard();
};

