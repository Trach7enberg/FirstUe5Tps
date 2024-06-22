// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSPauseWidget.h"

#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"


DEFINE_LOG_CATEGORY_STATIC(MyUTPSPauseWidgetLog, All, All);

void UTPSPauseWidget::NativeOnInitialized()
{
	if (ContinueButton) { ContinueButton->OnClicked.AddDynamic(this, &UTPSPauseWidget::OnClearPause); }

	

	Super::NativeOnInitialized();
}

void UTPSPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) { return; }
	GetWorld()->GetAuthGameMode()->ClearPause();
}

