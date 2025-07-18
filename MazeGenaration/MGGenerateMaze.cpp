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
			//(0,~),(~,0) ��ǥ�� �ܺ� �� ����
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
		
		//�湮�� ��ǥ�� ����
		NearPos.RemoveAll([this](const FVector2D& Pos) {
			return this->GetCellByPos(Pos)->GetVisited();
			});

		//�α� ��� ��ǥ�� �湮������
		if (NearPos.Num() == 0)
		{
			// �� Cell ǥ��
			if (!bPreviusPoped)\
				CurrentCell->SetEndCell();

			// �αٿ� �湮������ ��ǥ�� �����ϴ� cell ���� MazeStack Pop
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
			
			//������ ���� ����ؼ� �ش� ���� �� ����
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

		//�湮�� ��ǥ�� ����
		NearPos.RemoveAll([this](const FVector2D& Pos) {
			return this->GetCellByPos(Pos)->GetVisited();
			});

		//�α� ��� ��ǥ�� �湮������
		if (NearPos.Num() == 0)
		{
			// �� Cell ǥ��
			if (!bPreviusPopedForTimer)\
				CurrentCell->SetEndCell();

			// �αٿ� �湮������ ��ǥ�� �����ϴ� cell ���� MazeStack Pop
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
