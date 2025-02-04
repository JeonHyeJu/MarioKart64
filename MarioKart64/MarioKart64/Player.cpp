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

	TestForDebug(_deltaTime);
	GatherTrainigData(_deltaTime);
}

void APlayer::StartDriving()
{
	BoostSP = UEngineSound::Play("Boost.mp3");
	BoostSP.SetVolume(.0f);
}

void APlayer::Driving(float _deltaTime)
{
	if (Velocity < 5)
	{
		if (BoostSP.IsPlaying())
		{
			BoostSP.Stop();
		}
	}
	else if (Velocity < 200)
	{
		if (BoostSP.IsPlaying() == false)
		{
			BoostSP = UEngineSound::Play("Boost.mp3");
			BoostSP.SetVolume(.1f);
		}
	}
	else
	{
		if (BoostSP.IsPlaying() == false)
		{
			BoostSP = UEngineSound::Play("Boost.mp3");
			BoostSP.SetVolume(.5f);
		}
	}
}

void APlayer::FinishDriving()
{
	BoostSP.Stop();
}

void APlayer::PlayHandleSound()
{
	if (WheelSP.IsInited() == false)
	{
		WheelSP = UEngineSound::Play("Drift.wav");
		WheelSP.Loop(9999);
		WheelSP.SetVolume(.1f);
	}
	else
	{
		WheelSP.Resume();
	}
}

void APlayer::GetHandleRotation(float _deltaTime, float& _refRot)
{
	float rotY = GetActorRotation().Y;

	if (UEngineInput::IsPress(VK_LEFT))
	{
		DirHTrain = 1;
		if (PrevH != 0)
		{
			Renderer->SetRotation({ 0.f, 0.f, 0.f });
			Renderer->ChangeAnimation("TurnL");
			PrevH = 0;
			WheelVelocity = 0.f;
		}

		WheelVelocity = FPhysics::GetVf(WheelVelocity, -WHEEL_ACCEL, _deltaTime);
		_refRot = FPhysics::GetDeltaX(WheelVelocity, -WHEEL_ACCEL, _deltaTime);

		if (WheelVelocity < -30)
		{
			PlayHandleSound();
		}
	}
	else if (UEngineInput::IsPress(VK_RIGHT))
	{
		DirHTrain = 2;
		if (PrevH != 1)
		{
			Renderer->SetRotation({ 0.f, 180.f, 0.f });
			Renderer->ChangeAnimation("TurnR");
			PrevH = 1;
			WheelVelocity = 0.f;
		}
		
		WheelVelocity = FPhysics::GetVf(WheelVelocity, WHEEL_ACCEL, _deltaTime);
		_refRot = FPhysics::GetDeltaX(WheelVelocity, WHEEL_ACCEL, _deltaTime);

		if (WheelVelocity > 30)
		{
			PlayHandleSound();
		}
	}
	else
	{
		float acc = -1.f * WheelVelocity;
		if (PrevH != 2)
		{
			Renderer->SetRotation({ 0.f, 0.f, 0.f });
			WheelVelocity = FPhysics::GetVf(WheelVelocity, acc, _deltaTime);
			_refRot = FPhysics::GetDeltaX(WheelVelocity, acc, _deltaTime);
		}

		WheelSP.Pause();

		PrevH = 2;
		Renderer->ChangeAnimation("Idle");
	}

	//OutputDebugStringA(("rotVal: " + std::to_string(_refRot) + "\n").c_str());
}

void APlayer::TickItem(float _deltaTime)
{
	if (ItemIndex >= ITEM_NONE) return;

	if (UEngineInput::IsDown(VK_SPACE))
	{
		UseItem();
		PlayShootVoice();
		GameData::GetInstance()->SetPlayerItem(static_cast<EItemType>(ItemIndex));
	}
}

void APlayer::TestForDebug(float _deltaTime)
{
	//if (UEngineInput::IsDown(VK_SPACE))
	//{
		//UseItem_Shell(EItemType::GREEN_SHELL);
		//UseItem_Shell(EItemType::BOWSER_SHELL);
		//UseItem_Banana(EItemType::BANANA);
		//UseItem_FakeItemBox(EItemType::FAKE_ITEMBOX);
		//UseItem_Mushroom(EItemType::MUSHROOM);
		//IsSpin = true;
	//}
	if (UEngineInput::IsPress(VK_LSHIFT))
	{
		Velocity = 1000.f;
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
}

void APlayer::GatherTrainigData(float _deltaTime)
{
	if (!IsTraining) return;

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

void APlayer::OnChangeLap(int _lap)
{
	GameData::GetInstance()->SetPlayerLap(_lap);

	USoundPlayer sp = UEngineSound::Play("PassLap.mp3");
	sp.SetVolume(.5f);
}

void APlayer::OnChangeItem(int _itemIdx)
{
	GameData::GetInstance()->SetPlayerItem(static_cast<EItemType>(_itemIdx));
}

void APlayer::OnCollisionEnter(UCollision* _this, UCollision* _other)
{
	ADriver::OnCollisionEnter(_this, _other);

	const std::string& name = _other->GetCollisionProfileName();
	if (name == "ITEMBOX")
	{
		USoundPlayer sp = UEngineSound::Play("ItemRoulette.mp3");
		sp.SetVolume(.5f);
	}
}

void APlayer::EndLap()
{
	IsAutomative = true;
}