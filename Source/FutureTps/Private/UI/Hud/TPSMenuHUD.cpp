// FutureTPS Game All Rights Reserved


#include "UI/Hud//TPSMenuHUD.h"

#include "UI/Widgets/TPSBaseWidget.h"

void ATPSMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	const auto MenuWidget = CreateWidget<UTPSBaseWidget>(GetWorld(), MenuWidgetClass);
	if (MenuWidget)
	{
		MenuWidget->AddToViewport();
		MenuWidget->Show();
	}
}
