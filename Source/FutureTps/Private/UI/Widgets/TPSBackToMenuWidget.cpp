// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSBackToMenuWidget.h"

#include "Components/Button.h"
#include "GameInstances/TPSGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widgets/TPSPauseWidget.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSBackToMenuWidgetLog, All, All);

void UTPSBackToMenuWidget::NativeOnInitialized()
{
	if (MenuButton) { MenuButton->OnClicked.AddDynamic(this, &UTPSBackToMenuWidget::OnBackToMenu); }

	Super::NativeOnInitialized();
}

void UTPSBackToMenuWidget::OnBackToMenu()
{
	if (!GetWorld()) { return; }


	const auto TPSGameInstance = GetWorld()->GetGameInstance<UTPSGameInstance>();
	if (!TPSGameInstance) { return; }

	if (TPSGameInstance->GetMainMenuMapName().IsNone())
	{
		UE_LOG(MyUTPSBackToMenuWidgetLog, Error, TEXT("MenuLevelName is None"));
		return;
	}

	UGameplayStatics::OpenLevel(this, TPSGameInstance->GetMainMenuMapName());
}
