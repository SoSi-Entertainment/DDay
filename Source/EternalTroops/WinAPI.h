#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <windows.h>
#include <string>
#include "WinAPI.generated.h"

UCLASS(Blueprintable)
class ETERNALTROOPS_API UWinAPI : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "WinAPI")
    static FString GetMachineGuid();
};