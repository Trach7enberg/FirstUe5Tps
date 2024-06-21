// FutureTPS Game All Rights Reserved


#include "UI/TPSPauseWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"



void UTPSPauseWidget::NativeOnInitialized()
{
	if (ContinueButton) { ContinueButton->OnClicked.AddDynamic(this, &UTPSPauseWidget::OnClearPause); }
	Super::NativeOnInitialized();
}

void UTPSPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode())
	{
		return;
	}
	GetWorld()->GetAuthGameMode()->ClearPause();
}
