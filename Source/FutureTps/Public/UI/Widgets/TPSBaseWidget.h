// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSBaseWidget.generated.h"

/**
 * 本游戏所有UI的基类
 */
UCLASS()
class FUTURETPS_API UTPSBaseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void Show();

protected:
	// 
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation *ShowAnimation;
};
