// FutureTPS Game All Rights Reserved


#include "PickUp/TPSBasePickUp.h"

#include "Components/SphereComponent.h"

DEFINE_LOG_CATEGORY_STATIC(MyATPSBasePickUpLog, Error, Error);

ATPSBasePickUp::ATPSBasePickUp()
{
	PrimaryActorTick.bCanEverTick = true;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	CollisionComponent->InitSphereRadius(SphereRadius);
	// 不进行实际物理碰撞,但会返回碰撞的结果
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	// 能被所有东西穿过
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SetRootComponent(CollisionComponent);

	PickupStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupStaticMesh");
	PickupStaticMesh->SetupAttachment(GetRootComponent());

}

void ATPSBasePickUp::BeginPlay()
{
	Super::BeginPlay();
	InitPickUpRotation();
}


void ATPSBasePickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0, RotationYaw, 0));
}

void ATPSBasePickUp::Respawn()
{
	InitPickUpRotation();
	if (GetRootComponent())
	{
		this->SetActorEnableCollision(true);
		GetRootComponent()->SetVisibility(true, true);
	}
}

void ATPSBasePickUp::RespawnHandle()
{

	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ATPSBasePickUp::Respawn, RespawnDelay,
	                                       false);
}

bool ATPSBasePickUp::CanPickup(AActor *Player) { return false; }

void ATPSBasePickUp::Pickup(AActor *Actor)
{
	if (GetRootComponent())
	{
		this->SetActorEnableCollision(false);
		GetRootComponent()->SetVisibility(false, true);
		RespawnHandle();
	}
}


void ATPSBasePickUp::NotifyActorBeginOverlap(AActor *OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (CanPickup(OtherActor)) { Pickup(OtherActor); }


}

void ATPSBasePickUp::InitPickUpRotation()
{
	float Direction = FMath::RandBool() ? -1.0f : 1.0f;
	RotationYaw = Direction * FMath::RandRange(1.0f, 2.0f);

}
