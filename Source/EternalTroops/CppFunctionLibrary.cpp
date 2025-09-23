#include "CppFunctionLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "ComponentInstanceDataCache.h"

int32 UCppFunctionLibrary::HashFromString(const FString& Input)
{
	return GetTypeHash(Input);
}


void UCppFunctionLibrary::SpawnWeightedCount(const TArray<float>& Probabilities, int32 NumToSpawn, TArray<int32>& OutCounts)
{
    OutCounts.Init(0, Probabilities.Num());

    if (Probabilities.Num() == 0 || NumToSpawn <= 0)
    {
        return;
    }

    float TotalWeight = 0.0f;
    for (float W : Probabilities)
    {
        if (W < 0.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Weights must be non-negative"));
            return;
        }
        TotalWeight += W;
    }

    if (TotalWeight <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Total weight must be positive"));
        return;
    }

    TArray<float> Remainders;
    Remainders.SetNum(Probabilities.Num());

    int32 SumCounts = 0;

    // Вычисляем целую часть и остатки
    for (int32 i = 0; i < Probabilities.Num(); ++i)
    {
        float ExactCount = (Probabilities[i] / TotalWeight) * NumToSpawn;
        int32 IntPart = FMath::FloorToInt(ExactCount);
        OutCounts[i] = IntPart;
        Remainders[i] = ExactCount - IntPart;
        SumCounts += IntPart;
    }

    int32 Remaining = NumToSpawn - SumCounts;

    // Распределяем оставшиеся по наибольшему остатку
    while (Remaining > 0)
    {
        int32 MaxIndex = 0;
        float MaxRemainder = -1.0f;

        for (int32 i = 0; i < Remainders.Num(); ++i)
        {
            if (Remainders[i] > MaxRemainder)
            {
                MaxRemainder = Remainders[i];
                MaxIndex = i;
            }
        }

        OutCounts[MaxIndex]++;
        Remainders[MaxIndex] = 0.0f;  // чтобы не выбирать этот индекс повторно
        Remaining--;
    }
}


UActorComponent* UCppFunctionLibrary::SetCreationMethod(UActorComponent* ActorComponent, EComponentCreationMethod method)
{
    if (ActorComponent != nullptr) {
        ActorComponent->CreationMethod = method;
    }
    return ActorComponent;
}