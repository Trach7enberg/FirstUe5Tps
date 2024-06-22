// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSBackToMenuWidget.generated.h"

class UButton;
/**
 * 返回主菜单界面的UI(实际是一个按钮)
 */
UCLASS()
class FUTURETPS_API UTPSBackToMenuWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeOnInitialized() override;
	
	// 返回主菜单按钮
	UPROPERTY(meta=(BindWidget))
	UButton* MenuButton;

private:
	
	UFUNCTION()
	void OnBackToMenu();
};
