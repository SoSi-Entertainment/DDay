// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WorldObject.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ETERNALTROOPS_API UWorldObject : public UObject
{
	GENERATED_BODY()

public:
	// Allows the Object to use BP_Functions
	class UWorld* GetWorld() const;
	class ULevel* GetLevel() const;
};