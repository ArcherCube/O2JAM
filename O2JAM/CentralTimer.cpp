#include "CentralTimer.h"

#pragma comment( lib,"winmm.lib" )



CentralTimer::TimerSet* const CentralTimer::timerSet = new CentralTimer::TimerSet();
CentralTimer::TimerHashmap* const CentralTimer::timerHashmap = new CentralTimer::TimerHashmap();
/*LARGE_INTEGER CentralTimer::freq = { 0 };
LARGE_INTEGER CentralTimer::time = { 0 };*/

UINT CentralTimer::Add(const String& tag, const TimestampType& waitTime, const function<void()>& exc)
{
	//�����tag�Ѿ���������ɾ��ԭ����
	if (timerHashmap->find(tag) != timerHashmap->cend())
	{
		//����hashmap�е�ֵ��set�ĵ���������ɾ��set�еļ�¼
		timerSet->erase(timerHashmap->at(tag));

		//����keyɾ��hashmap�еļ�¼
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
	/*QueryPerformanceFrequency(&freq); // ��ȡʱ������
	QueryPerformanceCounter(&time); // ��ȡʱ�Ӽ���

	return (time.QuadPart * 1000 / freq.QuadPart);*/
	return timeGetTime();
}

void CentralTimer::Tick()
{
	TimestampType timeStamp = GetTimeStamp();

	for (TimerSetIter iter = timerSet->begin(); iter != timerSet->end();)
	{
		//��timer������ִ��ʱ�����ڵ�ǰʱ���ʱ��ִ�в�ɾ��timer�������˳�ѭ������ΪtimerSet��timeStamp����һ�������ϣ������ȫ�������ϣ�
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
