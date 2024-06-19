// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSRespawnComponent.generated.h"

/**
 * 控制重生的组件,挂载在玩家的控制器上
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FUTURETPS_API UTPSRespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTPSRespawnComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction *ThisTickFunction) override;

	void Respawn(float Time);
	float GetRespawnTimeCountDown() const { return RespawnTimeCountDown; }

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle RespawnTimerHandle;

	// 重生倒计时
	float RespawnTimeCountDown = 0;

	// 重生倒计时的更新,结束才会重生
	void RespawnTimerUpdate();


};
