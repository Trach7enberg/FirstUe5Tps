// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_CanPickup.generated.h"

/**
 *  判断场景中的拾取物能不能被拾取
 */
UCLASS()
class FUTURETPS_API UEnvQueryTest_CanPickup : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_CanPickup(const FObjectInitializer& ObjectInitializer);
	
	virtual void RunTest(FEnvQueryInstance &QueryInstance) const override;
};
