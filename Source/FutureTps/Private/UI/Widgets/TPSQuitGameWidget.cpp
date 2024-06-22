// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSQuitGameWidget.h"

#include "Components/Button.h"
#include "Kismet/KismetSystemLibrary.h"

void UTPSQuitGameWidget::NativeOnInitialized()
{
	if (QuitButton) { QuitButton->OnClicked.AddDynamic(this, &UTPSQuitGameWidget::OnQuitGame); }

	Super::NativeOnInitialized();
}

void UTPSQuitGameWidget::OnQuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
