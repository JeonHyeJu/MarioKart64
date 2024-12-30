#pragma once
#include "ActorComponent.h"

class USceneComponent : public UActorComponent
{
	friend class AActor;

public:
	USceneComponent();
	virtual ~USceneComponent() = 0;

	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	void AddRelativeLocation(const FVector& _Value)
	{
		OutputDebugStringA(("2. Actor.AddRotation: " + std::to_string(Transform.Rotation.X) + ", " + std::to_string(Transform.Rotation.Y) + ", " + std::to_string(Transform.Rotation.Z) + "\n\n").c_str());
		Transform.Location += _Value;
		TransformUpdate();
	}

	void SetWorldLocation(const FVector& _Value)
	{
		IsAbsolute = true;
		Transform.Location = _Value;
		TransformUpdate();
	}

	void SetRelativeLocation(const FVector& _Value)
	{
		Transform.Location = _Value;
		TransformUpdate();
	}

	void AddRotation(const FVector& _Value)
	{
		Transform.Rotation += _Value;
		OutputDebugStringA(("1. Actor.AddRotation: " + std::to_string(Transform.Rotation.X) + ", " + std::to_string(Transform.Rotation.Y) + ", " + std::to_string(Transform.Rotation.Z) + "\n").c_str());
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

	const FTransform& GetTransform()
	{
		return Transform;
	}

	FTransform& GetTransformRef()
	{
		return Transform;
	}

	ENGINEAPI void SetupAttachment(std::shared_ptr<USceneComponent> _Parent);

	void SetupAttachment(USceneComponent* _Parent);

	ENGINEAPI void TransformUpdate();

	ENGINEAPI USceneComponent* GetParent() const
	{
		return Parent;
	}

protected:
	bool IsAbsolute = false;

	FTransform Transform;

	ENGINEAPI void BeginPlay() override;

	void ParentMatrixCheck();

private:
	USceneComponent* Parent = nullptr;
	std::list<std::shared_ptr<USceneComponent>> Childs;
};

