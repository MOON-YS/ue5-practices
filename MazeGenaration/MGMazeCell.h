// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrototypeL.h"
#include "GameFramework/Actor.h"
#include "MGMazeCell.generated.h"

UCLASS()
class PROTOTYPEL_API AMGMazeCell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMGMazeCell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshs", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Floor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshs", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* NorthWall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshs", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SouthWall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshs", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* EastWall;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshs", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WestWall;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshs", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Pillar1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshs", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Pillar2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshs", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Pillar3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshs", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Pillar4;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshs", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* EndFloorMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meshs", meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* StartFloorMaterial;

	FVector2D Pos;
	TArray<FVector2D> NearPos;
	bool bVisited = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Maze Property", meta = (AllowPrivateAccess = "true"))
	bool bEndCell = false;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetSouthWallVisible();
	void SetWestWallVisible();

	void SetNorthWallHidden();
	void SetEastWallHidden();

	void SetPos(FVector2D InputPos, int32 MaxSize);
	FVector2D GetPos() { return Pos;};
	TArray<FVector2D> GetNearPos() { return NearPos; };
	bool GetVisited() { return bVisited; };
	void SetVisited() { bVisited = true; };
	void SetEndCell();
	void SetStartCell();

};
