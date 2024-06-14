// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TPSFindEnemyService.generated.h"

/**
 * 行为树中的AIService类 
 * 当前类的作用是找到敌人的位置,将它的值设置到黑板上
 */
UCLASS()
class FUTURETPS_API UTPSFindEnemyService : public UBTService
{
	GENERATED_BODY()

public:
	UTPSFindEnemyService();

protected:
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;

	// 在黑板中选择要更新的值,当前Service就会动态更新
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI)
	FBlackboardKeySelector EnemyActorKeySelector;
};
