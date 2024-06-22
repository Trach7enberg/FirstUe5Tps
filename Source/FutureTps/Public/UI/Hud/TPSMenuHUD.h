// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TPSMenuHUD.generated.h"

/**
 * 管理主菜单小部件UI的HUD类
 */
UCLASS()
class FUTURETPS_API ATPSMenuHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<UUserWidget> MenuWidgetClass;
};
