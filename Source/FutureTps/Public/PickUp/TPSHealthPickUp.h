// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PickUp/TPSBasePickUp.h"
#include "TPSHealthPickUp.generated.h"

/**
 * 
 */
UCLASS()
class FUTURETPS_API ATPSHealthPickUp : public ATPSBasePickUp
{
	GENERATED_BODY()

protected:
	virtual bool CanPickup(AActor *Actor) override;
	virtual void Pickup(AActor *Actor) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PickUp, meta=(ClampMin=0.1, ClampMax=1.0))
	float ResupplyPercent = 0.3f;


};
