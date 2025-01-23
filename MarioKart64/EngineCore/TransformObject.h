#pragma once

class UTransformObject 
{
public:
	ENGINEAPI UTransformObject();
	ENGINEAPI virtual ~UTransformObject();

	UTransformObject(const UTransformObject& _Other) = delete;
	UTransformObject(UTransformObject&& _Other) noexcept = delete;
	UTransformObject& operator=(const UTransformObject& _Other) = delete;
	UTransformObject& operator=(UTransformObject&& _Other) noexcept = delete;

	void AddRelativeLocation(const FVector& _Value)
	{
		Transform.Location += _Value;
		TransformUpdate();
	}

	void SetWorldLocation(const FVector& _Value)
	{
		IsAbsolute = true;
		Transform.Location = _Value;
		TransformUpdate();
	}

	void AddWorldLocation(const FVector& _Value)
	{
		IsAbsolute = true;
		Transform.Location += _Value;
		TransformUpdate();
	}

	void SetRelativeLocation(const FVector& _Value)
	{
		Transform.Location = _Value;
		TransformUpdate();
	}


	void AddWorldRotation(const FVector& _Value)
	{
		IsAbsolute = true;
		Transform.Rotation += _Value;
		TransformUpdate();
	}

	void AddLocalRotation(const FVector& _Value)
	{
		Transform.Rotation += _Value;
		TransformUpdate();
	}

	void SetRotation(const FVector& _Value)
	{
		Transform.Rotation = _Value;
		TransformUpdate();
	}

	void SetScale3D(const FVector& _Value)
	{
		IsAbsolute = true;
		Transform.Scale = _Value;
		TransformUpdate();
	}

	void SetRelativeScale3D(const FVector& _Value)
	{
		Transform.Scale = _Value;
		Transform.Scale.W = 0.0f;
		TransformUpdate();
	}

	FVector GetWorldScale3D()
	{
		return Transform.WorldScale;
	}

	FVector GetRelativeLocation()
	{
		return Transform.RelativeLocation;
	}
	
	FVector GetWorldLocation()
	{
		return Transform.WorldLocation;
	}
	
	FVector GetWorldForwardVector()
	{
		return Transform.GetWorldFoward();
	}
	
	const FTransform& GetTransform() const
	{
		return Transform;
	}

	FTransform& GetTransformRef()
	{
		return Transform;
	}

	ENGINEAPI virtual void CameraTransUpdate(class UEngineCamera* _Camera);

	ENGINEAPI void SetupAttachment(std::shared_ptr<UTransformObject> _Parent);

	ENGINEAPI void SetupAttachment(UTransformObject* _Parent);

	ENGINEAPI void TransformUpdate();

	UTransformObject* Parent = nullptr;
protected:
	bool IsAbsolute = false;

	FTransform Transform;
	void ParentMatrixCheck();
	std::list<UTransformObject*> Childs;
};

