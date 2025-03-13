#include "AsyncPathfinding.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Actor.h"

UAsyncPathfinding* UAsyncPathfinding::FindPathAsync(UObject* WorldContextObject, FVector Start, FVector End)
{
    UAsyncPathfinding* Node = NewObject<UAsyncPathfinding>();
    Node->WorldContext = WorldContextObject;
    Node->StartLocation = Start;
    Node->EndLocation = End;
    return Node;
}

void UAsyncPathfinding::Activate()
{

    //UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
    UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull);
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

    AActor* NavAgent = Cast<AActor>(WorldContext);
    if (!NavAgent)
    {
        UE_LOG(LogTemp, Error, TEXT("NavAgent is null!"));
        return;
    }

    FNavLocation NavStart, NavEnd;
    if (!NavSys->ProjectPointToNavigation(StartLocation, NavStart))
    {
        UE_LOG(LogTemp, Warning, TEXT("StartLocation %s is not on NavMesh!"), *StartLocation.ToString());
        OnPathFound.Broadcast(TArray<FVector>());
        return;
    }

    if (!NavSys->ProjectPointToNavigation(EndLocation, NavEnd))
    {
        UE_LOG(LogTemp, Warning, TEXT("EndLocation %s is not on NavMesh!"), *EndLocation.ToString());
        OnPathFound.Broadcast(TArray<FVector>());
        return;
    }

    FPathFindingQuery Query(
        NavAgent,  // Навигационный агент
        *NavSys->GetDefaultNavDataInstance(), // Используем дефолтные данные навигации
        NavStart.Location,
        NavEnd.Location
    );

    UE_LOG(LogTemp, Log, TEXT("Starting async pathfinding from %s to %s"),
        *NavStart.Location.ToString(), *NavEnd.Location.ToString());

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
