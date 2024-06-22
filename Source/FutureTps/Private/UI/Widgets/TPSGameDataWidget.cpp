// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSGameDataWidget.h"

#include "GameMode/TPSGameModeBase.h"
#include "TPSUtil/TPSUtils.h"

auto UTPSGameDataWidget::Initialize() -> bool { return Super::Initialize(); }

FText UTPSGameDataWidget::GetRoundText() const
{
	const auto GameMode = GetAtpsGameMode();

	if (!GameMode) { return FText(); }

	return FText::FromString(
		FString::FromInt(GameMode->GetCurrentRound()) + " / " + FString::FromInt(GameMode->GetMaxRound()));
}

FText UTPSGameDataWidget::GetKillText() const
{


	const auto PlayerState = GetPlayerState();
	if (!PlayerState) { return FText(); }


	return FText::FromString(FString::FromInt(PlayerState->GetKillSum()));
}

FText UTPSGameDataWidget::GetCountDownText() const
{

	const auto GameMode = GetAtpsGameMode();

	if (!GameMode) { return FText(); }

	const int32 Seconds = GameMode->GetRoundSecondsRemaining();
	// int32 hours = Seconds / 3600;
	const int32 Minutes = (Seconds % 3600) / 60;
	const int32 Sec = Seconds % 60;

	// 将小时、分钟和秒转换为两位数格式

	const FString FormattedMinutes = FString::Printf(TEXT("%02d"), Minutes);
	const FString FormattedSeconds = FString::Printf(TEXT("%02d"), Sec);

	return FText::FromString(FormattedMinutes + ":" + FormattedSeconds);
}

ATPSGameModeBase *UTPSGameDataWidget::GetAtpsGameMode() const
{
	return GetWorld() ? Cast<ATPSGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ATPSBasePlayerState *UTPSGameDataWidget::GetPlayerState() const
{
	return GetOwningPlayerPawn()
		? GetOwningPlayerPawn()->GetController()->GetPlayerState<ATPSBasePlayerState>()
		: nullptr;
}
