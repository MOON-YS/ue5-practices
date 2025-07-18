// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrototypeL.h"
#include "Blueprint/UserWidget.h"
#include "SSAimCircle.generated.h"

class UBorder;
class UCanvasPanelSlot;
/**
 * 
 */
UCLASS()
class PROTOTYPEL_API USSAimCircle : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	FName AimCircleName = FName("AimCircle");
	UPROPERTY(meta = (BindWidget))
	UBorder* AimCircle;
	UCanvasPanelSlot* CanvasSlot;
	float IdleAimRadius;
	float CurrentAimRadius;
	float CurrentMaxAimRadius;
	float CurrentRecoil;
	float CurrentRecoverSpeed = 0.0f;
public:
	virtual void NativeConstruct() override;
	void SetIdleAimRadius(float NewAimRadius);
	float GetCurrentIdleAimRadius() { return CurrentAimRadius; };
	void SetAimRecoil(float Recoil);
	void UpdateAimTick(float DeltaTime);
	void SetWeaponData(float Recoil, float AimRadius, float MaxAimRadius, float RecoverRatio);
};
