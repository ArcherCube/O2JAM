#include "Recorder.h"
#include "Message.h"

MelodyPieceList* const Recorder::melodyPieceList = new MelodyPieceList();

UINT Recorder::AlignHead()
{
	UINT result = 0;
	UINT tmpResult = 0;

	//melodyPieceList按timestamp从小到大排序（注意此处使用的是list，std::sort通用排序不适用list
	melodyPieceList->sort(
		[](const MelodyPiece& left, const MelodyPiece& right)
		{
			return left.timestamp <= right.timestamp;
		}
	);

	TimestampType avgTimestampOfSection = 0;
	UINT count = 0;
	MelodyPieceListIter iter = melodyPieceList->begin();
	MelodyPieceListIter target = iter;
	while (target != melodyPieceList->cend())
	{
		//使iter为target下一个
		++iter;

		//注意此处不需要处理“数量”的问题。
		//melodyPieceList按timestamp从小到大排序，设某一个为t，一组7个按键中的其中一个为a，第8个（实际上是第7个找出来的元素）为b且ab为同一个键位，NOTE_ALIGN_MAX为I
		//此时b与a的时间差必小于NOTE_ALIGN_MAX
		//反证：据题意有b<t+I，若b-a>=I既b>=I+a，则t+I>I+a，即t>a，与题意矛盾
		//所以b-a<I。而同一个键在一个NOTE_ALIGN_MAX内被连续敲击，按NOTE_ALIGN_MAX的设定，算作一次
		for (avgTimestampOfSection = target->timestamp, count = 1; iter != melodyPieceList->cend() && iter->timestamp - target->timestamp < NOTE_ALIGN_MAX; ++count, ++iter)
		{
			avgTimestampOfSection = (avgTimestampOfSection * count + iter->timestamp) / (count + 1);
			//跳出时iter会得到一个[target,iter)的区间，区间内则为一个对齐组，将它们的timestamp全部设为其平均值
		}

		//遍历[target,iter)
		tmpResult = 0;
		while (target != iter)
		{
			target->timestamp = avgTimestampOfSection;
			//if(target->delayHashmap.begin()->second< NOTE_DELAY_MIN)target->delayHashmap.begin()->second = 1;
			++target;

			++tmpResult;
		}
		//区间大于1的才是真实地被对齐
		result += tmpResult == 1 ? 0 : tmpResult;

		//此时target==iter==本次区间以外的下一个迭代器
	}

	return result;
}

UINT Recorder::AlignTail()
{
	UINT result = 0;
	UINT tmpResult = 0;

	//melodyPieceList按timestamp+delay=releaseTimestamp（音符尾部）从小到大排序（注意此处使用的是list，std::sort通用排序不适用list
	melodyPieceList->sort(
		[](const MelodyPiece& left, const MelodyPiece& right)
		{
			return left.timestamp + left.delayHashmap.begin()->second <= right.timestamp + right.delayHashmap.begin()->second;
		}
	);

	TimestampType avgReleaseTimestampOfSection = 0;
	UINT count = 0;
	MelodyPieceListIter iter = melodyPieceList->begin();
	MelodyPieceListIter target = iter;
	while (target != melodyPieceList->cend())
	{
		if (target->delayHashmap.begin()->second >= NOTE_DELAY_MIN)
		{
			//使iter为target下一个
			++iter;

			//跳出时iter会得到一个[target,iter)的区间，区间内则为一个对齐组，对delay进行调整
			for (avgReleaseTimestampOfSection = target->timestamp + target->delayHashmap.begin()->second, count = 1;
				iter != melodyPieceList->cend() && (iter->timestamp + iter->delayHashmap.begin()->second) - (target->timestamp + target->delayHashmap.begin()->second) < NOTE_ALIGN_MAX;
				++count, ++iter)
			{
				auto& delay = iter->delayHashmap.begin()->second;
				if (delay >= NOTE_DELAY_MIN)
				{
					avgReleaseTimestampOfSection = (avgReleaseTimestampOfSection * count + iter->timestamp + delay) / (count + 1);
				}
			}

			//遍历[target,iter)
			tmpResult = 0;
			while (target != iter)
			{
				//对于延音，改动delay使timestamp+delay=avgReleaseTimestampOfSection
				auto& delay = target->delayHashmap.begin()->second;
				if (delay >= NOTE_DELAY_MIN)
				{
					delay = avgReleaseTimestampOfSection - target->timestamp;

					++tmpResult;
				}
				++target;
			}
			//区间大于1的才是真实地被对齐
			result += tmpResult == 1 ? 0 : tmpResult;

			//此时target==iter==本次区间以外的下一个迭代器
		}
		else
		{
			++target;
			++iter;
		}
	}

	return result;
}

UINT Recorder::Combine()
{
	UINT result = 0;

	//melodyPieceList按timestamp从小到大排序（注意此处使用的是list，std::sort通用排序不适用list
	melodyPieceList->sort(
		[](const MelodyPiece& left, const MelodyPiece& right)
		{
			return left.timestamp <= right.timestamp;
		}
	);

	MelodyPieceListIter iter = melodyPieceList->begin();
	while (iter != melodyPieceList->cend())
	{
		//使iter为target下一个
		MelodyPieceListIter&& target = iter++;

		//注意此处不需要处理“数量”的问题。
		//melodyPieceList按timestamp从小到大排序，设某一个为t，一组7个按键中的其中一个为a，第8个（实际上是第7个找出来的元素）为b且ab为同一个键位，NOTE_ALIGN_MAX为I
		//此时b与a的时间差必小于NOTE_ALIGN_MAX
		//反证：据题意有b<t+I，若b-a>=I既b>=I+a，则t+I>I+a，即t>a，与题意矛盾
		//所以b-a<I。而同一个键在一个NOTE_ALIGN_MAX内被连续敲击，按NOTE_ALIGN_MAX的设定，算作一次
		//此处合并时，以timestamp较大（靠后）的delay为准（因为后一个音可能为延音
		while (iter != melodyPieceList->cend() && iter->timestamp == target->timestamp)
		{
			//提取信息。未合并前，每个iter的delayHashmap里只有一项
			auto& targetDelayHashmap = target->delayHashmap;
			auto& currentKeyName = iter->delayHashmap.begin()->first;
			auto& currentDelay = iter->delayHashmap.begin()->second;

			//移除targetDelayHashmap可能存在的currentKeyName（‘以timestamp较大（靠后）的delay为准’）
			targetDelayHashmap.erase(currentKeyName);
			targetDelayHashmap.insert({ currentKeyName,currentDelay });

			//移除
			iter = melodyPieceList->erase(iter);

			//合并了一次
			++result;
		}
	}

	return result;
}

UINT Recorder::StartRecord()
{
	melodyPieceList->clear();

	return SUCCESS;
}

UINT Recorder::Record(const TimestampType& releaseTimestamp, const KeyName& key, const TimestampType& holdTime)
{
	if (!melodyPieceList) return FAIL;

	//特殊情况：音乐还没开始播放就已经按下了键，开始后松开，此时holdTime>releaseTimestamp（或理解成，按键时长>音乐已经播放的时长）,则砍掉音乐开始播放之前的部分
	const TimestampType pressTimestamp = releaseTimestamp >= holdTime ? releaseTimestamp - holdTime : 0;
	const TimestampType delay = releaseTimestamp >= holdTime ? holdTime : releaseTimestamp;

	//melodyPiece中记录的是“按下”的时间戳
	melodyPieceList->push_back(MELODY_PIECE_SINGLE(pressTimestamp, key, delay));

	return SUCCESS;
}

const Melody& Recorder::FinishRecord()
{
	size_t noteCount = melodyPieceList->size();

	//先对齐头部（按下的时间戳，timestamp）
	UINT headCount = AlignHead();

	//再对齐尾部（尾部的时间戳，timestamp+delay）
	UINT tailCount = AlignTail();

	//合并（不合并也可以，但melodyPieceFile会比较大）
	UINT combineCount = Combine();

	Message::ShowMessage(
		TEXT("音符总数：") + TO_STRING(noteCount) +
		TEXT("\n头部对齐次数：") + TO_STRING(headCount) +
		TEXT("\n尾部（目前只针对延音）对齐次数：") + TO_STRING(tailCount) +
		TEXT("\n合并次数：") + TO_STRING(combineCount)
	);

	return Melody(melodyPieceList);
}

UINT Recorder::Clean()
{
	if (melodyPieceList)
	{
		melodyPieceList->clear();
		delete melodyPieceList;
	}

	//未启用判断
	return 0;
}
