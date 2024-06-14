// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TPSFireService.generated.h"

/**
 * 开火Service
 */
UCLASS()
class FUTURETPS_API UTPSFireService : public UBTService
{
	GENERATED_BODY()
public:
	UTPSFireService();
protected:
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;

	// 开火的目标
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=AI)
	FBlackboardKeySelector EnemyActorKey;
};
