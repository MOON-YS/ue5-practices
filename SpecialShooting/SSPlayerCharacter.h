// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrototypeL.h"
#include "PrototypeLCharacter.h"
#include "SSPlayerCharacter.generated.h"

class ASSWeaponBase;
class USSAimCircle;
class UInputAction;
/**
 * 
 */
UCLASS(BlueprintType)
class PROTOTYPEL_API ASSPlayerCharacter : public APrototypeLCharacter
{
	GENERATED_BODY()
public:
	ASSPlayerCharacter();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	FName FirstPersonWeaponSocket = FName("HandGrip_R");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapons")
	FName ThirdPersonWeaponSocket = FName("HandGrip_R");

public:
	/** Handles start firing input */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStartFiring();

	/** Handles stop firing input */
	UFUNCTION(BlueprintCallable, Category = "Input")
	void DoStopFiring();

	FVector GetWeaponTargetLocation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AimingAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SwapWeaponAction;

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	void SetAimCircle();
	TObjectPtr<ASSWeaponBase> CurrentWeapon;
	TObjectPtr<USSAimCircle> AimCircle;
	TArray<ASSWeaponBase*> OwnedWeapon;
	int32 CurrentWeaponIndex = 0;
	float MaxSpeed = 300.f;
	void SwapWeapon();

	void EnableAiming();
	void DisableAiming();

};
