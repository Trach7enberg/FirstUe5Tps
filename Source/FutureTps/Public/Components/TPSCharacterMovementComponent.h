// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TPSCharacterMovementComponent.generated.h"

/**
 * 当前角色的移动组件(覆盖默认的Character的移动组件)
 */
UCLASS()
class FUTURETPS_API UTPSCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	UTPSCharacterMovementComponent();
	virtual float GetMaxSpeed() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Movement, meta=(ClampMin=1.0, ClampMax=10))
	float RunFactor = 2;

};
