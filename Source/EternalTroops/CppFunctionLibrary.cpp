#include "CppFunctionLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "ComponentInstanceDataCache.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/Navigation/AvoidanceManager.h"

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


void UCppFunctionLibrary::SetCameraMovableWhenPaused(UObject* WorldContextObject, bool bMovable)
{
    if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
    {
        World->bIsCameraMoveableWhenPaused = bMovable;
    }
}


void UCppFunctionLibrary::SetAvoidanceConsiderationRadius(ACharacter* Character, float Radius)
{
    if (Character == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetAvoidanceConsiderationRadius: Character is null"));
        return;
    }

    if (UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement())
    {
        MovementComponent->AvoidanceConsiderationRadius = Radius;
    }
}

void UCppFunctionLibrary::RemoveFromAvoidanceManager(ACharacter* Character)
{
    if (!Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("RemoveFromAvoidanceManager: Character is null"));
        return;
    }

    UCharacterMovementComponent* Movement = Character->GetCharacterMovement();
    if (!Movement)
    {
        return;
    }

    const int32 AvoidanceUID = Movement->GetRVOAvoidanceUID();

    UWorld* World = Character->GetWorld();
    if (!World)
    {
        return;
    }

    UAvoidanceManager* Avoidance = World->GetAvoidanceManager();

    UE_LOG(LogTemp, Warning,
        TEXT("RemoveFromAvoidanceManager: UID = %d, Manager = %s"),
        AvoidanceUID,
        *GetNameSafe(Avoidance));

    if (Avoidance && AvoidanceUID != 0)
    {
        Avoidance->RemoveAvoidanceObject(AvoidanceUID);
    }

    Movement->SetAvoidanceEnabled(false);
}
