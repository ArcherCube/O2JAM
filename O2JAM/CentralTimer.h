#pragma once

#include <windows.h>
#include <iostream>
#include <functional>
#include <unordered_map> //对外，有tag、需要随机删（按唯一tag）、元素间无线性关系。
#include <set>  //对内，需要按excTime升序，所以，以set作为底层存储Timer，
#include "CommonType.h"
#include "Constant.h"

using std::function;


class CentralTimer
{
private:
	struct Timer
	{
		const String tag; //这个tag与hashmap中的key对应，在因到时间而执行并删除set中一个timer时，可以根据这个tag快速删除hashmap中对应的元素（而不是遍历查找
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
	//set对内，真实保存timer
	static TimerSet* const timerSet;
	//hashmap对外，表达tag于timer的联系
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

