// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"

/**
 * 和动画有关的工具类
 */
class FUTURETPS_API MyAnimUtils
{
public:
	/// 在动画蓝图(Animation)中查找通知(Notify)并且返回该通知,以便用它来绑定回调函数
	/// @tparam T 想要的动画通知类
	/// @param Animation 各种动画(蒙太奇),比如切枪动画、装弹动画
	/// @return 具体的通知类,找不到则为nullptr 
	template <typename T>
	static T *FindAnimNotifyByClass(UAnimSequenceBase *Animation)
	{
		if (!Animation) { return nullptr; }

		// 获取我们动画蒙太奇的通知数组
		// notifies是一个数组,这个数组里的内容是通知事件
		// 所以我们需要拿到通知事件里的notify判断是不是我不是我们自己的EquipFinishedAnimNotify
		for (auto NotifyEvent : Animation->Notifies)
		{
			if (auto Notify = Cast<T>(NotifyEvent.Notify)) { return Notify; }
		}
		return nullptr;
	}

	/// 初始化动画蒙太奇,绑定当动画帧开始或者完成时候的(通知)回调函数
	/// @tparam TBeginNotify 动画帧开始时的通知类型
	/// @tparam TCompleteNotify 动画帧结束时的通知类型
	/// @tparam UserClass 调用成员函数的类
	/// @tparam BeginFunc 要绑定通知开始 UserClass类的成员回调函数
	/// @tparam CompleteFunc 要绑定通知结束 UserClass类的成员的回调函数
	/// @param Animation 要绑定回调函数的那个动画
	/// @param ThisClass 选择调用回调函数的类
	template <typename TBeginNotify, typename TCompleteNotify, typename UserClass, void(UserClass::*
		          BeginFunc)(USkeletalMeshComponent *SkeletalMeshComponent), void(UserClass::*
		          CompleteFunc)(
		          USkeletalMeshComponent *SkeletalMeshComponent)>
	static void InitAnimationNotifies(UAnimMontage *Animation, UserClass *ThisClass)
	{
		if (!Animation) { return; }
		// UE_LOG(LogTemp, Error, TEXT("InInitWeapon"));

		TBeginNotify *EBeginAnimNotify = FindAnimNotifyByClass<TBeginNotify>(Animation);
		TCompleteNotify *EFinishedAnimNotify = FindAnimNotifyByClass<TCompleteNotify>(
			Animation);

		if (EBeginAnimNotify && EFinishedAnimNotify)
		{
			EBeginAnimNotify->OnNotified.AddUObject(ThisClass, BeginFunc);
			EFinishedAnimNotify->OnNotified.AddUObject(ThisClass, CompleteFunc);
		}

	}
};
