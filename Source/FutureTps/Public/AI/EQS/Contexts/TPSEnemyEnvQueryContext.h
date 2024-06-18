// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "TPSEnemyEnvQueryContext.generated.h"

/**
 * 
 */
UCLASS()
class FUTURETPS_API UTPSEnemyEnvQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

public:
	virtual void ProvideContext(FEnvQueryInstance &QueryInstance, FEnvQueryContextData &ContextData) const override;

protected:

	// 黑板上某个键的名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=AI)
	FName EnemyActorKeyName = "EnemyActor";
};
