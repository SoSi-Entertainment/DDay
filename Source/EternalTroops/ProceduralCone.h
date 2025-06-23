#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralCone.generated.h"

UCLASS()
class ETERNALTROOPS_API AProceduralCone : public AActor
{
    GENERATED_BODY()

public:
    AProceduralCone();

protected:
    virtual void BeginPlay() override;
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
    UProceduralMeshComponent* ProcMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
    float Radius = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh", meta = (ClampMin = "0", ClampMax = "10000"))
    float CutoffRadius = 0.f; // Радиус внутреннего выреза (0 = нет выреза)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
    float AngleDegrees = 90.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
    float Height = 100.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
    int32 Segments = 32;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
    UMaterialInterface* SectorMaterial = nullptr;

    UFUNCTION(BlueprintCallable, Category = "Procedural Mesh")
    void GenerateSectorMesh(float InRadius, float InAngleDegrees, float InHeight, int32 InSegments);

    UFUNCTION(BlueprintCallable, Category = "Procedural Mesh")
    void UpdateMesh();
};
