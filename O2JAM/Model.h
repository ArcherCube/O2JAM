#pragma once

#include <windows.h>
#include "CommonType.h"


class Model
{
public:
	//��Ҫʹ��Model������������漰ModelType�����֣�������Ⱦ��������ʵ�ϴ˴����Խ���enum class�ŵ����ⶨ�壬��ͳһ���������ŵ����ڡ�
	enum class ModelType
	{
		BITMAP = 0,
		TEXT = 1
	};

protected:
	HANDLE const handle;
	ModelType const modelType;
public:
	explicit Model(const HANDLE& handle,const ModelType& modelType);

	const HANDLE GetHandle() const;
	const ModelType GetModelType() const;

	virtual ~Model();
};

