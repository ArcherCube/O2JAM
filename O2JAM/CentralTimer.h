#pragma once

#include <windows.h>
#include <iostream>
#include <functional>
#include <unordered_map> //���⣬��tag����Ҫ���ɾ����Ψһtag����Ԫ�ؼ������Թ�ϵ��
#include <set>  //���ڣ���Ҫ��excTime�������ԣ���set��Ϊ�ײ�洢Timer��
#include "CommonType.h"
#include "Constant.h"

using std::function;


class CentralTimer
{
private:
	struct Timer
	{
		const String tag; //���tag��hashmap�е�key��Ӧ������ʱ���ִ�в�ɾ��set��һ��timerʱ�����Ը������tag����ɾ��hashmap�ж�Ӧ��Ԫ�أ������Ǳ�������
		const TimestampType excTime;
		const function<void()> exc;
	};
	struct TimerExcNOLess
	{
		bool operator()(const Timer& _Left, const Timer& _Right) const;
	};
	typedef std::set<Timer, TimerExcNOLess> TimerSet;
	typedef TimerSet::const_iterator TimerSetCiter;
	typedef TimerSet::iterator TimerSetIter;
	typedef std::unordered_map<String, const TimerSetCiter> TimerHashmap;
	typedef TimerHashmap::const_iterator TimerHashmapCiter;
private:
	//static LARGE_INTEGER freq, time;
	//set���ڣ���ʵ����timer
	static TimerSet* const timerSet;
	//hashmap���⣬���tag��timer����ϵ
	static TimerHashmap* const timerHashmap;

public:
	static UINT Add(const String& tag, const TimestampType& waitTime, const function<void()>& exc);

	static UINT Del(const String& tag);

	static TimestampType GetExcTime(const String& tag);

	static TimestampType GetTimeStamp();

	static void Tick();

	static UINT Clean();
};

typedef function<void> DefaultTask;

