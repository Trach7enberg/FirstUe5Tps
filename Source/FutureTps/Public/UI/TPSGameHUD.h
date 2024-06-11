// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TPSGameHUD.generated.h"

class UTPSPlayerHUDWidget;
class UUserWidget;
/**
 * 用户HUD管理类
 */
UCLASS()
class FUTURETPS_API ATPSGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<UUserWidget> PlayHUDWidgetClass;

	virtual void BeginPlay() override;

private:
	void DrawCrossHair();
};
