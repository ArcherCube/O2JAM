#include "Melody.h"
#include "FileUtil.h"

#include <iostream>


Melody::Melody(MelodyPieceList* const melodyPieceList) :melodyPieceList(melodyPieceList)
{
	this->loaded = true;
}
Melody::Melody(const String& musicFilename) :melodyPieceList(new MelodyPieceList())
{
	const auto& result = FileUtil::Read(Path::melodyPath + musicFilename + MELODY_FILE_SUFFIX, [this](const String& line)
		{
			if (!line.empty())
			{
				//�ǻ�������ȡ����
				UINT delayA, delayS, delayD, delaySPACE, delayJ, delayK, delayL;
				TimestampType timestamp;
				if (Istringstream(line) >> timestamp >> delayA >> delayS >> delayD >> delaySPACE >> delayJ >> delayK >> delayL)
				{
					this->melodyPieceList->push_back(MELODY_PIECE_FULL(timestamp, delayA, delayS, delayD, delaySPACE, delayJ, delayK, delayL));
				}
			}
		}
	);

	this->loaded = result == SUCCESS;
}

String& Melody::ToText() const
{
	Ostringstream oss;
	for (const auto& melodyPiece : *(this->melodyPieceList))
	{
		//�Ѽ�¼�˰���ʱ����hashmap��ȡ����
		const auto& delayHashmap = melodyPiece.delayHashmap;
		//Ѱ�����Ӧ�İ���ʱ����¼Ԫ�صĵ�����
		const auto& iterA = delayHashmap.find(KeyName::A);
		const auto& iterS = delayHashmap.find(KeyName::S);
		const auto& iterD = delayHashmap.find(KeyName::D);
		const auto& iterSPACE = delayHashmap.find(KeyName::SPACE);
		const auto& iterJ = delayHashmap.find(KeyName::J);
		const auto& iterK = delayHashmap.find(KeyName::K);
		const auto& iterL = delayHashmap.find(KeyName::L);

		//����ʽд��һ��
		oss << melodyPiece.timestamp << " "
			<< (iterA == delayHashmap.cend() ? 0 : iterA->second) << " "
			<< (iterS == delayHashmap.cend() ? 0 : iterS->second) << " "
			<< (iterD == delayHashmap.cend() ? 0 : iterD->second) << " "
			<< (iterSPACE == delayHashmap.cend() ? 0 : iterSPACE->second) << " "
			<< (iterJ == delayHashmap.cend() ? 0 : iterJ->second) << " "
			<< (iterK == delayHashmap.cend() ? 0 : iterK->second) << " "
			<< (iterL == delayHashmap.cend() ? 0 : iterL->second) << std::endl;
	}

	String result;

	oss.str(result);

	return result;
}

bool Melody::IsLoaded() const
{
	return this->loaded;
}

const MelodyPieceList* Melody::getMelodyPieceList() const
{
	return this->melodyPieceList;
}