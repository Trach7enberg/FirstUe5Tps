// FutureTPS Game All Rights Reserved


#include "UI/TPSPauseWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"

bool UTPSPauseWidget::Initialize()
{
	const auto Init = Super::Initialize();

	if (ContinueButton) { ContinueButton->OnClicked.AddDynamic(this, &UTPSPauseWidget::OnClearPause); }

	return Init;
}

void UTPSPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode())
	{
		return;
	}
	GetWorld()->GetAuthGameMode()->ClearPause();
}
