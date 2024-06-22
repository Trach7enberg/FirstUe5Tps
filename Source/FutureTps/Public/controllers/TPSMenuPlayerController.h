// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSMenuPlayerController.generated.h"

/**
 * 主菜单的玩家控制器
 */
UCLASS()
class FUTURETPS_API ATPSMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
