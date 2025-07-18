// Copyright Epic Games, Inc. All Rights Reserved.


#include "PrototypeLPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "PrototypeLCameraManager.h"

APrototypeLPlayerController::APrototypeLPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = APrototypeLCameraManager::StaticClass();
}

void APrototypeLPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
