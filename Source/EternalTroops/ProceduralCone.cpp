#include "ProceduralCone.h"

AProceduralCone::AProceduralCone()
{
    PrimaryActorTick.bCanEverTick = true;

    ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    RootComponent = ProcMesh;

    ProcMesh->bUseComplexAsSimpleCollision = true;
    ProcMesh->SetCastShadow(true);
}

void AProceduralCone::BeginPlay()
{
    Super::BeginPlay();
    UpdateMesh();
}

void AProceduralCone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AProceduralCone::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    UpdateMesh();
}

void AProceduralCone::UpdateMesh()
{
    if (!ProcMesh)
        return;

    ProcMesh->ClearAllMeshSections();
    GenerateSectorMesh(Radius, AngleDegrees, Height, Segments);

    if (SectorMaterial)
    {
        ProcMesh->SetMaterial(0, SectorMaterial);
    }
}

void AProceduralCone::GenerateSectorMesh(float InRadius, float InAngleDegrees, float InHeight, int32 InSegments)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UV0;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> Colors;

    float InnerRadius = FMath::Clamp(CutoffRadius, 0.f, InRadius);

    float HalfAngleRad = FMath::DegreesToRadians(InAngleDegrees * 0.5f);
    float Step = (2 * HalfAngleRad) / InSegments;

    // Создаем вершины: 4 кольца вершин
    // Нижний внешний круг
    TArray<int32> BottomOuterIndices;
    // Нижний внутренний круг
    TArray<int32> BottomInnerIndices;
    // Верхний внешний круг
    TArray<int32> TopOuterIndices;
    // Верхний внутренний круг
    TArray<int32> TopInnerIndices;

    // Добавляем вершины
    float HalfHeight = InHeight * 0.5f;

    for (int32 i = 0; i <= InSegments; ++i)
    {
        float Angle = -HalfAngleRad + i * Step;
        float CosA = FMath::Cos(Angle);
        float SinA = FMath::Sin(Angle);

        // Нижний внешний
        BottomOuterIndices.Add(Vertices.Add(FVector(CosA * InRadius, SinA * InRadius, -HalfHeight)));

        // Нижний внутренний (если CutoffRadius > 0, иначе центр)
        if (InnerRadius > 0.f)
            BottomInnerIndices.Add(Vertices.Add(FVector(CosA * InnerRadius, SinA * InnerRadius, -HalfHeight)));
        else if (i == 0)
            BottomInnerIndices.Add(Vertices.Add(FVector(0.f, 0.f, -HalfHeight))); // центр

        // Верхний внешний
        TopOuterIndices.Add(Vertices.Add(FVector(CosA * InRadius, SinA * InRadius, +HalfHeight)));

        // Верхний внутренний (если CutoffRadius > 0, иначе центр)
        if (InnerRadius > 0.f)
            TopInnerIndices.Add(Vertices.Add(FVector(CosA * InnerRadius, SinA * InnerRadius, +HalfHeight)));
        else if (i == 0)
            TopInnerIndices.Add(Vertices.Add(FVector(0.f, 0.f, +HalfHeight))); // центр
    }


    // Создаем треугольники
    // Нижняя плоскость (крышка)
    if (InnerRadius > 0.f)
    {
        // Нижняя крышка - кольцо
        for (int32 i = 0; i < InSegments; ++i)
        {
            Triangles.Add(BottomInnerIndices[i]);
            Triangles.Add(BottomOuterIndices[i + 1]);
            Triangles.Add(BottomOuterIndices[i]);

            Triangles.Add(BottomInnerIndices[i]);
            Triangles.Add(BottomInnerIndices[i + 1]);
            Triangles.Add(BottomOuterIndices[i + 1]);
        }
    }
    else
    {
        // Нижняя крышка - полный круг с центром
        int32 CenterBottomIndex = BottomInnerIndices[0];
        for (int32 i = 0; i < InSegments; ++i)
        {
            Triangles.Add(CenterBottomIndex);
            Triangles.Add(BottomOuterIndices[i]);
            Triangles.Add(BottomOuterIndices[i + 1]);
        }
    }

    // Верхняя плоскость (крышка)
    if (InnerRadius > 0.f)
    {
        // Верхняя крышка - кольцо
        for (int32 i = 0; i < InSegments; ++i)
        {
            Triangles.Add(TopInnerIndices[i]);
            Triangles.Add(TopOuterIndices[i]);
            Triangles.Add(TopOuterIndices[i + 1]);

            Triangles.Add(TopInnerIndices[i]);
            Triangles.Add(TopOuterIndices[i + 1]);
            Triangles.Add(TopInnerIndices[i + 1]);
        }
    }
    else
    {
        // Верхняя крышка - полный круг с центром
        int32 CenterTopIndex = TopInnerIndices[0];
        for (int32 i = 0; i < InSegments; ++i)
        {
            Triangles.Add(CenterTopIndex);
            Triangles.Add(TopOuterIndices[i + 1]);
            Triangles.Add(TopOuterIndices[i]);
        }
    }

    // Боковые стены
    // Внешняя стена
    for (int32 i = 0; i < InSegments; ++i)
    {
        int32 i0 = BottomOuterIndices[i];
        int32 i1 = BottomOuterIndices[i + 1];
        int32 i2 = TopOuterIndices[i];
        int32 i3 = TopOuterIndices[i + 1];

        Triangles.Add(i0);
        Triangles.Add(i3);
        Triangles.Add(i2);

        Triangles.Add(i0);
        Triangles.Add(i1);
        Triangles.Add(i3);
    }

    // Внутренняя стена (если есть)
    if (InnerRadius > 0.f)
    {
        for (int32 i = 0; i < InSegments; ++i)
        {
            int32 i0 = BottomInnerIndices[i];
            int32 i1 = BottomInnerIndices[i + 1];
            int32 i2 = TopInnerIndices[i];
            int32 i3 = TopInnerIndices[i + 1];

            Triangles.Add(i0);
            Triangles.Add(i2);
            Triangles.Add(i3);

            Triangles.Add(i0);
            Triangles.Add(i3);
            Triangles.Add(i1);
        }
    }

    // Левая боковая стенка (срез сектора)
    {
        int32 i0 = BottomOuterIndices[0];
        int32 i1 = TopOuterIndices[0];
        int32 i2 = InnerRadius > 0.f ? TopInnerIndices[0] : TopInnerIndices[0]; // если нет выреза - это центр
        int32 i3 = InnerRadius > 0.f ? BottomInnerIndices[0] : BottomInnerIndices[0];

        Triangles.Add(i0);
        Triangles.Add(i2);
        Triangles.Add(i1);

        Triangles.Add(i0);
        Triangles.Add(i3);
        Triangles.Add(i2);
    }

    // Правая боковая стенка (срез сектора)
    {
        int32 i0 = BottomOuterIndices.Last();
        int32 i1 = TopOuterIndices.Last();
        int32 i2 = InnerRadius > 0.f ? TopInnerIndices.Last() : TopInnerIndices.Last();
        int32 i3 = InnerRadius > 0.f ? BottomInnerIndices.Last() : BottomInnerIndices.Last();

        Triangles.Add(i0);
        Triangles.Add(i1);
        Triangles.Add(i2);

        Triangles.Add(i0);
        Triangles.Add(i2);
        Triangles.Add(i3);
    }

    // Нормали
    Normals.SetNum(Vertices.Num());
    for (int32 i = 0; i < Vertices.Num(); ++i)
    {
        FVector pos = Vertices[i];
        if (FMath::Abs(pos.Z) > KINDA_SMALL_NUMBER)
        {
            FVector horizontal = FVector(pos.X, pos.Y, 0.f);
            Normals[i] = horizontal.GetSafeNormal();
        }
        else
        {
            Normals[i] = FVector(0, 0, 1);
        }
    }

    // UV
    UV0.Init(FVector2D(0, 0), Vertices.Num());
    for (int32 i = 0; i < Vertices.Num(); ++i)
    {
        UV0[i] = FVector2D(Vertices[i].X / InRadius / 2.f + 0.5f, Vertices[i].Y / InRadius / 2.f + 0.5f);
    }

    Tangents.Init(FProcMeshTangent(1, 0, 0), Vertices.Num());
    Colors.Init(FLinearColor::White, Vertices.Num());

    ProcMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UV0, Colors, Tangents, true);
}
