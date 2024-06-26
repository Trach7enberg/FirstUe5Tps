// FutureTPS Game All Rights Reserved


#include "Characters/TPSBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TPSHealthComponent.h"
#include "Components/TPSWeaponLogicComponent.h"
#include "Components/TPSCharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY_STATIC(MyATPSBaseCharacterLog, All, All)

void ATPSBaseCharacter::InitMesh()
{
	FString AnimName =
		"/Script/Engine.AnimBlueprint'/Game/Blueprints/Animations/ABP_BaseCharacter.ABP_BaseCharacter_C'";
	FString MeshName = "/Script/Engine.SkeletalMesh'/Game/ExternalContent/Characters/HeroTPP/HeroTPP.HeroTPP'";
	FString AnimDeathName = "/Script/Engine.AnimMontage'/Game/Blueprints/Animations/AM_Death.AM_Death'";


	// UClass *AnimationBp = LoadObject<UClass>(this, *AnimName);


	// USkeletalMesh *SkeletalBp = LoadObject<USkeletalMesh>(nullptr, *MeshName);
	// UAnimMontage *AnimMontageDeathBp = LoadObject<UAnimMontage>(nullptr, *AnimDeathName);
	ConstructorHelpers::FObjectFinder<UClass> AnimationBp(*AnimName);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalBp(*MeshName);
	ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMontageDeathBp(*AnimDeathName);


	if (AnimationBp.Succeeded() && SkeletalBp.Succeeded() && AnimMontageDeathBp.Succeeded())
	{
		// UE_LOG(MyATPSBaseCharacterLog, Error, TEXT("In"));
		GetMesh()->SetSkeletalMesh(SkeletalBp.Object);
		GetMesh()->SetRelativeLocation(FVector(0, -8, -85));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
		GetMesh()->SetAnimationMode(EAnimationMode::Type::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(AnimationBp.Object);

		this->DeathAnimMontage = AnimMontageDeathBp.Object;
	}
}

// Sets default values
ATPSBaseCharacter::ATPSBaseCharacter(const FObjectInitializer &ObjectInitializer):
	Super(ObjectInitializer.SetDefaultSubobjectClass<UTPSCharacterMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;


	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 80));

	HealthComponent = CreateDefaultSubobject<UTPSHealthComponent>("HealthComponent");
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());

	WeaponLogicComponent = CreateDefaultSubobject<UTPSWeaponLogicComponent>("WeaponLogicComponent");

	CameraCollisionComponent = CreateDefaultSubobject<USphereComponent>("CameraCollisionComponent");
	CameraCollisionComponent->SetupAttachment(CameraComponent);
	CameraCollisionComponent->SetSphereRadius(10.0f);
	CameraCollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);


	BIsJumpActionDown = false;
	BIsRush = false;
	BCanRotatingCamera = true;
	MoveForwardValue = 0;
	MoveRightValue = 0;


	// 生命值文本 控制人不可见
	HealthTextComponent->SetOwnerNoSee(true);
	HealthTextComponent->SetRelativeLocation(FVector(0, 24, 85));
	// 文本初始值设置
	HealthTextComponent->SetText(FText::FromString(FString("100")));

	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->bUsePawnControlRotation = true;
	// 偏移弹簧臂socket
	SpringArmComponent->SocketOffset = FVector(0, 100.f, 0);

	// 绑定当死亡死的回调函数
	HealthComponent->OnDeath.AddUObject(this, &ATPSBaseCharacter::OnDeath);

	// 绑定当生命值更新时 触发的回调函数
	HealthComponent->OnHealthChanged.AddUObject(this, &ATPSBaseCharacter::OnHealthChanged);

	//直接在这里初始化角色的碰撞矩阵设置,不用每次都到蓝图里调整
	GetMesh()->SetCollisionProfileName("MyPawn");

	// InitMesh();
	GetMesh()->SetRelativeLocation(FVector(0, -8, -85));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

}


void ATPSBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());
	check(CameraCollisionComponent);

	InitCharacterName();
	// 游戏开始手动显示血量
	OnHealthChanged(HealthComponent->GetHealth(), false);

	// 绑定落地时的委托回调函数
	LandedDelegate.AddDynamic(this, &ATPSBaseCharacter::OnGroundLanded);

	CameraCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATPSBaseCharacter::CameraBeginOverlap);
	CameraCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATPSBaseCharacter::CameraEndOverlap);
}

// Called every frame
void ATPSBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// UE_LOG(MyATPSBaseCharacterLog,Error,TEXT("%f"),HealthComponent->GetHealth());
}

// Called to bind functionality to input
void ATPSBaseCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	check(WeaponLogicComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ATPSBaseCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ATPSBaseCharacter::Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATPSBaseCharacter::MyJump);

	PlayerInputComponent->BindAction("Rush", IE_Pressed, this, &ATPSBaseCharacter::RushBegin);
	PlayerInputComponent->BindAction("Rush", IE_Released, this, &ATPSBaseCharacter::RushEnd);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponLogicComponent, &UTPSWeaponLogicComponent::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponLogicComponent, &UTPSWeaponLogicComponent::StopFire);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponLogicComponent, &UTPSWeaponLogicComponent::Reload);

	// TODO 前滚轮切换上一把武器、后滚轮切换下一把武器
	PlayerInputComponent->BindAction("SwitchWeapons", IE_Pressed, WeaponLogicComponent,
	                                 &UTPSWeaponLogicComponent::SwitchWeapon);


}

float ATPSBaseCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) { return 0.0f; }

	FVector ActorForwardVector = GetActorForwardVector();
	FVector CurrentVelocity = GetVelocity().GetSafeNormal();

	// 当前人物速度的方向和人物的forward朝向的点乘得出标量
	float DotProduct = FVector::DotProduct(ActorForwardVector, CurrentVelocity);
	FVector CrossProduct = FVector::CrossProduct(ActorForwardVector, CurrentVelocity);

	// 将点乘得出的标量用反余弦函数Arc cos(标量)得出的弧度再转换成角度,这就是它们之间的夹角了
	float AngleBetween = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

	// 转换叉乘所得出来的垂直方向(即Z) ,注意Sign会返回0 
	float sign = FMath::Sign(CrossProduct.Z);

	return (CrossProduct.IsZero()) ? AngleBetween : AngleBetween * sign;
	// return AngleBetween * sign;
}


void ATPSBaseCharacter::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector(), value);
	MoveForwardValue = value;
	//当人物按下冲刺后可以进行后退,这是不允许的 修改为冲刺时按下后退自动关闭冲刺
	if (value < 0) { BIsRush = false; }
}

void ATPSBaseCharacter::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector(), value);
	MoveRightValue = value;
}

void ATPSBaseCharacter::LookUp(float value)
{
	if (!BCanRotatingCamera) { this->bUseControllerRotationPitch = false; }
	AddControllerPitchInput(value);
}

void ATPSBaseCharacter::Turn(float value)
{
	if (!BCanRotatingCamera) { this->bUseControllerRotationYaw = false; }
	AddControllerYawInput(value);
}


void ATPSBaseCharacter::RushBegin()
{
	BIsRush = (MoveForwardValue > 0 && !GetVelocity().IsZero() && !HealthComponent->IsDead());
}

void ATPSBaseCharacter::RushEnd() { BIsRush = false; }

void ATPSBaseCharacter::MyJump()
{
	Jump();
	BIsJumpActionDown = true;
	// UE_LOG(MyATPSBaseCharacterLog, Warning, TEXT("Jump"));
}

void ATPSBaseCharacter::OnDeath()
{
	// TODO 如果角色在半空中死亡会浮在空中,待修复
	// PlayAnimMontage(DeathAnimMontage);

	// 死亡之后不允许冲刺,比如角色生前是按着冲刺死亡,这时候就要关掉
	BIsRush = false;
	BCanRotatingCamera = false;

	// 播放死亡声音
	if (GetWorld()) { UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation()); }

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	// GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	GetMesh()->SetSimulatePhysics(true);

	// 视角转移到观察者视角
	if (Controller) { Controller->ChangeState(NAME_Spectating); }
}


void ATPSBaseCharacter::OnHealthChanged(float Health, bool BIsDecreaseHealth)
{

	// 给字体渲染组件更新内容文字
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%s , %0.f"), *CharacterName, Health)));
}

void ATPSBaseCharacter::OnGroundLanded(const FHitResult &Hit)
{
	float AbsFallVelocity = FMath::Abs(GetVelocity().Z);

	if (AbsFallVelocity < LandedDamageVelocity.X)
		return;

	// 将第三个参数的值与第一个参数的范围进行比例映射,然后再以第二个参数的范围作为输出的范围
	float
		FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, AbsFallVelocity);

	this->TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

void ATPSBaseCharacter::SetCharacterColor(const FLinearColor &Color)
{
	// 在运行时动态创建并设置一个材质实例，这个函数通常用于在游戏运行时改变物体的材质属性，例如颜色、纹理等
	const auto MaterialInst = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);

	if (!MaterialInst) { return; }
	// 设置人物材质球里一个名字叫PaintColor的颜色
	MaterialInst->SetVectorParameterValue(PaintColor, Color);
}

void ATPSBaseCharacter::InitCharacterName()
{
	if (!CharacterName.IsEmpty()) { return; }
	FString Left;
	FString Right;
	FString Tag = "AI_";

	if (GetName().Split("_", &Left, &Right))
	{
		if (Right.Split("_", &Left, &Right))
		{
			if (Left.Contains("Base")) { Tag = "Base_"; }
			CharacterName = Tag + Right;
		}
	}

	// UE_LOG(MyATPSBaseCharacterLog, Error, TEXT("Left:%s,Right:%s"), *Left, *Right);
}

FString ATPSBaseCharacter::GetCharacterName()
{
	InitCharacterName();
	return CharacterName;
}

void ATPSBaseCharacter::CheckCameraOverlap()
{
	const auto NeedHide = CameraCollisionComponent->IsOverlappingComponent(GetCapsuleComponent());
	GetMesh()->SetOwnerNoSee(NeedHide);

	TArray<USceneComponent *> Childs;
	GetMesh()->GetChildrenComponents(true, Childs);
	for (auto Child : Childs)
	{
		const auto c = Cast<UPrimitiveComponent>(Child);
		if (c) { c->SetOwnerNoSee(NeedHide); }
	}
}

void ATPSBaseCharacter::CameraBeginOverlap(
	UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult &SweepResult) { CheckCameraOverlap(); }

void ATPSBaseCharacter::CameraEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                         UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) { CheckCameraOverlap(); }
