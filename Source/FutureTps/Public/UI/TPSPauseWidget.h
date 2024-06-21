// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TPSPauseWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class FUTURETPS_API UTPSPauseWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeOnInitialized() override;


	UPROPERTY(meta=(BindWidget))
	UButton * ContinueButton;

private:
	UFUNCTION()
	void OnClearPause();
};