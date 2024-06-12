// FutureTPS Game All Rights Reserved


#include "Dev/TPSDevActor.h"

#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(MyATPSDevActorLog, All, All)


ATPSDevActor::ATPSDevActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	FVector ActorLocation = GetActorLocation();
	ActorLocation.Z += Radius / 2.0;
	SceneComponent->SetWorldLocation(ActorLocation);
}

// Called when the game starts or when spawned
void ATPSDevActor::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ATPSDevActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// UE_LOG(MyATPSDevActorLog, Error, TEXT("Hurted"));
	// segments是球体的片段数，越大就越像圆
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, SphereSegment, SphereColor);

	// bool bApplyRadialDamage =
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius,
	                                    DamageType, {},
	                                    this, nullptr, BIsDoFullDamage);
	// BIsDoFullDamage为true时,角色不管在圆球的哪一个点上都受到完全的伤害,为false时,根据角色离圆心点的距离进行插值越近圆心伤害越接近baseDamage
	// UE_LOG(MyATPSDevActorLog, Error, TEXT("%d"), bApplyRadialDamage);
}
