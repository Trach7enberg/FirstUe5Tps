// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TPSChangeWeaponRandomlyService.generated.h"

/**
 * 根据概率随机切换AI的武器
 */
UCLASS()
class FUTURETPS_API UTPSChangeWeaponRandomlyService : public UBTService
{
	GENERATED_BODY()

public:
	UTPSChangeWeaponRandomlyService();
	
	// 随机切换武器的可能性
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI, meta=(ClampMin=0.0f, ClampMax=1.0f))
	float Probability = 0.5;

protected:
	virtual void TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds) override;
};
