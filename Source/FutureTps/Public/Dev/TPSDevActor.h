// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSDevActor.generated.h"

UCLASS()
class FUTURETPS_API ATPSDevActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATPSDevActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent *SceneComponent;

	// 选择伤害的类型
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 600.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor SphereColor = FColor::Red;

	// 数字越大球的片段矩形越多越像圆
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SphereSegment = 24;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)

	bool BIsDoFullDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 10.0f;;
};
