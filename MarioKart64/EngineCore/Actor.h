#pragma once
#include "SceneComponent.h"

class AActor : public UObject
{
	friend class ULevel;

public:
	ENGINEAPI AActor();
	ENGINEAPI ~AActor();

	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	ENGINEAPI virtual void BeginPlay();
	ENGINEAPI virtual void Tick(float _DeltaTime);

	virtual void LevelChangeStart() {}
	virtual void LevelChangeEnd() {}

	template<typename ComponentType>
	inline std::shared_ptr<ComponentType> CreateDefaultSubObject()
	{
		static_assert(std::is_base_of_v<UActorComponent, ComponentType>, "액터 컴포넌트를 상속받지 않은 클래스를 CreateDefaultSubObject하려고 했습니다.");

		if (false == std::is_base_of_v<UActorComponent, ComponentType>)
		{
			MSGASSERT("액터 컴포넌트를 상속받지 않은 클래스를 CreateDefaultSubObject하려고 했습니다.");
			return nullptr;
		}

		char* ComMemory = new char[sizeof(ComponentType)];

		UActorComponent* ComPtr = reinterpret_cast<ComponentType*>(ComMemory);
		ComPtr->Actor = this;

		ComponentType* NewPtr = reinterpret_cast<ComponentType*>(ComMemory);
		std::shared_ptr<ComponentType> NewCom(new(ComMemory) ComponentType());

		if (std::is_base_of_v<UActorComponent, ComponentType> && !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			ActorComponentList.push_back(NewCom);
		}
		else if(!std::is_base_of_v<UActorComponent, ComponentType> && !std::is_base_of_v<USceneComponent, ComponentType>)
		{
			MSGASSERT("말도 안됨");
		}

		return NewCom;
	}

	ULevel* GetWorld()
	{
		return World;
	}

	void SetActorLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetWorldLocation(_Value);
	}

	void AddActorLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->AddWorldLocation(_Value);
	}

	void SetActorRelativeScale3D(const FVector& _Scale)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetRelativeScale3D(_Scale);
	}

	void AddRelativeLocation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->AddRelativeLocation(_Value);
	}

	void SetActorRotation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->SetRotation(_Value);
	}

	void AddActorRotation(const FVector& _Value)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->AddWorldRotation(_Value);
	}

	ENGINEAPI void AttachToActor(AActor* _Parent);

	FVector GetActorLocation()
	{
		return RootComponent->Transform.WorldLocation;
	}

	// 트랜스폼 자체를 고칠수는 없다. 복사본을 주는 함수.
	FTransform GetActorTransform()
	{
		if (nullptr == RootComponent)
		{
			return Base;
		}

		return RootComponent->GetTransformRef();
	}
	
	const FTransform& GetTransform()
	{
		if (nullptr == RootComponent)
		{
			return Base;
		}

		return RootComponent->GetTransform();
	}
	
	void SetActorTransform(const FTransform& _Transform)
	{
		if (nullptr == RootComponent)
		{
			return;
		}

		RootComponent->Transform = _Transform;

		return;
	}

	ENGINEAPI FVector GetActorUpVector();
	ENGINEAPI FVector GetActorRightVector();
	ENGINEAPI FVector GetActorForwardVector();

protected:
	std::shared_ptr<class USceneComponent> RootComponent = nullptr;
	FTransform Base;

private:
	AActor* Parent = nullptr;
	std::list<std::shared_ptr<AActor>> ChildList;
	ULevel* World;

	std::list<std::shared_ptr<class UActorComponent>> ActorComponentList;
};

