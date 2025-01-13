#include "PreCompile.h"
#include "TransformObject.h"

UTransformObject::UTransformObject()
{
}

UTransformObject::~UTransformObject()
{
}

void UTransformObject::SetupAttachment(std::shared_ptr<UTransformObject> _Parent)
{
	SetupAttachment(_Parent.get());
}

void UTransformObject::ParentMatrixCheck()
{
	if (nullptr != Parent)
	{
		Transform.ParentMat = Parent->Transform.World;
	}
}

void UTransformObject::SetupAttachment(UTransformObject* _Parent)
{
	Parent = _Parent;
	Parent->Childs.push_back(this);

	TransformUpdate();
}

void UTransformObject::TransformUpdate()
{
	ParentMatrixCheck();
	Transform.TransformUpdate(IsAbsolute);

	for (UTransformObject* Child : Childs)
	{
		Child->TransformUpdate();
	}

	IsAbsolute = false;
}

