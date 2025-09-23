// NavBlockerBoxComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "NavAreas/NavArea_Null.h"
#include "NavBlockerBoxComponent.generated.h"

UCLASS(ClassGroup = "Custom", meta = (BlueprintSpawnableComponent))
class ETERNALTROOPS_API UNavBlockerBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UNavBlockerBoxComponent();
};
