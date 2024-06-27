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
	// 根据所有以配置的感知配置来综合考虑获取离AI最近的玩家
	AActor *GetClosestEnemy(const AAIController *AiController) const;

private:
	/// 根据感知配置获得离AI最靠近的玩家
	/// @param SenseToUse 使用的感知配置
	/// @return 玩家的Actor
	AActor *GetClosestEnemy(TSubclassOf<UAISense> SenseToUse) const;

};
