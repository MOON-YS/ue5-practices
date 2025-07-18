// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialShooting/SSWeaponPistol.h"

ASSWeaponPistol::ASSWeaponPistol()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Pistol/Meshes/SKM_Pistol.SKM_Pistol'"));
	if (WeaponMeshAsset.Succeeded())
	{
		FirstPersonMesh->SetSkeletalMesh(WeaponMeshAsset.Object);
		ThirdPersonMesh->SetSkeletalMesh(WeaponMeshAsset.Object);
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClassFirst(TEXT("/Script/Engine.AnimBlueprint'/Game/PrototypeL/Anim/ABP_FP_Pistol.ABP_FP_Pistol_C'"));
	if (AnimClassFirst.Succeeded())
	{
		FirstPersonAnimInstanceClass = AnimClassFirst.Class;
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClassThird(TEXT("/Script/Engine.AnimBlueprint'/Game/PrototypeL/Anim/ABP_TP_Pistol.ABP_TP_Pistol_C'"));
	if (AnimClassThird.Succeeded())
	{
		ThirdPersonAnimInstanceClass = AnimClassThird.Class;
	}

	
	bFullAuto = false;
	RefireRate = 0.2f;

	IdleAimRadiusRatio = 0.01f;
	MaxAimRatio = 10.0f;
	RecoilRate = 3.0f;
	AimRecoverRatio = 2.0f;

	AimedAimRadiusRatio = 0.005;
	AimedRecoilRate = 3.5f;
	AimedAimRecoverRatio = 3.0f;
	AimedMaxAimRatio = 7.5f;
}
