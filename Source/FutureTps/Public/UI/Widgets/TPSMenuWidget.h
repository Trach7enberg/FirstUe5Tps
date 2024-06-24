// FutureTPS Game All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "TPSBaseWidget.h"
#include "CoreTypes/CoreType.h"
#include "TPSMenuWidget.generated.h"

class UTPSLevelSelectItemWidget;
class UTPSGameInstance;
class UHorizontalBox;
class UButton;
/**
 * 主菜单UI小部件
 */
UCLASS()
class FUTURETPS_API UTPSMenuWidget : public UTPSBaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	/// 开始游戏按钮
	UPROPERTY(meta=(BindWidget))
	UButton *StartGameButton;

	// 关卡选择的区域
	UPROPERTY(meta=(BindWidget))
	UHorizontalBox *LevelItemArea;

	// 预加载关卡时的动画,动画结束时关卡则加载完毕
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	UWidgetAnimation* PreLoadLevelAnimation;
	
	// 关卡选择区域下的关卡item UI的类
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=UI)
	TSubclassOf<UUserWidget> LevelSelectItemWidgetClass;

	/// @brief 当前类任何动画播放完毕时都会调用这个函数
	/// @param Animation 
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation *Animation) override;

private:
	// TArray<UTPSLevelSelectItemWidget *> LevelItemWidgets;

	TMap<FName, UTPSLevelSelectItemWidget *> LevelItemWidgets;

	// 上一次选择的关卡名字Tmap的索引,用于关闭上一次选择的关卡的红框
	FName LastSelected ;

	/// 开始按钮点击之后的回调函数
	UFUNCTION()
	void OnStartGame();

	/// 初始化选择关卡区域的关卡项目数量
	void InitLevelItems();

	// 选择光卡的按钮被按下时就会调用该函数
	UFUNCTION()
	void OnSelectedLevelItem(const FLevelData &Data);

	UTPSGameInstance *GetTPSGameInstance() const;

	/// @brief 判断要添加进关卡选择区域的关卡是否是重复的关卡
	/// @param Data 关卡数据
	/// @return 重复则true,否则false
	bool IsDuplicateLevel(const FLevelData &Data) const;
};
