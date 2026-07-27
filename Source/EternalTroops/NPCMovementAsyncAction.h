#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "NPCMovementAsyncAction.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNPCGoToTileSimpleDelegate);



UCLASS()
class ETERNALTROOPS_API UNPCMovementAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:

	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate TargetReached;


	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate AcceptanceRadiusReached;


	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate WalkLengthRadiusReached;


	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate Canceled;



	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DefaultToSelf = "Controller"))
	static UNPCMovementAsyncAction* GoToTile(
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


	UFUNCTION()
	void HandleCanceled();
};


/** Async Blueprint node that starts the NPC_Movement "Follow Character" task. */
UCLASS()
class ETERNALTROOPS_API UNPCFollowCharacterAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate AcceptanceRadiusReached;

	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate WalkLengthRadiusReached;


	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate Refollowed;

	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate Canceled;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DefaultToSelf = "Controller"))
	static UNPCFollowCharacterAsyncAction* FollowCharacter(
		AAIController* Controller,
		ACharacter* TargetCharacter,
		float AcceptanceRadius,
		float WalkLengthRadius,
		bool DynamicRepath,
		bool FollowAfterReach,
		float RefollowTimer
	);

	virtual void Activate() override;

private:
	UPROPERTY()
	AAIController* NPCController;

	UPROPERTY()
	ACharacter* Character;

	float Acceptance = 0.f;
	float WalkLength = 0.f;
	bool bDynamicRepath = false;
	bool bFollowAfterReach = false;
	float RefollowInterval = 0.f;

	UObject* MovementComponent = nullptr;

	void ExecuteFollowCharacter();

	UFUNCTION()
	void HandleAcceptanceRadiusReached();

	UFUNCTION()
	void HandleWalkLengthRadiusReached();

	UFUNCTION()
	void HandleRefollowed();

	UFUNCTION()
	void HandleCanceled();
};


/** Async Blueprint node that starts the NPC_Movement "Start Static Path" task. */
UCLASS()
class ETERNALTROOPS_API UNPCStartStaticPathAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate Finished;

	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate PointReached;

	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate OnLooped;

	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate Canceled;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DefaultToSelf = "Controller"))
	static UNPCStartStaticPathAsyncAction* StartStaticPath(
		AAIController* Controller,
		const TArray<AActor*>& Points,
		bool Looped,
		int32 Iterator
	);

	virtual void Activate() override;

private:
	UPROPERTY()
	AAIController* NPCController;

	UPROPERTY()
	TArray<AActor*> PathPoints;

	bool bLooped = false;
	int32 StartIterator = 0;
	UObject* MovementComponent = nullptr;

	void ExecuteStartStaticPath();

	UFUNCTION()
	void HandleFinished();

	UFUNCTION()
	void HandlePointReached();

	UFUNCTION()
	void HandleOnLooped();

	UFUNCTION()
	void HandleCanceled();
};


/** Async Blueprint node that starts the NPC_Movement "Start Ai Path" task. */
UCLASS()
class ETERNALTROOPS_API UNPCStartAIPathAsyncAction : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate Finished;

	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate PointReached;

	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate OnLooped;

	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate RouteSwitched;

	UPROPERTY(BlueprintAssignable)
	FNPCGoToTileSimpleDelegate Canceled;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", DefaultToSelf = "Controller"))
	static UNPCStartAIPathAsyncAction* StartAIPath(
		AAIController* Controller,
		const TArray<AActor*>& Points,
		bool Looped,
		int32 Iterator,
		AActor* End,
		bool DynamicRepath
	);

	virtual void Activate() override;

private:
	UPROPERTY()
	AAIController* NPCController;

	UPROPERTY()
	TArray<AActor*> PathPoints;

	UPROPERTY()
	AActor* EndActor;

	bool bLooped = false;
	int32 StartIterator = 0;
	bool bDynamicRepath = false;
	UObject* MovementComponent = nullptr;

	void ExecuteStartAIPath();

	UFUNCTION()
	void HandleFinished();

	UFUNCTION()
	void HandlePointReached();

	UFUNCTION()
	void HandleOnLooped();

	UFUNCTION()
	void HandleRouteSwitched();

	UFUNCTION()
	void HandleCanceled();
};
