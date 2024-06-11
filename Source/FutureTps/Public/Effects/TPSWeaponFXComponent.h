// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSWeaponFXComponent.generated.h"

class UNiagaraSystem;
/**
 * 武器特效(FX)组件
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FUTURETPS_API UWeaponFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponFXComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction *ThisTickFunction) override;

	// 生成特效
	void PlayImpactFX(const FHitResult& Hit) const;

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem *NiagaraSystemEffect;


};