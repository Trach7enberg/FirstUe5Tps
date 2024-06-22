// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSQuitGameWidget.generated.h"

class UButton;
/**
 * 退出游戏的UI
 */
UCLASS()
class FUTURETPS_API UTPSQuitGameWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	
	// 退出游戏按钮
	UPROPERTY(meta=(BindWidget))
	UButton* QuitButton;

private:
	
	UFUNCTION()
	void OnQuitGame();
};
