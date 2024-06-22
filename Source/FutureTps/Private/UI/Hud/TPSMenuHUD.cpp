// FutureTPS Game All Rights Reserved


#include "UI/Hud//TPSMenuHUD.h"

#include "Blueprint/UserWidget.h"

void ATPSMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	const auto MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
	if (MenuWidget) { MenuWidget->AddToViewport(); }
}
