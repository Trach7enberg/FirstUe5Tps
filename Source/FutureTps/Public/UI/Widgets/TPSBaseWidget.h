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
	// 播放打开界面时相应的一系列动画
	void Show();

protected:
	// 播放显示动画
	UPROPERTY(meta=(BindWidgetAnim), Transient)
	UWidgetAnimation *ShowAnimation;
};
