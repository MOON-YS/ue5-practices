// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialShooting/SSPlayerController.h"
#include "InputMappingContext.h"
#include "SpecialShooting/SSAimCircle.h"


ASSPlayerController::ASSPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Default(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (IMC_Default.Succeeded())
	{
		DefaultMappingContexts.Add(IMC_Default.Object);
	}
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_MouseLook(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_MouseLook.IMC_MouseLook'"));
	if (IMC_MouseLook.Succeeded())
	{
		DefaultMappingContexts.Add(IMC_MouseLook.Object);
	}
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Weapons(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Variant_Shooter/Input/IMC_Weapons.IMC_Weapons'"));
	if (IMC_Weapons.Succeeded())
	{
		DefaultMappingContexts.Add(IMC_Weapons.Object);
	}
	static ConstructorHelpers::FClassFinder<USSAimCircle> UI_AimCircle(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/PrototypeL/UI/WBP_AimCircle.WBP_AimCircle_C'"));
	if (UI_AimCircle.Succeeded())
	{
		AimCircleClass = UI_AimCircle.Class;
	}
}

void ASSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	AimCircleWidget = CreateWidget<USSAimCircle>(this, AimCircleClass);
	AimCircleWidget->AddToPlayerScreen(0);
}
