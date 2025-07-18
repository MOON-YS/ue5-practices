// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialShooting/SSPlayerCharacter.h"
#include "PrototypeLCharacter.h"
#include "InputAction.h"
#include "Components/SkeletalMeshComponent.h"
#include "SpecialShooting/SSWeaponBase.h"
#include "SpecialShooting/SSWeaponPistol.h"
#include "SpecialShooting/SSWeaponRifle.h"
#include "SpecialShooting/SSPlayerController.h"
#include "SpecialShooting/SSAimCircle.h"
#include "EnhancedInputComponent.h"

ASSPlayerCharacter::ASSPlayerCharacter()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Junmp(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Jump.IA_Jump'"));
	if (IA_Junmp.Succeeded())
	{
		JumpAction = IA_Junmp.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Move(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Move.IA_Move'"));
	if (IA_Move.Succeeded())
	{
		MoveAction = IA_Move.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Look(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_Look.IA_Look'"));
	if (IA_Look.Succeeded())
	{
		LookAction = IA_Look.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MouseLook(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Actions/IA_MouseLook.IA_MouseLook'"));
	if (IA_MouseLook.Succeeded())
	{
		MouseLookAction = IA_MouseLook.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Fire(TEXT("/Script/EnhancedInput.InputAction'/Game/Variant_Shooter/Input/Actions/IA_Shoot.IA_Shoot'"));
	if (IA_Fire.Succeeded())
	{
		FireAction = IA_Fire.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_Aiming(TEXT("/Script/EnhancedInput.InputAction'/Game/Variant_Shooter/Input/Actions/IA_Aiming.IA_Aiming'"));
	if (IA_Aiming.Succeeded())
	{
		AimingAction = IA_Aiming.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_SwapWeapon(TEXT("/Script/EnhancedInput.InputAction'/Game/Variant_Shooter/Input/Actions/IA_SwapWeapon.IA_SwapWeapon'"));
	if (IA_SwapWeapon.Succeeded())
	{
		SwapWeaponAction = IA_SwapWeapon.Object;
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	if (SkeletalMeshAsset.Succeeded())
	{
		SetFirstPersonMesh(SkeletalMeshAsset.Object);
		GetMesh()->SetSkeletalMesh(SkeletalMeshAsset.Object);
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed_C'"));
	if (AnimInstance.Succeeded())
	{
		GetFirstPersonMesh()->SetAnimInstanceClass(AnimInstance.Class);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -97.f),FQuat(FRotator(0.f, -90.f, 0.f)));
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}

void ASSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	//GetWorld()->SpawnActor()
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

	ASSWeaponBase* Pistol = GetWorld()->SpawnActor<ASSWeaponBase>(ASSWeaponPistol::StaticClass(), GetActorTransform(), SpawnParams);
	ASSWeaponBase* Rifle = GetWorld()->SpawnActor<ASSWeaponBase>(ASSWeaponRifle::StaticClass(), GetActorTransform(), SpawnParams);

	OwnedWeapon.Add(Pistol);
	OwnedWeapon.Add(Rifle);
	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon = Pistol;

	Pistol->AttachToActor(this, AttachmentRule);
	Pistol->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	Pistol->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, FirstPersonWeaponSocket);

	Rifle->AttachToActor(this, AttachmentRule);
	Rifle->GetFirstPersonMesh()->AttachToComponent(GetFirstPersonMesh(), AttachmentRule, FirstPersonWeaponSocket);
	Rifle->GetThirdPersonMesh()->AttachToComponent(GetMesh(), AttachmentRule, FirstPersonWeaponSocket);

	Rifle->SetActorHiddenInGame(true);

	GetFirstPersonMesh()->SetAnimInstanceClass(Pistol->GetFirstPersonAnimInstanceClass());
	GetMesh()->SetAnimInstanceClass(Pistol->GetThirdPersonAnimInstanceClass());


	//초기 무기의 에임 설정을 위한 타이머 설정
	if (ASSPlayerController* PC = Cast<ASSPlayerController>(GetWorld()->GetFirstPlayerController()))
	{

		AimCircle = PC->GetAimCircleWidget();
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&ASSPlayerCharacter::SetAimCircle,
			0.5f, 
			false 
		);

	}

}
void ASSPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimCircle->UpdateAimTick(DeltaTime);
}
void ASSPlayerCharacter::SetAimCircle()
{
	float Radius = CurrentWeapon->GetIdleAimRadiusRatio();

	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		Radius = sqrtf(ViewportSize.X * ViewportSize.X + ViewportSize.Y * ViewportSize.Y) * Radius;
		AimCircle->SetWeaponData(
			CurrentWeapon->GetRecoilRatio(), 
			Radius,
			CurrentWeapon->GetMaxAimRatio(),
			CurrentWeapon->GetAimRecoverRatio());
	}
}

void ASSPlayerCharacter::SwapWeapon()
{
	CurrentWeaponIndex++;
	if (CurrentWeaponIndex > OwnedWeapon.Num()-1) {
		CurrentWeaponIndex = 0;
	}
	CurrentWeapon->SetActorHiddenInGame(true);
	CurrentWeapon = OwnedWeapon[CurrentWeaponIndex];
	CurrentWeapon->SetActorHiddenInGame(false);
	GetFirstPersonMesh()->SetAnimInstanceClass(CurrentWeapon->GetFirstPersonAnimInstanceClass());
	GetMesh()->SetAnimInstanceClass(CurrentWeapon->GetThirdPersonAnimInstanceClass());
	SetAimCircle();
}

void ASSPlayerCharacter::EnableAiming()
{
	CurrentWeapon->EnableAiming();
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed*0.5f;
	SetAimCircle();
}

void ASSPlayerCharacter::DisableAiming()
{
	CurrentWeapon->DisableAiming();
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
	SetAimCircle();

}

void ASSPlayerCharacter::DoStartFiring()
{

	if (CurrentWeapon)
	{
		CurrentWeapon->StartFiring();
	}

}

void ASSPlayerCharacter::DoStopFiring()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFiring();
	}
}

FVector ASSPlayerCharacter::GetWeaponTargetLocation()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return FVector::ZeroVector;

	int32 ViewportX, ViewportY;
	PC->GetViewportSize(ViewportX, ViewportY);

	float Radius = AimCircle->GetCurrentIdleAimRadius();


	float Angle = FMath::FRandRange(0.f, 2 * PI);  // 랜덤한 각도(라디안)
	float Distance = FMath::Pow(FMath::FRand(),2.0f) * Radius * 0.8f;  // 중심에 더 가까운 밀도를 보장,FOV 보정 0.8(임시)
	PLLOG(Warning, TEXT("%f"), Distance);
	float RandX = FMath::Cos(Angle) * Distance;
	float RandY = FMath::Sin(Angle) * Distance;

	float ScreenX2 = ViewportX / 2.0f + RandX;
	float ScreenY2 = ViewportY / 2.0f + RandY;

	FVector WorldLocation2, WorldDirection2;
	if (PC->DeprojectScreenPositionToWorld(ScreenX2, ScreenY2, WorldLocation2, WorldDirection2))
	{
		FVector Start2 = WorldLocation2;
		FVector End2 = Start2 + (WorldDirection2 * 10000.0f); // 사거리 10000.0f

		FHitResult HitResult2;
		FCollisionQueryParams Params2;
		Params2.AddIgnoredActor(this);

		bool bHit2 = GetWorld()->LineTraceSingleByChannel(
			HitResult2,
			Start2,
			End2,
			ECC_Visibility,
			Params2
		);
		AimCircle->SetAimRecoil(CurrentWeapon->GetRecoilRatio());
		// 디버그 라인
		//DrawDebugLine(GetWorld(), Start2, bHit2 ? HitResult2.ImpactPoint : End2, FColor::Green, false, 2.0f, 0, 2.f);
		return bHit2 ? HitResult2.ImpactPoint : End2;
	}
	
	return FVector::ZeroVector;
}

void ASSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Firing
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &ASSPlayerCharacter::DoStartFiring);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &ASSPlayerCharacter::DoStopFiring);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &ASSPlayerCharacter::EnableAiming);
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &ASSPlayerCharacter::DisableAiming);
		EnhancedInputComponent->BindAction(SwapWeaponAction, ETriggerEvent::Started, this, &ASSPlayerCharacter::SwapWeapon);
	}
}


