#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CppFunctionLibrary.generated.h"

UCLASS()
class ETERNALTROOPS_API UCppFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Hash")
	static int32 HashFromString(const FString& Input);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	static void SpawnWeightedCount(const TArray<float>& Probabilities, int32 NumToSpawn, TArray<int32>& OutCounts);
};
