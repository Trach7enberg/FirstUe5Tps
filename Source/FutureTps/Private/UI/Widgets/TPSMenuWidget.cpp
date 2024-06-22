// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSMenuWidget.h"

#include "Components/Button.h"
#include "GameInstances/TPSGameInstance.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(MyUTPSMenuWidgetLog, All, All);

void UTPSMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (StartGameButton) { StartGameButton->OnClicked.AddDynamic(this, &UTPSMenuWidget::OnStartGame); }

}

void UTPSMenuWidget::OnStartGame()
{
	if (!GetWorld()) { return; }

	
	const auto TPSGameInstance = GetWorld()->GetGameInstance<UTPSGameInstance>();
	if (!TPSGameInstance) {  return; }

	if (TPSGameInstance->GetDefaultMapName().IsNone())
	{
		UE_LOG(MyUTPSMenuWidgetLog, Error, TEXT("StartLevelName is None"));
		return;
	}

	UGameplayStatics::OpenLevel(this, TPSGameInstance->GetDefaultMapName());

}
