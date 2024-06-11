// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CoreTypes/CoreType.h"
#include "TPSBaseWeapon.generated.h"


/**
 * 这是武器的mesh类
 */
UCLASS()
class FUTURETPS_API ATPSBaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ATPSBaseWeapon();
	virtual void Fire();
	virtual void StopFire();
	void Reload();
	bool CanReload() const;

	/// 按总子弹的百分比数量补充弹匣
	/// @param ResupplyPercent 百分比数
	virtual void Resupply(float ResupplyPercent);

	/// 判断是否是满弹药
	/// @return 满弹药返回true
	bool IsFullAmmo() const;

	// TODO 修复自动射击模式下半自动射击不灵敏问题 (实际是射速间隔(FireRate)导致的问题)
	FORCEINLINE bool IsUnderFire() const { return BIsUnderFire; }

	// 获取武器UI数据
	FORCEINLINE FWeaponUIData GetWeaponUIData() const { return WeaponUIData; }

	FAmmoData GetCurrentAmmo() const { return CurrentAmmo; }

protected:
	// 武器模型, 需要在蓝图里手动初始化
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent *WeaponSkeletalMeshComponent;

	// 枪骨骼里的枪口socket名字,用于武器骨骼的枪口位置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	FName MuzzlePosition{"MuzzleSocket"};

	// 射线长度
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	float TraceLength = 1500.f;

	// 开火状态
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	bool BIsUnderFire = false;

	// 默认武器弹药数
	UPROPERTY(EditDefaultsOnly, Category=Weapon)
	FAmmoData DefaultAmmo{30, 4, false};

	// 需要在蓝图里手动初始化
	UPROPERTY(EditDefaultsOnly, Category=UI)
	FWeaponUIData WeaponUIData{};

protected:
	/// 生成射线检测碰撞,如果发生Hit了则将HitResult数据填补
	/// @param HitResult 击中物体的信息
	/// @param TraceStart 射线开始位置
	/// @param TraceEnd 射线结束位置
	void MakeTraceHit(FHitResult &HitResult, const FVector &TraceStart, const FVector &TraceEnd) const;


	/// 从当前类的owner里获取controller
	/// @return 当前actor的拥有者(Owner)的character的controller类
	APlayerController *GetPlayerController() const;

	/// 获取玩家摄像机的中心视点坐标和旋转方向
	/// @param ViewLocation 视点的坐标向量
	/// @param ViewRotation 视点的旋转方向度数
	/// @return 获取摄像机中心视点成功则为true,否则为false
	bool GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const;

	/// 获取枪口的transform
	/// @return 枪口的transform
	FTransform GetMuzzleWorldTransform() const;

	/// 获取发射射线所需要的起点和始点位置方向
	/// 如果给入锥体角度大于0,则在半径为HalfConeAngle的锥体内取随机射线方向,(形成散射效果)
	/// @param TraceStart 起点位置
	/// @param TraceEnd 终点位置
	/// @param HalfConeAng 锥体的一半角度
	/// @return 获取位置成功为true,否则为false
	virtual bool GetTraceData(FVector &TraceStart, FVector &TraceEnd, float HalfConeAng = 0) const;

	/// 根据击中信息,计算武器枪口的向前向量 与 武器枪口点到(光线)击中点这条向量的夹角,用来修复明明敌人在角色背后却还可以攻击
	/// @param MuzzleTransform 武器枪口的transform
	/// @param Hit 击中信息
	/// @return 武器枪口的向前向量 与 武器枪口点到(光线)击中点间的向量,的夹角
	static float GetAngleBetweenMuzzleAndHitPoint(const FTransform &MuzzleTransform, const FHitResult &Hit);


	virtual void AutoFire();
	virtual void SemiFire();
	virtual void MakeShot();

	virtual void BeginPlay() override;

	// 减少子弹
	void DecreaseBullet();


	// 子弹是否打空
	bool IsBulletEmpty() const;
	// 弹匣是否空了
	bool IsMagzEmpty() const;

	// 更换弹匣
	void ChangeMagz();

	void DebugLogAmmo() const;

private:
	// 武器当前的弹药类
	FAmmoData CurrentAmmo;


};
