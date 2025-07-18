// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrototypeL.h"
#include "GameFramework/Actor.h"
#include "SSWeaponBase.generated.h"

UCLASS(abstract)
class PROTOTYPEL_API ASSWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASSWeaponBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ThirdPersonMesh;
	/** Third person perspective mesh */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	//USkeletalMeshComponent* ThirdPersonMesh;

	/** Animation montage to play when firing this weapon */
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* FiringMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> FirstPersonAnimInstanceClass;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> ThirdPersonAnimInstanceClass;

	UPROPERTY(EditAnywhere, Category = "Aim")
	float MuzzleOffset = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void Fire();

	UPROPERTY(EditAnywhere, Category = "Refire")
	bool bFullAuto = true;

	UPROPERTY(EditAnywhere, Category = "Refire")
	float RefireRate = 0.2f;

	float TimeOfLastShot = 0.0f;

	bool bIsFiring = false;
	bool bAimed = false;

	float IdleAimRadiusRatio = 0.01f;
	float RecoilRate = 1.2f;
	float AimRecoverRatio = 2.0f;
	float MaxAimRatio = 15.0f;

	float AimedAimRadiusRatio = 0.005;
	float AimedRecoilRate = 1.2f;
	float AimedAimRecoverRatio = 1.0f;
	float AimedMaxAimRatio = 7.5f;


	FTimerHandle RefireTimer;

	UPROPERTY(EditAnywhere, Category = "Aim")
	FName MuzzleSocketName = FName("Muzzle");

	void FireCooldownExpired();

	virtual void FireProjectile(const FVector& TargetLocation);
	FTransform CalculateProjectileSpawnTransform(const FVector& TargetLocation) const;


public:
	void StartFiring();

	void StopFiring();

public:
	const TSubclassOf<UAnimInstance>& GetFirstPersonAnimInstanceClass() const;

	/** Returns the third person anim instance class */
	const TSubclassOf<UAnimInstance>& GetThirdPersonAnimInstanceClass() const;

	UFUNCTION(BlueprintPure, Category = "Weapon")
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; };

	/** Returns the third person mesh */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	USkeletalMeshComponent* GetThirdPersonMesh() const { return ThirdPersonMesh; };

	UFUNCTION(BlueprintPure, Category = "Weapon")
	float GetIdleAimRadiusRatio() { return bAimed ? AimedAimRadiusRatio : IdleAimRadiusRatio; };

	UFUNCTION(BlueprintPure, Category = "Weapon")
	float GetRecoilRatio() { return bAimed ? AimedRecoilRate : RecoilRate; };

	UFUNCTION(BlueprintPure, Category = "Weapon")
	float GetMaxAimRatio() { return bAimed ? AimedMaxAimRatio : MaxAimRatio; };

	UFUNCTION(BlueprintPure, Category = "Weapon")
	float GetAimRecoverRatio() { return bAimed ? AimedAimRecoverRatio : AimRecoverRatio; };

	bool GetAimed() { return bAimed; };

	void EnableAiming();
	void DisableAiming();

};
