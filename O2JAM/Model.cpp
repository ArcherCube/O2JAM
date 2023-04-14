#include "Model.h"

Model::Model(const HANDLE& handle, const ModelType& modelType) : handle(handle), modelType(modelType)
{}


const HANDLE Model::GetHandle() const
{
	return handle;
}

const Model::ModelType Model::GetModelType() const
{
	return modelType;
}


Model::~Model()
{
	DeleteObject(handle);
}