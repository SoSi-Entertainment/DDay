#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AsyncPathfinding.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPathFound, const TArray<FVector>&, PathPoints);

UCLASS()
class ETERNALTROOPS_API UAsyncPathfinding : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnPathFound OnPathFound;

    UPROPERTY()
    AAIController* AIController;

    UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
    static UAsyncPathfinding* FindPathAsync(
        AAIController* Controller,
        FVector Start,
        FVector End
    );

    virtual void Activate() override;

private:
    void OnPathCompleted(uint32 RequestID, ENavigationQueryResult::Type Result, FNavPathSharedPtr Path);

    UObject* WorldContext;
    FVector StartLocation;
    FVector EndLocation;
};
