
// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPSPlayerController.generated.h"

class UTPSRespawnComponent;
/**
 * 
 */
UCLASS()
class FUTURETPS_API ATPSPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPSPlayerController();
	// 重生组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)
	UTPSRespawnComponent* RespawnComponent;
};
