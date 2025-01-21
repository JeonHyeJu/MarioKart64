#include "PreCompile.h"
#include "SelectGameMode.h"
#include "SelectGame.h"
#include "SelectCharacter.h"
#include "SelectMap.h"
#include "GameData.h"
#include <EngineCore/EngineCamera.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/SpriteRenderer.h>

ASelectGameMode::ASelectGameMode()
{
	SelectGame = GetWorld()->SpawnActor<ASelectGame>();
	SelectGame->SetCallback(std::bind(&ASelectGameMode::OnEndSelectGame, this));
	SelectGame->SetActive(false);

	SelectCharacter = GetWorld()->SpawnActor<ASelectCharacter>();
	SelectCharacter->SetCallback(std::bind(&ASelectGameMode::OnEndSelectCharacter, this));
	SelectCharacter->SetActive(false);
	
	SelectMap = GetWorld()->SpawnActor<ASelectMap>();
	SelectMap->SetCallback(std::bind(&ASelectGameMode::OnEndSelectMap, this));
	SelectMap->SetActive(false);

	std::shared_ptr<ACameraActor> camera = GetWorld()->GetMainCamera();
	camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f });
	camera->GetCameraComponent()->SetProjectionType(EProjectionType::Orthographic);

	Fsm.CreateState(Scene::SELECT_GAME, nullptr, std::bind(&ASelectGameMode::OnShowSelectGame, this));
	Fsm.CreateState(Scene::SELECT_CHARACTER, nullptr, std::bind(&ASelectGameMode::OnShowSelectCharacter, this));
	Fsm.CreateState(Scene::SELECT_MAP, nullptr, std::bind(&ASelectGameMode::OnShowSelectMap, this));
	Fsm.CreateState(Scene::END, nullptr);
}

ASelectGameMode::~ASelectGameMode()
{
}

void ASelectGameMode::BeginPlay()
{
	AActor::BeginPlay();

	Fsm.ChangeState(Scene::SELECT_GAME);
}

void ASelectGameMode::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	// I'm not use Fsm.Update() intentionally. 
	// Because there are not any update functions.
}

/* Fsm start function */
void ASelectGameMode::OnShowSelectGame()
{
	SelectGame->SetActive(true);
	SelectCharacter->SetActive(false);
	SelectMap->SetActive(false);
}

void ASelectGameMode::OnShowSelectCharacter()
{
	SelectGame->SetActive(false);
	SelectCharacter->SetActive(true);
	SelectMap->SetActive(false);
}

void ASelectGameMode::OnShowSelectMap()
{
	SelectGame->SetActive(false);
	SelectCharacter->SetActive(false);
	SelectMap->SetActive(true);
}

/* Callbacks */
void ASelectGameMode::OnEndSelectGame()
{
	Fsm.ChangeState(Scene::SELECT_CHARACTER);
}

void ASelectGameMode::OnEndSelectCharacter()
{
	uint8_t idx = SelectCharacter->GetSelectedIdx();
	int size = static_cast<int>(ECharacter::END);

	std::vector<SPlayerInfo> palyerInfos{ SPlayerInfo{ static_cast<ECharacter>(idx), EItemType::NONE } };
	palyerInfos.reserve(size);
	
	for (int i = 0; i < size; ++i)
	{
		if (i != idx)
		{
			palyerInfos.emplace_back(SPlayerInfo{ static_cast<ECharacter>(i), EItemType::NONE });
		}
	}

	GameData::GetInstance()->SetPlayers(palyerInfos);
	Fsm.ChangeState(Scene::SELECT_MAP);
}

void ASelectGameMode::OnEndSelectMap()
{
	uint8_t idx = SelectMap->GetSelectedIdx();

	std::vector<ECircuit> mapInfo;
	mapInfo.reserve(4);
	for (int i = 0; i < 4; ++i)
	{
		mapInfo.push_back(static_cast<ECircuit>(i+idx));
	}

	UEngineCore::OpenLevel("PlayLevel");
	Fsm.ChangeState(Scene::END);
}