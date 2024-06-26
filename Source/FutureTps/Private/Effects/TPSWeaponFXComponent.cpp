// FutureTPS Game All Rights Reserved


#include "Effects/TPSWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(MyUWeaponFXComponentLog, All, All);

UWeaponFXComponent::UWeaponFXComponent() { PrimaryComponentTick.bCanEverTick = false; }


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
	// 一开始等于默认冲击结构
	auto ImpactData = DefaultImpactData;

	// 如果Hit中存在物理材质 (注意 默认情况下物理材质不会传到Hit中,这出于优化目的)
	// (所以我们得在发出子弹射击或类似的动作时候就得提前设置(通过FCollisionQueryParams.bReturnPhysicalMaterial)
	// (这样发生冲击的时候我们才能成功拿到物理材质)
	if (Hit.PhysMaterial.IsValid())
	{

		// 获得被击中物体的的物理材质的弱指针引用 (意味着这个物理材质原本的指针该销毁时候就会被销毁,所以我们使用前要用IsValid()判断是否存在)
		const auto PhysMat = Hit.PhysMaterial.Get();
		// UE_LOG(MyUWeaponFXComponentLog, Error, TEXT("IsValid"));

		if (ImpactDataMap.Contains(PhysMat))
		{
			// 通过键获取值FImpactData
			ImpactData = ImpactDataMap[PhysMat];
			// UE_LOG(MyUWeaponFXComponentLog, Error, TEXT("in"));
		}
	}
	// 将Niagara特效系统生成到Hit的冲击点位置
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NiagaraSystemEffect, Hit.ImpactPoint,
	                                               Hit.ImpactPoint.Rotation(), FVector(1), true);

	// 生成贴花
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), ImpactData.DecalData.MaterialInterface,
	                                                             ImpactData.DecalData.Size, Hit.ImpactPoint,
	                                                             Hit.ImpactNormal.Rotation());

	// 设置生成的贴花在经过LifeTime秒后持续消散FadeOutTime秒,最后销毁
	if (DecalComponent) { DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime); }

	// 播放冲击声音
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.ImpactSoundCue, Hit.ImpactPoint);
}
