// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrototypeL.h"
#include "PrototypeLPlayerController.h"
#include "SSPlayerController.generated.h"

class USSAimCircle; 

/**
 * 
 */
UCLASS()
class PROTOTYPEL_API ASSPlayerController : public APrototypeLPlayerController
{
	GENERATED_BODY()
	
public:
	ASSPlayerController();

	virtual void BeginPlay() override;

	TObjectPtr<USSAimCircle> GetAimCircleWidget() { return AimCircleWidget; };
protected:


	TSubclassOf<USSAimCircle> AimCircleClass;

	TObjectPtr<USSAimCircle> AimCircleWidget;
};
