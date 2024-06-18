// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "TPSIsHurtBTDecorator.generated.h"

/**
 *  根据设置的血量百分比,判断当前AI是否受伤了然后需要拾取补给品
 */
UCLASS()
class FUTURETPS_API UTPSIsHurtBTDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	UTPSIsHurtBTDecorator();

protected:
	// 血量百分比
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI, meta=(ClampMin=0.1, ClampMax=0.9))
	float HealthPercent = 0.6f;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const override;
};
