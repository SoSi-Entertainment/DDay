#include "AsyncPathfinding.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Actor.h"
#include "AIController.h"

UAsyncPathfinding* UAsyncPathfinding::FindPathAsync(AAIController* Controller, FVector Start, FVector End)
{
    UAsyncPathfinding* Node = NewObject<UAsyncPathfinding>();
    Node->AIController = Controller;
    Node->StartLocation = Start;
    Node->EndLocation = End;
    return Node;
}

void UAsyncPathfinding::Activate()
{
    if (!AIController)
    {
        UE_LOG(LogTemp, Error, TEXT("AIController is null!"));
        return;
    }

    APawn* Pawn = AIController->GetPawn();
    if (!Pawn)
    {
        UE_LOG(LogTemp, Error, TEXT("Pawn is null!"));
        return;
    }

    UWorld* World = Pawn->GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("World is null!"));
        return;
    }

    UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (!NavSys)
    {
        UE_LOG(LogTemp, Error, TEXT("Navigation System is null!"));
        return;
    }

    FNavLocation NavStart, NavEnd;

    if (!NavSys->ProjectPointToNavigation(StartLocation, NavStart))
    {
        UE_LOG(LogTemp, Warning, TEXT("Start not on NavMesh"));
        OnPathFound.Broadcast(TArray<FVector>());
        return;
    }

    if (!NavSys->ProjectPointToNavigation(EndLocation, NavEnd))
    {
        UE_LOG(LogTemp, Warning, TEXT("End not on NavMesh"));
        OnPathFound.Broadcast(TArray<FVector>());
        return;
    }

    FPathFindingQuery Query(
        Pawn,
        *NavSys->GetDefaultNavDataInstance(),
        NavStart.Location,
        NavEnd.Location
    );

    NavSys->FindPathAsync(
        Query.NavAgentProperties,
        Query,
        FNavPathQueryDelegate::CreateUObject(this, &UAsyncPathfinding::OnPathCompleted),
        EPathFindingMode::Regular
    );
}

void UAsyncPathfinding::OnPathCompleted(uint32 RequestID, ENavigationQueryResult::Type Result, FNavPathSharedPtr Path)
{
    UE_LOG(LogTemp, Log, TEXT("OnPathCompleted called with result: %d"), (int32)Result);

    if (Path.IsValid() && Result == ENavigationQueryResult::Success)
    {
        TArray<FVector> PathPoints;
        for (const auto& Point : Path->GetPathPoints())
        {
            PathPoints.Add(Point.Location);
        }

        UE_LOG(LogTemp, Log, TEXT("Path found with %d points"), PathPoints.Num());
        OnPathFound.Broadcast(PathPoints);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Pathfinding failed or returned empty path."));
        OnPathFound.Broadcast(TArray<FVector>());
    }
}
