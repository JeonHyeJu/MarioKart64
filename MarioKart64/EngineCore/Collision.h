#pragma once
#include "SceneComponent.h"
#include "EngineSprite.h"
#include "RenderUnit.h"
#include <set>

class UCollision : public USceneComponent
{
public:
	ENGINEAPI UCollision();
	ENGINEAPI ~UCollision();

	UCollision(const UCollision& _Other) = delete;
	UCollision(UCollision&& _Other) noexcept = delete;
	UCollision& operator=(const UCollision& _Other) = delete;
	UCollision& operator=(UCollision&& _Other) noexcept = delete;

	ENGINEAPI void SetCollisionProfileName(std::string_view _ProfileName);
	ENGINEAPI void BeginPlay() override;

	std::string GetCollisionProfileName()
	{
		return ProfileName;
	}

	void SetRadius(float _Value);

	ENGINEAPI bool CollisionCheck(std::string_view _OtherName, std::vector<UCollision*>& _Vector);

	void SetCollisionType(ECollisionType _Type)
	{
		CollisionType = _Type;
	}

	void CollisionEventCheck(std::shared_ptr<UCollision> _Other);

	ENGINEAPI void SetCollisionEnter(std::function<void(UCollision*, UCollision*)> _Function);
	ENGINEAPI void SetCollisionStay(std::function<void(UCollision*, UCollision*)> _Function);
	ENGINEAPI void SetCollisionEnd(std::function<void(UCollision*, UCollision*)> _Function);

	bool IsEvent()
	{
		return Enter != nullptr || Stay != nullptr || End != nullptr;
	}

private:
	ECollisionType CollisionType = ECollisionType::OBB2D;

	std::set<UCollision*> CollisionCheckSet;

	std::string ProfileName = "NONE";

	std::function<void(UCollision*, UCollision*)> Enter;

	std::function<void(UCollision*, UCollision*)> Stay;

	std::function<void(UCollision*, UCollision*)> End;
};

