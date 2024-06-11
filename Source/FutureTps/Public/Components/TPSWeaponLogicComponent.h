// FutureTPS Game All Rights Reserved


#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreTypes/CoreType.h"
#include "TPSWeaponLogicComponent.generated.h"

class ATPSBaseWeapon;

/**
 * 该类处理和武器逻辑相关的动作逻辑,比如开火射击等
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FUTURETPS_API UTPSWeaponLogicComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTPSWeaponLogicComponent();
	void Fire();
	// 停止开火,注意有定时器的子类 死亡时还在开火的话,需要清除定时器
	void StopFire();
	void SwitchWeapon();

	void Reload();

	/// 按百分比补充弹匣数
	/// @param ResupplyPercent 百分比
	void Resupply(float ResupplyPercent) const;

	// 获取武器UI数据
	bool GetWeaponUIData(FWeaponUIData &WeaponUIData) const;

	// 获取武器弹药结构信息
	bool GetWeaponAmmo(FAmmoData &AmmoData) const;

	/// 判断是否在换弹匣
	/// @return 在换弹中返回true,否则返回false
	bool IsReloading() const;

	bool IsFullAmmo() const;

protected:
	// 武器结构的实体数组,在蓝图里指定
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	TArray<FWeaponData> WeaponData = {};

	// 拿在手中的socket点位置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	FName WeaponEquipSocketName{"WeaponPoint"};

	// 武器放在背部的socket点位置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=Weapon)
	FName WeaponArmorySocketName{"ArmorySocket"};

	// 切换武器动画
	UPROPERTY(EditDefaultsOnly, Category=Animation)
	UAnimMontage *EquipAnimMontage;


	virtual void BeginPlay() override;

	// 结束当前类的play时,销毁通过实体数组生成的(指针武器数组里的)所有武器
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	// 存储通过WeaponData已经Spawn了的武器的数组
	UPROPERTY()
	TArray<ATPSBaseWeapon *> Weapons = {};

	// 指向当前武器的指针
	UPROPERTY()
	ATPSBaseWeapon *CurrentWeapon = nullptr;

	// 当前的武器换弹动画
	UPROPERTY()
	UAnimMontage *CurrentReloadAnimMontage = nullptr;


	// 当前指针武器数组里的索引
	int32 CurrentWeaponIndex = 0;

	// 切换武器动画开始时的flag,初始为false
	bool BIsBeginWeaponSwitching = false;

	// 切换武器动画完成时的flag,初始为true
	bool BIsCompleteWeaponSwitching = true;

	// 开始换子弹时候的动画Notify flag,初始为false
	bool BisBeginWeaponReloading = false;

	// 完成换子弹时候的动画Notify flag,初始为true
	bool BisCompleteWeaponReloading = true;

	/// 是否能开火
	/// @return 能开火返回true
	bool CanFire();

	/// 是否能换弹
	/// @return 能换弹返回true
	bool CanReload() const;


	bool IsUnderFire() const;

	// 重置换弹过程(通知)的标记,因为当换弹时又切枪会导致不能再次换弹
	void ResetReloadState();

	/// 是否能切换武器
	/// @return 能切换武器返回true
	bool CanSwitchWeapon() const;

	// 装备武器
	void EquipWeapon(int32 WeaponIndex);

	/// 获取当前组件类的Owner 
	/// @param Owner 当前组件的Owner
	/// @return ACharacter (当前组件的拥有者它实际上是我们的Character)
	ACharacter *GetCharacter(AActor *Owner);

	/// 生成武器到指针数组
	void InitWeaponData();

	/// 将武器绑定到Socket上(初始化是全部绑定到背上了)
	void AttachWeaponToSocket(ATPSBaseWeapon *Weapon, USceneComponent *CharacterMesh, const FName &SocketName);


	/// 播放蒙太奇动画,注意当前类是组件,所以通过它的Owner调用PlayAnim
	/// @param Am 蒙太奇动画实例
	void PlayAnimMontage(UAnimMontage *Am);


	/// 切换武器的蒙太奇动画完成时发出通知时就调用这个回调函数
	void OnEquipFinished(USkeletalMeshComponent *SkeletalMeshComponent);

	/// 切换武器的蒙太奇动画开始发出通知时就调用这个回调函数
	void OnEquipBegin(USkeletalMeshComponent *SkeletalMeshComponent);

	void OnReloadBegin(USkeletalMeshComponent *SkeletalMeshComponent);
	void OnReloadFinished(USkeletalMeshComponent *SkeletalMeshComponent);


	// 当换弹时又切枪时就会发出广播的委托
	FOnSwitchWhenChangeMagz OnSwitchWhenChangeMagz;


};
