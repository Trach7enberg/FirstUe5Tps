// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreTypes/CoreType.h"
#include "TPSWeaponFXComponent.generated.h"

class UNiagaraSystem;
class UPhysicalMaterial;

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

	/// 生成冲击点的特效和贴花效果
	/// @param Hit 击中数据
	void PlayImpactFX(const FHitResult &Hit) const;

protected:
	virtual void BeginPlay() override;


	// 当击中无法确定的物理材质时,使用这个特效
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	FImpactData DefaultImpactData;

	// 利用Map映射:物理材质和对应的击中特效
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	TMap<UPhysicalMaterial *, FImpactData> ImpactDataMap;


};
