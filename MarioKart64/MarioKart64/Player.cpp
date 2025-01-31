#include "PreCompile.h"
#include "Player.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/Collision.h>
#include "BaseMap.h"
#include "GameData.h"
#include "CData.h"
#include "Item.h"
#include "CGlobal.h"

APlayer::APlayer()
{
	IsAutomative = false;
	TrainDatas.reserve(10000);
}

APlayer::~APlayer()
{
	// Temp
	/*std::ofstream f;
	f.open("temp.txt");
	f.write(FileTemp.str().c_str(), FileTemp.str().size());
	f.close();*/
}

void APlayer::BeginPlay()
{
	ADriver::BeginPlay();
}

void APlayer::Tick(float _deltaTime)
{
	ADriver::Tick(_deltaTime);
	TickItem(_deltaTime);

	DirVTrain = 0;
	DirHTrain = 0;
	//OutputDebugStringA(("Player loc: " + GetActorLocation().ToString() + "\n").c_str());

	/* for debug start */
	if (UEngineInput::IsDown(VK_SPACE))
	{
		UseItem_Shell(EItemType::GREEN_SHELL);
	//	//IsSpin = true;

	//	/*std::shared_ptr<AItem> item = GetWorld()->SpawnActor<AItem>();
	//	item->Init(EItemType::FAKE_ITEMBOX);
	//	item->SetActorLocation(GetActorLocation() + FVector{ -item->Size * .5f, item->Size, 0.f });
	//	item->SetDirection(GetActorForwardVector());*/
	}
	if (UEngineInput::IsDown(VK_F8))
	{
		IsTraining = !IsTraining;

		if (!IsTraining)
		{
			FILE* file;
			fopen_s(&file, "train_data_.txt", "a");

			if (file != nullptr)
			{
				fwrite("[", 1, 1, file);
				for (size_t i = 0, size = TrainDatas.size(); i < size; ++i)
				{
					std::string str = TrainDatas[i].ToString();
					if (i < size - 1)
					{
						str += ",";
					}
					fwrite(str.c_str(), str.size(), 1, file);
				}
				TrainDatas.clear();
				fwrite("]", 1, 1, file);
				fclose(file);
			}
		}
	}
	else if (UEngineInput::IsDown(VK_F7))
	{
		IsReverseTrain = !IsReverseTrain;
	}
	/* for debug end */

	if (IsTraining)
	{
		static float elaspedSecTrain = 0.f;

		elaspedSecTrain += _deltaTime;

		if (elaspedSecTrain > .0001f)
		{
			elaspedSecTrain = 0.f;

			GameData* pData = GameData::GetInstance();
			FVector loc = GetActorLocation();

			float subX = pData->MapSizeInfo.Max.X - pData->MapSizeInfo.Min.X;
			float subZ = pData->MapSizeInfo.Max.Z - pData->MapSizeInfo.Min.Z;
			float normX = (loc.X - pData->MapSizeInfo.Min.X) / subX;
			float normZ = (loc.Z - pData->MapSizeInfo.Min.Z) / subZ;

			TrainData data
			{
				pData->MapSizeInfo.InitLoc,
				GetActorForwardVector(),
				normX,
				normZ,
				GetTransform().Rotation.Y,
				IsReverseTrain ? 1 : 0,
				Velocity,
				DirVTrain,
				DirHTrain
			};
			TrainDatas.push_back(data);
		}
	}
}

void APlayer::GetHandleRotation(float _deltaTime, float& _refRot)
{
	if (UEngineInput::IsPress(VK_LEFT))
	{
		DirHTrain = 1;
		_refRot = -65.f * _deltaTime;
	}
	else if (UEngineInput::IsPress(VK_RIGHT))
	{
		DirHTrain = 2;
		_refRot = 65.f * _deltaTime;
	}
	//OutputDebugStringA(("rotVal: " + std::to_string(_refRot) + "\n").c_str());
}

void APlayer::TickItem(float _deltaTime)
{
	static const int NONE = static_cast<int>(EItemType::SIZE);
	if (ItemIndex >= NONE) return;

	if (UEngineInput::IsDown(VK_SPACE))
	{
		UseItem();
	}
}