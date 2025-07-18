// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenaration/MGVisCursor.h"

// Sets default values
AMGVisCursor::AMGVisCursor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CursorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cursor"));
		
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_ChamferCube1.SM_ChamferCube1'"));
	if (MeshAsset.Succeeded())
	{
		CursorMesh->SetStaticMesh(MeshAsset.Object);
	}

}

// Called when the game starts or when spawned
void AMGVisCursor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMGVisCursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

