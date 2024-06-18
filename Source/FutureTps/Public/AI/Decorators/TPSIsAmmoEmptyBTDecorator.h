// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "TPSIsAmmoEmptyBTDecorator.generated.h"

/**
 * 判断当前AI拿着的武器是否没子弹了
 * (总子弹数和设定的弹药百分比对比,小于设置的即需要补充弹药)
 */
UCLASS()
class FUTURETPS_API UTPSIsAmmoEmptyBTDecorator : public UBTDecorator
{
	GENERATED_BODY()
public:
	UTPSIsAmmoEmptyBTDecorator();
protected:
	/// 弹药百分比
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI, meta=(ClampMin=0, ClampMax=0.99999))
	float AmmoLeftPercent = 0.5;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) const override;
};
