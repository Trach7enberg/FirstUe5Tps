#pragma once
#include "PlayerStates/TPSBasePlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(MyFTPSUtilsLog, All, All);

class FTPSUtils
{
public:
	/// 根据当前类所属的Pawn和 组件类名 获取对应的组件
	/// @tparam ComponentType 组件类名
	/// @param PlayerPawn 玩家的Pawn
	/// @return ComponentType类型的组件
	template <typename ComponentType>
	static ComponentType *GetComponentByCurrentPlayer(APawn *PlayerPawn)
	{
		// 获取当前类的玩家Pawn
		if (!PlayerPawn)
		{
			UE_LOG(MyFTPSUtilsLog, Error, TEXT("Cant be null!"))
			return nullptr;
		}

		UActorComponent *ComponentByClass = PlayerPawn->GetComponentByClass(
			ComponentType::StaticClass());
		ComponentType *Component = Cast<ComponentType>(
			ComponentByClass);

		if (!Component) { return nullptr; }

		return Component;
	}

	template <typename ComponentType>
	static ComponentType *GetComponentByCurrentPlayer(AActor *PlayerActor)
	{
		if (!PlayerActor)
		{
			UE_LOG(MyFTPSUtilsLog, Error, TEXT("Cant be null!"))
			return nullptr;
		}
		return PlayerActor->GetComponentByClass<ComponentType>();
	}

	/// 判断两个控制器是否是敌人
	/// @param Controller1 控制器1
	/// @param Controller2 控制器2
	/// @return 是则返回true,否则返回false
	static bool AreEnemies(const AController *Controller1, const AController *Controller2)
	{
		if (!Controller1 || !Controller2 || Controller1 == Controller2) { return false; }

		const auto PlayerState1 = Controller1->GetPlayerState<ATPSBasePlayerState>();
		const auto PlayerState2 = Controller2->GetPlayerState<ATPSBasePlayerState>();

		return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
	}
};
