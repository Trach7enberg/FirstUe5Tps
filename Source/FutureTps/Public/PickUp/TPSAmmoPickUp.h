// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PickUp/TPSBasePickUp.h"
#include "TPSAmmoPickUp.generated.h"


/**
 * 弹药拾取类
 */
UCLASS()
class FUTURETPS_API ATPSAmmoPickUp : public ATPSBasePickUp
{
	GENERATED_BODY()

protected:
	virtual bool CanPickup(AActor *Actor) override;
	virtual void Pickup(AActor *Actor) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PickUp, meta=(ClampMin=0.5, ClampMax=1.0))
	float ResupplyPercent = 0.1f;

	//TODO 判断角色有几把枪,如果当前是满子弹的则不补充,优先补充没有子弹的

};
