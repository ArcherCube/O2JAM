#include "CentralTimer.h"

#pragma comment( lib,"winmm.lib" )



CentralTimer::TimerSet* const CentralTimer::timerSet = new CentralTimer::TimerSet();
CentralTimer::TimerHashmap* const CentralTimer::timerHashmap = new CentralTimer::TimerHashmap();
/*LARGE_INTEGER CentralTimer::freq = { 0 };
LARGE_INTEGER CentralTimer::time = { 0 };*/

UINT CentralTimer::Add(const String& tag, const TimestampType& waitTime, const function<void()>& exc)
{
	//若这个tag已经存在则先删除原本的
	if (timerHashmap->find(tag) != timerHashmap->cend())
	{
		//根据hashmap中的值（set的迭代器）来删除set中的记录
		timerSet->erase(timerHashmap->at(tag));

		//根据key删除hashmap中的记录
		timerHashmap->erase(tag);
	}

	return timerHashmap->insert({ tag ,timerSet->insert({tag,waitTime + GetTimeStamp(),exc}).first }).second ? SUCCESS : FAIL;
}

UINT CentralTimer::Del(const String& tag)
{
	TimerHashmapCiter cIter = timerHashmap->find(tag);

	if (cIter == timerHashmap->cend()) return FAIL;

	timerSet->erase(cIter->second);
	timerHashmap->erase(cIter);

	return SUCCESS;
}

TimestampType CentralTimer::GetExcTime(const String& tag)
{
	if (timerHashmap->find(tag) != timerHashmap->cend())
	{
		return timerHashmap->at(tag)->excTime;
	}

	return 0;
}

TimestampType CentralTimer::GetTimeStamp()
{
	/*QueryPerformanceFrequency(&freq); // 获取时钟周期
	QueryPerformanceCounter(&time); // 获取时钟计数

	return (time.QuadPart * 1000 / freq.QuadPart);*/
	return timeGetTime();
}

void CentralTimer::Tick()
{
	TimestampType timeStamp = GetTimeStamp();

	for (TimerSetIter iter = timerSet->begin(); iter != timerSet->end();)
	{
		//当timer存在且执行时间早于当前时间戳时，执行并删除timer；否则，退出循环（因为timerSet按timeStamp升序，一个不符合，后面的全部不符合）
		if (iter->excTime <= timeStamp)
		{
			iter->exc();
			timerHashmap->erase(iter->tag);
			iter = timerSet->erase(iter);
		}
		else
		{
			break;
		}
	}
}

UINT CentralTimer::Clean()
{
	timerHashmap->clear();
	timerSet->clear();
	delete timerHashmap;
	delete timerSet;

	return 0;
}

bool CentralTimer::TimerExcNOLess::operator()(const Timer& _Left, const Timer& _Right) const
{
	return _Left.excTime <= _Right.excTime;
}
