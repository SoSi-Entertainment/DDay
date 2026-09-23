#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CppFunctionLibrary.generated.h"

class ACharacter;

UCLASS()
class ETERNALTROOPS_API UCppFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Hash")
	static int32 HashFromString(const FString& Input);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	static void SpawnWeightedCount(const TArray<float>& Probabilities, int32 NumToSpawn, TArray<int32>& OutCounts);

	UFUNCTION(BlueprintCallable, Category = "My_Editor_Scripts")
	static UActorComponent* SetCreationMethod(UActorComponent* ActorComponent, EComponentCreationMethod method);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	static void SetCameraMovableWhenPaused(UObject* WorldContextObject, bool bMovable);

	UFUNCTION(BlueprintCallable, Category = "Movement|Avoidance")
	static void SetAvoidanceConsiderationRadius(ACharacter* Character, float Radius);

	UFUNCTION(BlueprintCallable, Category = "Movement|Avoidance")
	static void RemoveFromAvoidanceManager(ACharacter* Character);
};