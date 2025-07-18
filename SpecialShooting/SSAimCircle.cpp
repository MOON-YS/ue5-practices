// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialShooting/SSAimCircle.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
void USSAimCircle::NativeConstruct()
{
	AimCircle = Cast<UBorder>(GetWidgetFromName(AimCircleName));
	CanvasSlot = Cast<UCanvasPanelSlot>(AimCircle->Slot);
	FAnchors Anchors = CanvasSlot->GetAnchors();
	FVector2D AnchorSize = CanvasSlot->GetSize();
}



void USSAimCircle::SetIdleAimRadius(float NewAimRadius)
{
	CanvasSlot->SetSize(FVector2D(NewAimRadius * 2, NewAimRadius * 2));
	IdleAimRadius = NewAimRadius;
	CurrentAimRadius = IdleAimRadius;	
}

void USSAimCircle::SetAimRecoil(float Recoil)
{
	CurrentAimRadius = FMath::Clamp(CurrentAimRadius * CurrentRecoil, IdleAimRadius, IdleAimRadius*CurrentMaxAimRadius);
	CanvasSlot->SetSize(FVector2D(CurrentAimRadius * 2, CurrentAimRadius * 2));
}

void USSAimCircle::UpdateAimTick(float DeltaTime)
{
	float InterpSpeed = 2.0f; // 얼마나 빠르게 목표값으로 수렴할지
	CurrentAimRadius = FMath::FInterpTo(CurrentAimRadius, IdleAimRadius, DeltaTime, CurrentRecoverSpeed);
	CanvasSlot->SetSize(FVector2D(CurrentAimRadius * 2, CurrentAimRadius * 2));
}

void USSAimCircle::SetWeaponData(float Recoil, float AimRadius, float MaxAimRadius, float RecoverRatio)
{
	IdleAimRadius = AimRadius;
	CurrentRecoil = Recoil;
	CurrentMaxAimRadius = MaxAimRadius;
	CurrentRecoverSpeed = RecoverRatio;

}
