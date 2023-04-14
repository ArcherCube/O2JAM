#pragma once

#include <windows.h>
#include "CommonType.h"


class Model
{
public:
	//需要使用Model的类基本都会涉及ModelType的区分（比如渲染器）。事实上此处可以将该enum class放到类外定义，但统一化处理，都放到类内。
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

