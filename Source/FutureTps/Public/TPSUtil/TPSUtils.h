#pragma once

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
		if (!PlayerPawn) { return nullptr; }

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
			UE_LOG(LogTemp, Error, TEXT("Cant be null!"))
			return nullptr;
		}
		return PlayerActor->GetComponentByClass<ComponentType>();
	}
};
