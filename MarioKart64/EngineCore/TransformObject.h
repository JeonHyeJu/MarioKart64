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

	// local
	void SetRelativeScale3D(const FVector& _Value)
	{
		// 절대값이라는 뜻
		// 이게 true가 되면 부모가 있건 없건
		// 100 100 100
		// 10 10 10
		// 나는 무조건 이값에 해당하는 행렬이 되어야 한다는 뜻으로 
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

protected:
	bool IsAbsolute = false;

	FTransform Transform;
	void ParentMatrixCheck();
	UTransformObject* Parent = nullptr;
	std::list<UTransformObject*> Childs;
};

