// FutureTPS Game All Rights Reserved


#include "Projectiles/TPSProjectile.h"


#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Effects/TPSWeaponFXComponent.h"

DEFINE_LOG_CATEGORY_STATIC(MyATPSProjectileLog, All, All);

ATPSProjectile::ATPSProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionComponent->InitSphereRadius(SphereRadius);
	// 不进行实际物理碰撞,但会返回碰撞的结果
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	// 能被所有东西挡住
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);

	// 允许返回的HitResult拿到击中物体的物理材质
	CollisionComponent->bReturnMaterialOnMove = true;

	SetRootComponent(CollisionComponent);

	WeaponFXComponent = CreateDefaultSubobject<UWeaponFXComponent>("WeaponFXComponent");

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	// ParticleTraceComponent = CreateDefaultSubobject<UNiagaraComponent>("ParticleTrace");
	// ParticleTraceComponent->SetupAttachment(GetRootComponent());

	ProjectileMovementComponent->InitialSpeed = 2500.0f;
	ProjectileMovementComponent->MaxSpeed = 2500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

	// 射弹的重力作用	
	ProjectileMovementComponent->ProjectileGravityScale = 0.5f;
}

void ATPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	check(ProjectileMovementComponent);
	check(CollisionComponent);
	check(WeaponFXComponent);
	ProjectileMovementComponent->Velocity = ShotDirection * ProjectileMovementComponent->InitialSpeed;

	// 绑定碰撞盒碰撞的回调函数
	CollisionComponent->OnComponentHit.AddDynamic(this, &ATPSProjectile::OnProjectileHit);

	// 射弹移动时(如果碰到owner)忽略Owner,或者不能追上射弹
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	SetLifeSpan(LifeSec);
}

void ATPSProjectile::OnProjectileHit(UPrimitiveComponent *HitComponent, AActor *OtherActor,
                                     UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	if (!GetWorld()) { return; }
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Hit")));

	// segments是球体的片段数，越大就越像圆
	DrawDebugSphere(GetWorld(), Hit.ImpactPoint, DamageRadius, 20, FColor::Purple, false, 3.f);


	// BIsDoFullDamage为true时,角色不管在圆球的哪一个点上都受到完全的伤害,为false时,根据角色离圆心点的距离进行插值越近圆心伤害越接近baseDamage
	UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageValue, GetActorLocation(), DamageRadius, nullptr,
	                                    {GetOwner(),},
	                                    this, GetPlayerController(), BIsDoFullDamage);
	
	ProjectileMovementComponent->StopMovementImmediately();

	// 播放特效
	WeaponFXComponent->PlayImpactFX(Hit);

	Destroy();
}


void ATPSProjectile::SetShotDirection(const FVector &Direction) { ShotDirection = Direction; }

AController *ATPSProjectile::GetPlayerController() const
{

	ACharacter *Player = Cast<ACharacter>(GetOwner());
	if (!Player) { return nullptr; }

	return Player->GetController<APlayerController>();
}
