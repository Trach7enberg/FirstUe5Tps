#include "GameMode/TPSGameModeBase.h"

#include "Characters/TPSBaseCharacter.h"
#include "Components/TPSRespawnComponent.h"
#include "PlayerStates/TPSBasePlayerState.h"
#include "TPSUtil/TPSUtils.h"
#include "UI/TPSGameHUD.h"
#include "controllers/TPSAIController.h"
#include "controllers/TPSPlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(MyATPSGameModeBaseLog, All, All);

ATPSGameModeBase::ATPSGameModeBase()
{
	DefaultPawnClass = ATPSBaseCharacter::StaticClass();
	PlayerControllerClass = ATPSPlayerController::StaticClass();
	HUDClass = ATPSGameHUD::StaticClass();
	PlayerStateClass = ATPSBasePlayerState::StaticClass();


}

void ATPSGameModeBase::StartPlay()
{
	Super::StartPlay();
	MaxRound = GameData.RoundsCount;
	SpawnBots();
	InitAllPlayerState();
	StartRound();
}

UClass *ATPSGameModeBase::GetDefaultPawnClassForController_Implementation(AController *InController)
{
	// 如果是AI控制器,则返回和控制器相匹配的AI角色类
	if (InController && InController->IsA<AAIController>()) { return AIPawnClass; }

	// 否则返回AI角色类的基类
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ATPSGameModeBase::SpawnBots()
{
	if (!GetWorld()) { return; }

	for (int32 i = 0; i < GameData.PlayersNum - 1; ++i)
	{
		// 调整生成的Actor的位置(如果碰撞的话会调整,总是会生成)
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		const auto AiController = GetWorld()->SpawnActor<ATPSAIController>(AIControllerClass, SpawnInfo);

		// 通过AI控制器生成AI
		RestartPlayer(AiController);
	}
}

void ATPSGameModeBase::StartRound()
{
	CurrentRoundCountDown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ATPSGameModeBase::GameRoundTimerUpdate, 1.0f, true);
}


void ATPSGameModeBase::GameRoundTimerUpdate()
{
	if (--CurrentRoundCountDown <= 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
		if (CurrentRound + 1 <= MaxRound)
		{
			++CurrentRound;

			ResetAllPlayer();
			StartRound();
		}
		else { UE_LOG(MyATPSGameModeBaseLog, Error, TEXT("Over!")); }
	}
}

void ATPSGameModeBase::ResetAllPlayer()
{
	if (!GetWorld()) { return; }

	// 通过世界获取世界中的所有控制器
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It) { ResetPlayer(It->Get()); }
}

void ATPSGameModeBase::ResetPlayer(AController *Controller)
{
	// 如果控制器存在,并且控制器的Pawn存在,则重置Pawn,以便我们能销毁Player,再重新生成
	if (Controller && Controller->GetPawn())
	{

		// 手动destroy控制器的Pawn
		Controller->GetPawn()->Reset();
	}

	RestartPlayer(Controller);
	SetPlayerColor(Controller);
}

void ATPSGameModeBase::InitAllPlayerState()
{
	if (!GetWorld()) { return; }

	int32 TeamID = GameData.TeamIDs[0];
	// 通过世界获取世界中的所有控制器
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) { continue; }

		const auto Character = Cast<ATPSBaseCharacter>(Controller->GetCharacter());
		if (!Character) { continue; }

		const auto PlayerState = Controller->GetPlayerState<ATPSBasePlayerState>();
		if (!PlayerState) { continue; }

		PlayerState->SetTeamID(TeamID);
		PlayerState->SetTeamColor(GetColorByTeamID(TeamID));
		// 设置玩家的名字
		PlayerState->SetPlayerName(Character->GetCharacterName());

		SetPlayerColor(Controller);

		// 有多少个团队取决于团队ID数组中有几个元素
		TeamID = GameData.TeamIDs[(TeamID + 1) % (GameData.TeamIDs.Num())];
	}

}

FLinearColor ATPSGameModeBase::GetColorByTeamID(const int32 TeamID)
{
	if (GameData.TeamColors.IsValidIndex(TeamID)) { return GameData.TeamColors[TeamID]; }

	UE_LOG(MyATPSGameModeBaseLog, Error, TEXT("Wrong Index!"));
	return GameData.DefaultTeamColor;
}

void ATPSGameModeBase::SetPlayerColor(const AController *Controller)
{
	if (!Controller) { return; }

	const auto BaseCharacter = Cast<ATPSBaseCharacter>(Controller->GetPawn());
	if (!BaseCharacter) { return; }

	const auto PlayerState = BaseCharacter->GetPlayerState<ATPSBasePlayerState>();
	if (!PlayerState) { return; }

	BaseCharacter->SetCharacterColor(PlayerState->GetTeamColor());
}


void ATPSGameModeBase::KillPlayer(AController *Killer, AController *Victim)
{
	const auto KillerPlayerState = Killer ? Killer->GetPlayerState<ATPSBasePlayerState>() : nullptr;
	const auto VictimPlayerState = Victim ? Victim->GetPlayerState<ATPSBasePlayerState>() : nullptr;

	if (KillerPlayerState) { KillerPlayerState->AddKill(); }

	if (VictimPlayerState) { VictimPlayerState->AddDeath(); }

	// 准备重生被击杀的玩家
	StartRespawn(Victim);
}


void ATPSGameModeBase::StartRespawn(AController *Controller)
{
	const auto RespawnComp = FTPSUtils::GetComponentByCurrentPlayer<UTPSRespawnComponent>(Controller);
	if(!RespawnComp){return;}

	RespawnComp->Respawn(GameData.RespawnTime);
}

void ATPSGameModeBase::RespawnPlayer(AController *Controller)
{
	ResetPlayer(Controller);
}

void ATPSGameModeBase::LogPlayerStates() const
{
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) { continue; }

		const auto PlayerState = Controller->GetPlayerState<ATPSBasePlayerState>();
		if (!PlayerState) { continue; }

		PlayerState->LogInfo();
	}
}
