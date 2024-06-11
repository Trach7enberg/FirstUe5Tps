// FutureTPS Game All Rights Reserved


#include "Effects/TPSWeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

DEFINE_LOG_CATEGORY_STATIC(MyUWeaponFXComponentLog, All, All);

UWeaponFXComponent::UWeaponFXComponent() { PrimaryComponentTick.bCanEverTick = true; }


// Called when the game starts
void UWeaponFXComponent::BeginPlay() { Super::BeginPlay(); }


// Called every frame
void UWeaponFXComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponFXComponent::PlayImpactFX(const FHitResult &Hit) const
{
	auto Effect = DefaultEffect;
	// 如果Hit中存在物理材质 (注意 默认情况下物理材质不会传到Hit中,这出于优化目的)
	// (所以我们得在发出子弹射击或类似的动作时候就得提前设置(通过FCollisionQueryParams.bReturnPhysicalMaterial)
	// (这样发生冲击的时候我们才能成功拿到物理材质)
	if (Hit.PhysMaterial.IsValid())
	{

		// 获得被击中物体的的物理材质的弱指针引用 (意味着这个物理材质原本的指针该销毁时候就会被销毁,所以我们使用前要用IsValid()判断是否存在)
		const auto PhysMat = Hit.PhysMaterial.Get();
		UE_LOG(MyUWeaponFXComponentLog, Error, TEXT("IsValid"));
		if (EffectsMap.Contains(PhysMat))
		{
			// 通过键获取值
			Effect = EffectsMap[PhysMat];
			UE_LOG(MyUWeaponFXComponentLog, Error, TEXT("in"));
		}
	}
	// 将特效系统生成到Hit的冲击点位置
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Effect, Hit.ImpactPoint,
	                                               Hit.ImpactPoint.Rotation(), FVector(1), true);
}
