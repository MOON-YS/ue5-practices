// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialShooting/SSWeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "SpecialShooting/SSPlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "SpecialShooting/SSProjectile.h"
// Sets default values
ASSWeaponBase::ASSWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));


	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));
	FirstPersonMesh->SetupAttachment(RootComponent);

	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	FirstPersonMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::FirstPerson);
	FirstPersonMesh->bOnlyOwnerSee = true;

	// create the third person mesh
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Third Person Mesh"));
	ThirdPersonMesh->SetupAttachment(RootComponent);

	ThirdPersonMesh->SetCollisionProfileName(FName("NoCollision"));
	ThirdPersonMesh->SetFirstPersonPrimitiveType(EFirstPersonPrimitiveType::WorldSpaceRepresentation);
	ThirdPersonMesh->bOwnerNoSee = true;

}

// Called when the game starts or when spawned
void ASSWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASSWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASSWeaponBase::Fire()
{
	if (!bIsFiring)
	{
		return;
	}

	ASSPlayerCharacter* WeaponOwner = Cast<ASSPlayerCharacter>(GetOwner());
	if (WeaponOwner != nullptr)
	{
		FireProjectile(WeaponOwner->GetWeaponTargetLocation());
	}
	


	TimeOfLastShot = GetWorld()->GetTimeSeconds();
	if (bFullAuto)
	{
		// schedule the next shot
		GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &ASSWeaponBase::Fire, RefireRate, false);
	}
	else {

		// for semi-auto weapons, schedule the cooldown notification
		GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &ASSWeaponBase::FireCooldownExpired, RefireRate, false);

	}
}

void ASSWeaponBase::FireCooldownExpired()
{
}

void ASSWeaponBase::FireProjectile(const FVector& TargetLocation)
{
	FVector Start = FirstPersonMesh->GetSocketLocation(MuzzleSocketName);
	// get the projectile transform
	FTransform ProjectileTransform = CalculateProjectileSpawnTransform(TargetLocation);

	// spawn the projectile
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.TransformScaleMethod = ESpawnActorScaleMethod::OverrideRootScale;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	ASSProjectile* Projectile = GetWorld()->SpawnActor<ASSProjectile>(ASSProjectile::StaticClass(), ProjectileTransform, SpawnParams);

	// 디버그 라인
	/*DrawDebugLine(GetWorld(), Start, TargetLocation, FColor::Blue, false, 2.0f, 0, 2.0f);*/
}

FTransform ASSWeaponBase::CalculateProjectileSpawnTransform(const FVector& TargetLocation) const
{
	// find the muzzle location
	const FVector MuzzleLoc = FirstPersonMesh->GetSocketLocation(MuzzleSocketName);

	// calculate the spawn location ahead of the muzzle
	const FVector SpawnLoc = MuzzleLoc + ((TargetLocation - MuzzleLoc).GetSafeNormal() * MuzzleOffset);

	// find the aim rotation vector while applying some variance to the target 
	const FRotator AimRot = UKismetMathLibrary::FindLookAtRotation(SpawnLoc, TargetLocation);

	// return the built transform
	return FTransform(AimRot, SpawnLoc, FVector::OneVector);

}

void ASSWeaponBase::StartFiring()
{
	// raise the firing flag
	bIsFiring = true;

	// check how much time has passed since we last shot
	// this may be under the refire rate if the weapon shoots slow enough and the player is spamming the trigger
	const float TimeSinceLastShot = GetWorld()->GetTimeSeconds() - TimeOfLastShot;

	if (TimeSinceLastShot > RefireRate)
	{
		// fire the weapon right away
		Fire();

	}
	else {

		// if we're full auto, schedule the next shot
		if (bFullAuto)
		{
			GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &ASSWeaponBase::Fire, TimeSinceLastShot, false);
		}

	}
}

void ASSWeaponBase::StopFiring()
{
	bIsFiring = false;

	// clear the refire timer
	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
}

const TSubclassOf<UAnimInstance>& ASSWeaponBase::GetFirstPersonAnimInstanceClass() const
{
	return FirstPersonAnimInstanceClass;
}

const TSubclassOf<UAnimInstance>& ASSWeaponBase::GetThirdPersonAnimInstanceClass() const
{
	// TODO: 여기에 return 문을 삽입합니다.
	return ThirdPersonAnimInstanceClass;
}

void ASSWeaponBase::EnableAiming()
{
	FirstPersonMesh->SetRelativeLocationAndRotation(FVector(3.0f, -6.0f, 0.0f), FQuat(FRotator(-26.0f, 0.f, 0.f)));
	bAimed = true;
}

void ASSWeaponBase::DisableAiming()
{
	FirstPersonMesh->SetRelativeLocationAndRotation(FVector(.0f, .0f, .0f), FQuat(FRotator(.0f, .0f, .0f)));
	bAimed = false;
}

