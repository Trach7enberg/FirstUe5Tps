// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSSpectatorHudWidget.h"

#include "Components/TPSRespawnComponent.h"
#include "TPSUtil/TPSUtils.h"

FText UTPSSpectatorHudWidget::GetRespawnCountDownText() const
{
	const auto RespawnComp = FTPSUtils::GetComponentByCurrentPlayer<UTPSRespawnComponent>(GetOwningPlayer());
	if (!RespawnComp) { return FText::FromString(FString("Dead")); }

	return FText::FromString(FString::Printf(TEXT("%.0f"), RespawnComp->GetRespawnTimeCountDown()));
}
