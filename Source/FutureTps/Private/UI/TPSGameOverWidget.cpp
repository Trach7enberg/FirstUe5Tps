// FutureTPS Game All Rights Reserved


#include "UI/TPSGameOverWidget.h"

#include "Components/VerticalBox.h"
#include "GameMode/TPSGameModeBase.h"
#include "CoreTypes/CoreType.h"
#include "PlayerStates/TPSBasePlayerState.h"
#include "UI/TPSPlayerStatisticsWidget.h"

bool UTPSGameOverWidget::Initialize()
{
	if (GetWorld())
	{
		const auto GameMode = Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode) { GameMode->OnMatchStateChanged.AddUObject(this, &UTPSGameOverWidget::OnMatchStateChanged); }
	}
	return Super::Initialize();
}

void UTPSGameOverWidget::OnMatchStateChanged(const ETPSMatchState MatchState)
{
	if (MatchState == ETPSMatchState::GameOver) { UpdatePlayerStat(); }
}

void UTPSGameOverWidget::UpdatePlayerStat()
{
	if (!GetWorld() || !StatisticsArea) { return; }
	StatisticsArea->ClearChildren();

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const auto Controller = It->Get();
		if (!Controller) { continue; }

		const auto PlayerState = Cast<ATPSBasePlayerState>(Controller->PlayerState);
		if (!PlayerState) { continue; }

		const auto PlayerStatisticsRow = CreateWidget<UTPSPlayerStatisticsWidget>(
			GetWorld(), PlayerStatisticsRowWidgetClass);
		if (!PlayerStatisticsRow) { continue; }

		PlayerStatisticsRow->SetPlayerNameText(PlayerState->GetPlayerName());
		PlayerStatisticsRow->SetTeamNameText(FString::FromInt(PlayerState->GetTeamID()));
		PlayerStatisticsRow->SetKillsText(FString::FromInt(PlayerState->GetKillSum()));
		PlayerStatisticsRow->SetDeathsText(FString::FromInt(PlayerState->GetDeathSum()));
		PlayerStatisticsRow->SetPlayerIndicatorImageVisibility(Controller->IsPlayerController());

		StatisticsArea->AddChildToVerticalBox(PlayerStatisticsRow);
	}
}
