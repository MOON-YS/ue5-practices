// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PrototypeL.h"
#include "Engine/LevelScriptActor.h"
#include "MGGenerateMaze.generated.h"
class AMGMazeCell;
/**
 * 
 */
UCLASS()
class PROTOTYPEL_API AMGGenerateMaze : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	TArray<AMGMazeCell*> MazeCells;
	TArray<FVector2D> MazeStack;
	FVector2D CurrentPos = FVector2D(0,0);

	int32 MazeSize = 16;

	bool bIsAllVisited = false;

	FTimerHandle MazeVisualize;
	bool bPreviusPopedForTimer = false;
	class AMGVisCursor* CursorActor;
protected:
	void SetupMaze();
	void SetupMazeForVisualize();
	AMGMazeCell* GetCellByPos(FVector2D Pos);
	bool CheckAllVisited();

};
