// FutureTPS Game All Rights Reserved


#include "UI/TPSPlayerStatisticsWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTPSPlayerStatisticsWidget::SetTeamNameText(const FString &TeamName) const
{
	if(!TeamNameText){return;}
	TeamNameText->SetText(FText::FromString(TeamName));
}

void UTPSPlayerStatisticsWidget::SetPlayerNameText(const FString &PlayerName) const
{
	if(!PlayerNameText){return;}
	PlayerNameText->SetText(FText::FromString(PlayerName));
}

void UTPSPlayerStatisticsWidget::SetKillsText(const FString &Kills) const
{
	if(!KillsText){return;}
	KillsText->SetText(FText::FromString(Kills));
}

void UTPSPlayerStatisticsWidget::SetDeathsText(const FString &Deaths) const
{
	if(!DeathsText){return;}
	DeathsText->SetText(FText::FromString(Deaths));
}

void UTPSPlayerStatisticsWidget::SetPlayerIndicatorImageVisibility(const bool Visible) const
{
	if(!PlayerIndicatorImage){return;}
	PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
