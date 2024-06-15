// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/TPSWeaponLogicComponent.h"
#include "TPSAIWeaponLogicComponent.generated.h"

/**
 * 管理AI的武器逻辑类
 */
UCLASS()
class FUTURETPS_API UTPSAIWeaponLogicComponent : public UTPSWeaponLogicComponent
{
	GENERATED_BODY()

public:
	virtual void Fire() override;
	virtual void SwitchWeapon() override;
};
