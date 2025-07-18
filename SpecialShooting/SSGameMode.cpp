// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialShooting/SSGameMode.h"
#include "SpecialShooting/SSPlayerCharacter.h"
#include "SpecialShooting/SSPlayerController.h"


ASSGameMode::ASSGameMode()
{

    DefaultPawnClass = ASSPlayerCharacter::StaticClass();
    PlayerControllerClass = ASSPlayerController::StaticClass();

}
