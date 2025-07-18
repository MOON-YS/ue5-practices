// Fill out your copyright notice in the Description page of Project Settings.
#include "MazeGenaration/MGGenerateMaze.h"
#include "MazeGenaration/MGMazeCell.h"
#include "MazeGenaration/MGVisCursor.h"
void AMGGenerateMaze::BeginPlay()
{
	Super::BeginPlay();
	
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	for (int i = 0; i < MazeSize; ++i)
	{
		for (int j = 0; j < MazeSize; ++j)
		{
			AMGMazeCell* Cell = GetWorld()->SpawnActor<AMGMazeCell>(AMGMazeCell::StaticClass(), FVector(300.f * i, 300.f * j, 0.f) , SpawnRotation, SpawnParams);
			//(0,~),(~,0) 좌표의 외부 벽 생성
			if (i == 0)
			{
				Cell->SetSouthWallVisible();
			}
			if (j == 0)
			{
				Cell->SetWestWallVisible();
			}
			Cell->SetPos(FVector2D(i, j), MazeSize);
			if (i == 0 && j == 0)\
				Cell->SetStartCell();
			MazeCells.Add(Cell);
		}
	}
	SetupMaze();

	//Viusalize Maze Genaration
	//CursorActor = GetWorld()->SpawnActor<AMGVisCursor>(AMGVisCursor::StaticClass(), FVector(0.f, 0.f, 125.f), SpawnRotation, SpawnParams);
	//GetWorldTimerManager().SetTimer(MazeVisualize, this, &AMGGenerateMaze::SetupMazeForVisualize, 0.05f, true);
}

void AMGGenerateMaze::SetupMaze()
{
	bool bPreviusPoped = false;

	//Generate a maze using Depth-First Search algorithm with walls set to be hidden
	while (!CheckAllVisited())
	{
		
		AMGMazeCell* CurrentCell = GetCellByPos(CurrentPos);
		CurrentCell->SetVisited();
		TArray<FVector2D> NearPos = CurrentCell->GetNearPos();
		
		//방문한 좌표는 제외
		NearPos.RemoveAll([this](const FVector2D& Pos) {
			return this->GetCellByPos(Pos)->GetVisited();
			});

		//인근 모든 좌표를 방문했을때
		if (NearPos.Num() == 0)
		{
			// 끝 Cell 표시
			if (!bPreviusPoped)\
				CurrentCell->SetEndCell();

			// 인근에 방문가능한 좌표가 존재하는 cell 까지 MazeStack Pop
			CurrentPos = MazeStack.Pop();
			bPreviusPoped = true;
		}
		else
		{
			bPreviusPoped = false;
			MazeStack.Push(CurrentPos);
			int32 RandomIndex = FMath::RandRange(0, NearPos.Num() - 1);
			FVector2D NextPos = NearPos[RandomIndex];
			AMGMazeCell* NextCell = GetCellByPos(NextPos);
			
			//움직인 방향 계산해서 해당 방향 벽 제거
			FVector2D DiffPos = NextPos - CurrentPos;
			if (DiffPos == FVector2D(1, 0))
			{
				//Moved North
				CurrentCell->SetNorthWallHidden();
			}
			else if (DiffPos == FVector2D(-1, 0))
			{
				//Moved South
				NextCell->SetNorthWallHidden();
			}
			else if (DiffPos == FVector2D(0, 1))
			{
				//Moved East
				CurrentCell->SetEastWallHidden();

			}
			else if (DiffPos == FVector2D(0, -1))
			{
				//Moved West
				NextCell->SetEastWallHidden();
			}
			CurrentPos = NextPos;
		}

	}
}

void AMGGenerateMaze::SetupMazeForVisualize()
{
	//Generate a maze using Depth-First Search algorithm with walls set to be hidden
	if (!CheckAllVisited())
	{
		AMGMazeCell* CurrentCell = GetCellByPos(CurrentPos);
		CurrentCell->SetVisited();
		
		TArray<FVector2D> NearPos = CurrentCell->GetNearPos();

		//방문한 좌표는 제외
		NearPos.RemoveAll([this](const FVector2D& Pos) {
			return this->GetCellByPos(Pos)->GetVisited();
			});

		//인근 모든 좌표를 방문했을때
		if (NearPos.Num() == 0)
		{
			// 끝 Cell 표시
			if (!bPreviusPopedForTimer)\
				CurrentCell->SetEndCell();

			// 인근에 방문가능한 좌표가 존재하는 cell 까지 MazeStack Pop
			CurrentPos = MazeStack.Pop();
			CursorActor->SetActorLocation(FVector(CurrentPos.X * 300, CurrentPos.Y * 300, 300.f));
			bPreviusPopedForTimer = true;
		}
		else
		{
			bPreviusPopedForTimer = false;
			MazeStack.Push(CurrentPos);
			int32 RandomIndex = FMath::RandRange(0, NearPos.Num() - 1);
			FVector2D NextPos = NearPos[RandomIndex];
			AMGMazeCell* NextCell = GetCellByPos(NextPos);

			FVector2D DiffPos = NextPos - CurrentPos;
			if (DiffPos == FVector2D(1, 0))
			{
				//Moved North
				CurrentCell->SetNorthWallHidden();
			}
			else if (DiffPos == FVector2D(-1, 0))
			{
				//Moved South
				NextCell->SetNorthWallHidden();
			}
			else if (DiffPos == FVector2D(0, 1))
			{
				//Moved East
				CurrentCell->SetEastWallHidden();

			}
			else if (DiffPos == FVector2D(0, -1))
			{
				//Moved West
				NextCell->SetEastWallHidden();
			}
			CurrentPos = NextPos;
			CursorActor->SetActorLocation(FVector(CurrentPos.X * 1000, CurrentPos.Y * 1000, 125.f));
		}
	}
	else
	{
		GetWorldTimerManager().ClearTimer(MazeVisualize);
	}
}

AMGMazeCell* AMGGenerateMaze::GetCellByPos(FVector2D Pos)
{
	for (AMGMazeCell* Cell : MazeCells) {
		if (Cell->GetPos() == Pos) return Cell;
	}
	return nullptr;
}

bool AMGGenerateMaze::CheckAllVisited()
{
	bool bAllVisitedMask = true;
	for (AMGMazeCell* Cell : MazeCells) {
		bAllVisitedMask = bAllVisitedMask && Cell->GetVisited();
	}
	return bAllVisitedMask;
}
