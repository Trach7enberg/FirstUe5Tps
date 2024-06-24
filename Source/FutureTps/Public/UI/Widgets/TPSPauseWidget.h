// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TPSBaseWidget.h"
#include "TPSPauseWidget.generated.h"

class UButton;
/**
 * 暂停界面UI
 */
UCLASS()
class FUTURETPS_API UTPSPauseWidget : public UTPSBaseWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeOnInitialized() override;
	
	// 继续游戏按钮
	UPROPERTY(meta=(BindWidget))
	UButton * ContinueButton;

	
	

private:
	UFUNCTION()
	void OnClearPause();

};
