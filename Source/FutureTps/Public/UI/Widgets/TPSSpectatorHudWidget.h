// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSSpectatorHudWidget.generated.h"

/**
 * 
 */
UCLASS()
class FUTURETPS_API UTPSSpectatorHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/// 获取重生倒计时
	/// @return 
	UFUNCTION(BlueprintCallable, Category=UI)
	FText GetRespawnCountDownText() const;
};
