#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MD5Calculator.generated.h"

/**
 * MD5 Calculator Utility Class
 */
UCLASS(Blueprintable)
class ETERNALTROOPS_API UMD5Calculator : public UObject
{
	GENERATED_BODY()

public:

	// Static function to calculate MD5 hash of a string
	UFUNCTION(BlueprintCallable, Category = "MD5")
	static FString CalculateMD5(const FString& Input);
};
