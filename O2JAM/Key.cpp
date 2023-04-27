#include "Key.h"

#include "CentralTimer.h"
#include "Path.h"

Key::Key(const KeyType& keyType, const Position& position)
	:Actor(keyType == KeyType::WHITE_L ? WHITE_KEY_LEFT_PATH : (keyType == KeyType::WHITE_R ? WHITE_KEY_RIGHT_PATH : (keyType == KeyType::BLUE ? BLUE_KEY_PATH : YELLOW_KEY_PATH))
		, position, keyType == KeyType::BLUE ? BLUE_KEY_Z : (keyType == KeyType::YELLOW ? YELLOW_KEY_Z : WHITE_KEY_Z))
	, keyType(keyType), lastPressTime(CentralTimer::GetTimeStamp()), lastReleaseTime(lastPressTime),isPressed(false)
{
	PlayAction(TEXT("dark"));
}

const TimestampType Key::Press()
{
	if (isPressed) return lastPressTime;
	isPressed = true;

	PlayAction(TEXT("light"));

	return (lastPressTime = CentralTimer::GetTimeStamp());
}

const TimestampType Key::Release()
{
	if (!isPressed) return lastReleaseTime;
	isPressed = false;

	PlayAction(TEXT("dark"));

	return (lastReleaseTime = CentralTimer::GetTimeStamp());
}

const TimestampType Key::GetLastHoldTime() const
{
	return lastReleaseTime - lastPressTime;
}
