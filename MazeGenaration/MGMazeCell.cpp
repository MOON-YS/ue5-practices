// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeGenaration/MGMazeCell.h"

// Sets default values
AMGMazeCell::AMGMazeCell()
{
	RootComponent = Floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Floor"));

	NorthWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("North Wall"));
	NorthWall->SetupAttachment(RootComponent);

	SouthWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("South Wall"));
	SouthWall->SetupAttachment(RootComponent);

	EastWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("East Wall"));
	EastWall->SetupAttachment(RootComponent);

	WestWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("West Wall"));
	WestWall->SetupAttachment(RootComponent);

	//Pillar1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar1"));
	//Pillar1->SetupAttachment(RootComponent);

	//Pillar2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar2"));
	//Pillar2->SetupAttachment(RootComponent);

	//Pillar3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar3"));
	//Pillar3->SetupAttachment(RootComponent);

	//Pillar4 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pillar4"));
	//Pillar4->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> WallMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/PrototypeL/Meshs/SM_Template_Map_Floor_300x300.SM_Template_Map_Floor_300x300'"));
	if (WallMesh.Succeeded())
	{
		Floor->SetStaticMesh(WallMesh.Object);

		FTransform WallTransform;
		

		WallTransform.SetScale3D(FVector(1.5f, 1.f, 1.0f));
		WallTransform.SetLocation(FVector(135.0f, 0.0f, 225.f));
		WallTransform.SetRotation(FQuat(FRotator(90.f, 0.f, 0.f)));
		NorthWall->SetStaticMesh(WallMesh.Object);
		NorthWall->SetRelativeTransform(WallTransform);

		WallTransform.SetLocation(FVector(-150.0f, 0.0f, 225.f));
		SouthWall->SetStaticMesh(WallMesh.Object);
		SouthWall->SetRelativeTransform(WallTransform);
		SouthWall->SetHiddenInGame(true);
		SouthWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);



		WallTransform.SetScale3D(FVector(1.f, 1.5f, 1.0f));
		WallTransform.SetLocation(FVector(0.0f, 150.0f, 225.f));
		WallTransform.SetRotation(FQuat(FRotator(0.f, 0.f, 90.f)));
		EastWall->SetStaticMesh(WallMesh.Object);
		EastWall->SetRelativeTransform(WallTransform);


		WallTransform.SetLocation(FVector(0.0f, -135.0f, 225.f));
		WestWall->SetStaticMesh(WallMesh.Object);
		WestWall->SetRelativeTransform(WallTransform);
		WestWall->SetHiddenInGame(true);
		WestWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//Pillar1->SetStaticMesh(WallMesh.Object);
		//Pillar2->SetStaticMesh(WallMesh.Object);
		//Pillar3->SetStaticMesh(WallMesh.Object);
		//Pillar4->SetStaticMesh(WallMesh.Object);
	
		//FTransform PillarTransform;
		//PillarTransform.SetScale3D(FVector(0.05f, 1.5f, 1.0f));
		//PillarTransform.SetRotation(FQuat(FRotator(0.f, 0.f, 90.f)));

		//PillarTransform.SetLocation(FVector(142.5f, -135.f, 225.f));
		//Pillar1->SetRelativeTransform(PillarTransform);

		//PillarTransform.SetLocation(FVector(-142.5f, -135.f, 225.f));
		//Pillar2->SetRelativeTransform(PillarTransform);

		//PillarTransform.SetLocation(FVector(142.5f, 150.f, 225.f));
		//Pillar3->SetRelativeTransform(PillarTransform);

		//PillarTransform.SetLocation(FVector(-142.5f, 150.f, 225.f));
		//Pillar4->SetRelativeTransform(PillarTransform);



	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface> EndFloorMaterialFinder(
		TEXT("/Script/Engine.MaterialInstanceConstant'/AnimationSharing/AnimSharingRed.AnimSharingRed'"));
	if (EndFloorMaterialFinder.Succeeded())
	{
		EndFloorMaterial = EndFloorMaterialFinder.Object;
	}
	ConstructorHelpers::FObjectFinder<UMaterialInterface> StartFloorMaterialFinder(
		TEXT("/Script/Engine.MaterialInstanceConstant'/AnimationSharing/AnimSharingBlue.AnimSharingBlue'"));
	if (StartFloorMaterialFinder.Succeeded())
	{
		StartFloorMaterial = StartFloorMaterialFinder.Object;
	}
}

// Called when the game starts or when spawned
void AMGMazeCell::BeginPlay()
{
	Super::BeginPlay();
	FVector WorldScale = Floor->GetComponentToWorld().GetScale3D();
	FBoxSphereBounds Bounds = Floor->GetStaticMesh()->GetBounds();
	FVector MeshExtent = Bounds.BoxExtent * 2.0f;
	FVector WorldSizeCM = MeshExtent * WorldScale;
}

// Called every frame
void AMGMazeCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMGMazeCell::SetSouthWallVisible()
{
	SouthWall->SetHiddenInGame(false);
	SouthWall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void AMGMazeCell::SetWestWallVisible()
{
	WestWall->SetHiddenInGame(false);
	WestWall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void AMGMazeCell::SetNorthWallHidden()
{
	NorthWall->SetHiddenInGame(true);
	NorthWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMGMazeCell::SetEastWallHidden()
{
	EastWall->SetHiddenInGame(true);
	EastWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AMGMazeCell::SetPos(FVector2D InputPos, int32 MaxSize)
{
	Pos = InputPos;

	// Add North, except North Edge
	if (InputPos.X != MaxSize-1)\
		NearPos.Add(FVector2D(InputPos.X+1, InputPos.Y));
	// Add South, except South Edge
	if(InputPos.X != 0)\
		NearPos.Add(FVector2D(InputPos.X-1, InputPos.Y));
	// Add East, except East Edge
	if (InputPos.Y != MaxSize-1)\
		NearPos.Add(FVector2D(InputPos.X, InputPos.Y+1));
	// Add West, except West Edge
	if(InputPos.Y != 0)\
		NearPos.Add(FVector2D(InputPos.X, InputPos.Y-1));

}

void AMGMazeCell::SetEndCell()
{
	bEndCell = true;
	if(EndFloorMaterial != nullptr)\
		Floor->SetMaterial(0, EndFloorMaterial);
}

void AMGMazeCell::SetStartCell()
{
	if (StartFloorMaterial != nullptr)\
		Floor->SetMaterial(0, StartFloorMaterial);
}

