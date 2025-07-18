// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialShooting/SSWeaponRifle.h"

ASSWeaponRifle::ASSWeaponRifle()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponMeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapons/Rifle/Meshes/SKM_Rifle.SKM_Rifle'"));
	if (WeaponMeshAsset.Succeeded())
	{
		FirstPersonMesh->SetSkeletalMesh(WeaponMeshAsset.Object);
		ThirdPersonMesh->SetSkeletalMesh(WeaponMeshAsset.Object);
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClassFirst(TEXT("/Script/Engine.AnimBlueprint'/Game/PrototypeL/Anim/ABP_FP_Weapon.ABP_FP_Weapon_C'"));
	if (AnimClassFirst.Succeeded())
	{
		FirstPersonAnimInstanceClass = AnimClassFirst.Class;
	}
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimClassThird(TEXT("/Script/Engine.AnimBlueprint'/Game/PrototypeL/Anim/ABP_TP_Rifle.ABP_TP_Rifle_C'"));
	if (AnimClassThird.Succeeded())
	{
		ThirdPersonAnimInstanceClass = AnimClassThird.Class;
	}

	bFullAuto = true;
	RefireRate = 0.1f;


	IdleAimRadiusRatio = 0.008f;
	MaxAimRatio = 15.0f;
	RecoilRate = 1.5f;
	AimRecoverRatio = 2.0f;

	AimedAimRadiusRatio = 0.004f;
	AimedRecoilRate = 1.2f;
	AimedAimRecoverRatio = 3.0f;
	AimedMaxAimRatio = 7.5f;

}
