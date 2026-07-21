#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "NPCGoToTileAsyncAction.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNPCGoToTileSimpleDelegate);



UCLASS()
class ETERNALTROOPS_API UNPCGoToTileAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:

	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate TargetReached;


	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate AcceptanceRadiusReached;


	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate WalkLengthRadiusReached;



	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DefaultToSelf = "Controller"))
	static UNPCGoToTileAsyncAction* GoToTile(
		AAIController* Controller,
		AActor* TargetTile,
		float AcceptanceRadius,
		float WalkLengthRadius,
		bool DynamicRepath
	);



	virtual void Activate() override;



private:

	UPROPERTY()
	AAIController* NPCController;


	UPROPERTY()
	AActor* Tile;


	float Acceptance = 0.f;

	float WalkLength = 0.f;

	bool bDynamicRepath = false;



	UObject* MovementComponent = nullptr;



	void ExecuteGoToTile();



	UFUNCTION()
	void HandleTargetReached();


	UFUNCTION()
	void HandleAcceptanceRadiusReached();


	UFUNCTION()
	void HandleWalkLengthRadiusReached();
};