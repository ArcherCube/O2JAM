#include "Recorder.h"
#include "Message.h"

MelodyPieceList* const Recorder::melodyPieceList = new MelodyPieceList();

UINT Recorder::AlignHead()
{
	UINT result = 0;
	UINT tmpResult = 0;

	//melodyPieceList��timestamp��С��������ע��˴�ʹ�õ���list��std::sortͨ����������list
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
		//ʹiterΪtarget��һ��
		++iter;

		//ע��˴�����Ҫ���������������⡣
		//melodyPieceList��timestamp��С����������ĳһ��Ϊt��һ��7�������е�����һ��Ϊa����8����ʵ�����ǵ�7���ҳ�����Ԫ�أ�Ϊb��abΪͬһ����λ��NOTE_ALIGN_MAXΪI
		//��ʱb��a��ʱ����С��NOTE_ALIGN_MAX
		//��֤����������b<t+I����b-a>=I��b>=I+a����t+I>I+a����t>a��������ì��
		//����b-a<I����ͬһ������һ��NOTE_ALIGN_MAX�ڱ������û�����NOTE_ALIGN_MAX���趨������һ��
		for (avgTimestampOfSection = target->timestamp, count = 1; iter != melodyPieceList->cend() && iter->timestamp - target->timestamp < NOTE_ALIGN_MAX; ++count, ++iter)
		{
			avgTimestampOfSection = (avgTimestampOfSection * count + iter->timestamp) / (count + 1);
			//����ʱiter��õ�һ��[target,iter)�����䣬��������Ϊһ�������飬�����ǵ�timestampȫ����Ϊ��ƽ��ֵ
		}

		//����[target,iter)
		tmpResult = 0;
		while (target != iter)
		{
			target->timestamp = avgTimestampOfSection;
			//if(target->delayHashmap.begin()->second< NOTE_DELAY_MIN)target->delayHashmap.begin()->second = 1;
			++target;

			++tmpResult;
		}
		//�������1�Ĳ�����ʵ�ر�����
		result += tmpResult == 1 ? 0 : tmpResult;

		//��ʱtarget==iter==���������������һ��������
	}

	return result;
}

UINT Recorder::AlignTail()
{
	UINT result = 0;
	UINT tmpResult = 0;

	//melodyPieceList��timestamp+delay=releaseTimestamp������β������С��������ע��˴�ʹ�õ���list��std::sortͨ����������list
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
			//ʹiterΪtarget��һ��
			++iter;

			//����ʱiter��õ�һ��[target,iter)�����䣬��������Ϊһ�������飬��delay���е���
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

			//����[target,iter)
			tmpResult = 0;
			while (target != iter)
			{
				//�����������Ķ�delayʹtimestamp+delay=avgReleaseTimestampOfSection
				auto& delay = target->delayHashmap.begin()->second;
				if (delay >= NOTE_DELAY_MIN)
				{
					delay = avgReleaseTimestampOfSection - target->timestamp;

					++tmpResult;
				}
				++target;
			}
			//�������1�Ĳ�����ʵ�ر�����
			result += tmpResult == 1 ? 0 : tmpResult;

			//��ʱtarget==iter==���������������һ��������
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

	//melodyPieceList��timestamp��С��������ע��˴�ʹ�õ���list��std::sortͨ����������list
	melodyPieceList->sort(
		[](const MelodyPiece& left, const MelodyPiece& right)
		{
			return left.timestamp <= right.timestamp;
		}
	);

	MelodyPieceListIter iter = melodyPieceList->begin();
	while (iter != melodyPieceList->cend())
	{
		//ʹiterΪtarget��һ��
		MelodyPieceListIter&& target = iter++;

		//ע��˴�����Ҫ���������������⡣
		//melodyPieceList��timestamp��С����������ĳһ��Ϊt��һ��7�������е�����һ��Ϊa����8����ʵ�����ǵ�7���ҳ�����Ԫ�أ�Ϊb��abΪͬһ����λ��NOTE_ALIGN_MAXΪI
		//��ʱb��a��ʱ����С��NOTE_ALIGN_MAX
		//��֤����������b<t+I����b-a>=I��b>=I+a����t+I>I+a����t>a��������ì��
		//����b-a<I����ͬһ������һ��NOTE_ALIGN_MAX�ڱ������û�����NOTE_ALIGN_MAX���趨������һ��
		//�˴��ϲ�ʱ����timestamp�ϴ󣨿��󣩵�delayΪ׼����Ϊ��һ��������Ϊ����
		while (iter != melodyPieceList->cend() && iter->timestamp == target->timestamp)
		{
			//��ȡ��Ϣ��δ�ϲ�ǰ��ÿ��iter��delayHashmap��ֻ��һ��
			auto& targetDelayHashmap = target->delayHashmap;
			auto& currentKeyName = iter->delayHashmap.begin()->first;
			auto& currentDelay = iter->delayHashmap.begin()->second;

			//�Ƴ�targetDelayHashmap���ܴ��ڵ�currentKeyName������timestamp�ϴ󣨿��󣩵�delayΪ׼����
			targetDelayHashmap.erase(currentKeyName);
			targetDelayHashmap.insert({ currentKeyName,currentDelay });

			//�Ƴ�
			iter = melodyPieceList->erase(iter);

			//�ϲ���һ��
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

	//������������ֻ�û��ʼ���ž��Ѿ������˼�����ʼ���ɿ�����ʱholdTime>releaseTimestamp�������ɣ�����ʱ��>�����Ѿ����ŵ�ʱ����,�򿳵����ֿ�ʼ����֮ǰ�Ĳ���
	const TimestampType pressTimestamp = releaseTimestamp >= holdTime ? releaseTimestamp - holdTime : 0;
	const TimestampType delay = releaseTimestamp >= holdTime ? holdTime : releaseTimestamp;

	//melodyPiece�м�¼���ǡ����¡���ʱ���
	melodyPieceList->push_back(MELODY_PIECE_SINGLE(pressTimestamp, key, delay));

	return SUCCESS;
}

const Melody& Recorder::FinishRecord()
{
	size_t noteCount = melodyPieceList->size();

	//�ȶ���ͷ�������µ�ʱ�����timestamp��
	UINT headCount = AlignHead();

	//�ٶ���β����β����ʱ�����timestamp+delay��
	UINT tailCount = AlignTail();

	//�ϲ������ϲ�Ҳ���ԣ���melodyPieceFile��Ƚϴ�
	UINT combineCount = Combine();

	Message::ShowMessage(
		TEXT("����������") + TO_STRING(noteCount) +
		TEXT("\nͷ�����������") + TO_STRING(headCount) +
		TEXT("\nβ����Ŀǰֻ������������������") + TO_STRING(tailCount) +
		TEXT("\n�ϲ�������") + TO_STRING(combineCount)
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

	//δ�����ж�
	return 0;
}
