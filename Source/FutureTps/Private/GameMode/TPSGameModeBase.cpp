#include "GameMode/TPSGameModeBase.h"

#include "Characters/TPSBaseCharacter.h"
#include "PlayerStates/TPSBasePlayerState.h"
#include "UI/TPSGameHUD.h"
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
	CurrentRound = 1;

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

		AAIController *AaiController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);

		// 通过AI控制器生成AI
		RestartPlayer(AaiController);
	}
}

void ATPSGameModeBase::StartRound()
{
	RoundCountDown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ATPSGameModeBase::GameRoundTimerUpdate, 1.0f, true);
}


void ATPSGameModeBase::GameRoundTimerUpdate()
{
	if (--RoundCountDown <= 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);
		if (CurrentRound + 1 <= GameData.RoundsCount)
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

		const auto PlayerState = Controller->GetPlayerState<ATPSBasePlayerState>();
		if (!PlayerState) { continue; }

		PlayerState->SetTeamID(TeamID);
		PlayerState->SetTeamColor(GetColorByTeamID(TeamID));
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
