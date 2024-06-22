// FutureTPS Game All Rights Reserved


#include "UI/Widgets/TPSMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UTPSMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (StartGameButton) { StartGameButton->OnClicked.AddDynamic(this, &UTPSMenuWidget::OnStartGame); }


}

void UTPSMenuWidget::OnStartGame()
{
	if(GetWorld())
	{
		UGameplayStatics::OpenLevel(this,StartLevelName);
	}
}
