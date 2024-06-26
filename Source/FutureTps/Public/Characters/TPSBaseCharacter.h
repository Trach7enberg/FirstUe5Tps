// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "TPSBaseCharacter.generated.h"

class USphereComponent;
class UTPSWeaponLogicComponent;
class UTPSHealthComponent;
class USoundCue;

/**
 * 玩家角色类
 */
UCLASS()
class FUTURETPS_API ATPSBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATPSBaseCharacter(const FObjectInitializer &ObjectInitializer);

public:
	virtual void Tick(float DeltaTime) override;

	/// 设置角色(外观)的颜色
	virtual void SetCharacterColor(const FLinearColor &Color);

	virtual void SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent) override;

	// 是否在冲刺
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Movement)
	bool BIsRush;

	// 角色死亡后相机左右移动角色也跟着动,这是不允许的,该flag的作用就是禁止死亡后尸体左右旋转
	// TODO 进入观察者视角后这个BUG 也无所谓了,考虑是否移除这个flag不用修复
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Movement)
	bool BCanRotatingCamera;

	// 一开始游戏人物会莫名触发跳跃姿势bug,该flag用来修复该Bug
	// 第一次手动开启为true后一直都是true,不然角色掉落无法触发下落动画
	// TODO 待修复这个BUG
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Movement)
	bool BIsJumpActionDown;

	// 当前前进和后退的归一化方向,1为前进,-1为后退
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Movement)
	float MoveForwardValue;

	// 当前往左和往右的归一化方向,1为右,-1为左
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Movement)
	float MoveRightValue;

	// 计算当前角度(动画机通过角度调整对应的动画),返回direction供混合动画输入使用
	UFUNCTION(BlueprintCallable, Category=Movement)
	float GetMovementDirection() const;

	/// 获取当前角色的名字(GetName的简化版)
	/// @return 角色的名字
	FString GetCharacterName();

protected:
	virtual void BeginPlay() override;

	// 初始化人物角色的mesh模型、动画等 (TODO 注意 发行打包时,不要调用此函数 会报错)
	void InitMesh();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)
	UCameraComponent *CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)
	USpringArmComponent *SpringArmComponent;

	// actor的生命组件,纯逻辑组件,没有实体,不需要附加到根组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)
	UTPSHealthComponent *HealthComponent;

	// 渲染文字组件
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)
	UTextRenderComponent *HealthTextComponent;

	// 处理武器逻辑组件,比如挂载武器、开枪
	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	UTPSWeaponLogicComponent *WeaponLogicComponent;

	// 摄像机的碰撞盒
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Components)
	USphereComponent *CameraCollisionComponent;

	// 播放死亡蒙太奇
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Animation)
	UAnimMontage *DeathAnimMontage;

	// 死亡音效
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Sound)
	USoundCue * DeathSound;

	// 落地速度Velocity(Z轴)
	UPROPERTY(EditDefaultsOnly, Category=Damage)
	FVector2D LandedDamageVelocity = FVector2D(900.f, 1200.f);

	// 根据落地Z.velocity 计算落地伤害
	UPROPERTY(EditDefaultsOnly, Category=Damage)
	FVector2D LandedDamage = FVector2D(10.f, 100.f);

	UPROPERTY(EditDefaultsOnly, Category=Damage)
	float LifeSpanOnDeath = 5.f;

	/// 当前人物的材质颜色
	UPROPERTY(EditDefaultsOnly, Category=Material)
	FName PaintColor = "Paint Color";

	/// 角色的名字(GetName简化版)
	UPROPERTY(EditDefaultsOnly, Category=Player)
	FString CharacterName;

	// 生命组件委托的回调函数
	UFUNCTION()
	virtual void OnDeath();

	/// 生命组件当生命值变化时候委托的回调函数,主要用于更新角色头顶的文本
	/// @param Health 变化的生命值
	/// @param BIsDecreaseHealth 当前生命值变化是降低还是增加 
	virtual void OnHealthChanged(float Health, bool BIsDecreaseHealth);

private:
	void MoveForward(float value);
	void MoveRight(float value);
	void LookUp(float value);
	void Turn(float value);
	void RushBegin();
	void RushEnd();
	void MyJump();

	// 当角色落地时候触发该回调函数,记得在LandedDelegate绑定
	UFUNCTION()
	void OnGroundLanded(const FHitResult &Hit);

	/// 初始化当前角色的名字
	void InitCharacterName();

	/// 检测摄像机是否与角色重叠,重叠则对相应的物体进行隐藏处理
	void CheckCameraOverlap();

	/// 摄像机球体碰撞盒开始重叠时的回调函数
	UFUNCTION()
	void CameraBeginOverlap(UPrimitiveComponent *OverlappedComponent,
	                        AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                        const FHitResult &SweepResult);

	/// 摄像机球体碰撞盒结束重叠时的回调函数
	UFUNCTION()
	void CameraEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
	                      int32 OtherBodyIndex);
};
