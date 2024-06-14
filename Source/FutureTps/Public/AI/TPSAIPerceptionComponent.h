// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "TPSAIPerceptionComponent.generated.h"

/**
 * AI感知组件
 */
UCLASS()
class FUTURETPS_API UTPSAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	/// 获取离AI最靠近的玩家
	/// @return 玩家的Actor
	AActor *GetClosestEnemy() const;
};
