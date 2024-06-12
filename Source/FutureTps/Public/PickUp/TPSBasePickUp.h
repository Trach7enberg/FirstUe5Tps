// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSBasePickUp.generated.h"

class USphereComponent;
/**
 * 拾取物(血包和弹药包)的基类
 */
UCLASS()
class FUTURETPS_API ATPSBasePickUp : public AActor
{
	GENERATED_BODY()

public:
	ATPSBasePickUp();

	/// 当其它actor与本actor重合时会触发事件,然后事件调用该函数
	/// @param OtherActor 
	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category=Pickup)
	USphereComponent *CollisionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Pickup)
	UStaticMeshComponent *PickupStaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Pickup)
	float SphereRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Pickup)
	float RespawnDelay = 5.0f;

protected:
	virtual void BeginPlay() override;

	virtual void Respawn();

	virtual void RespawnHandle();

	// 当前是否能捡起
	virtual bool CanPickup(AActor *Player);

	// 捡起
	virtual void Pickup(AActor *Actor);

private:
	float RotationYaw = 0.0f;

	FTimerHandle RespawnTimer;

	// 设置拾取物自动旋转的方向和单次旋转角度
	void InitPickUpRotation();

};
