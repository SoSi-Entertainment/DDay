#include "NPCMovementAsyncAction.h"

#include "Components/ActorComponent.h"



UNPCMovementAsyncAction* UNPCMovementAsyncAction::GoToTile(
	AAIController* Controller,
	AActor* TargetTile,
	float AcceptanceRadius,
	float WalkLengthRadius,
	bool DynamicRepath
)
{
	UNPCMovementAsyncAction* Node =
		NewObject<UNPCMovementAsyncAction>();


	Node->NPCController = Controller;

	Node->Tile = TargetTile;

	Node->Acceptance = AcceptanceRadius;

	Node->WalkLength = WalkLengthRadius;

	Node->bDynamicRepath = DynamicRepath;


	return Node;
}




void UNPCMovementAsyncAction::Activate()
{
	ExecuteGoToTile();
}




void UNPCMovementAsyncAction::ExecuteGoToTile()
{
	if (!NPCController)
	{
		return;
	}



	UClass* MovementClass = LoadObject<UClass>(
		nullptr,
		TEXT("/Game/Blueprints/Navigation/NPC_Movement/NPC_Movement.NPC_Movement_C")
	);



	if (!MovementClass)
	{
		return;
	}



	MovementComponent =
		NPCController->GetComponentByClass(MovementClass);



	if (!MovementComponent)
	{
		return;
	}



	UFunction* Function =
		MovementComponent->FindFunction(TEXT("Go To Tile"));



	if (!Function)
	{
		return;
	}



	uint8* Params =
		(uint8*)FMemory_Alloca(Function->ParmsSize);



	FMemory::Memzero(
		Params,
		Function->ParmsSize
	);



	for (TFieldIterator<FProperty> It(Function); It; ++It)
	{
		FProperty* Property = *It;


		FString Name =
			Property->GetName();



		uint8* Value =
			Params + Property->GetOffset_ForInternal();



		if (Name == TEXT("Target Tile"))
		{
			if (FObjectProperty* Obj =
				CastField<FObjectProperty>(Property))
			{
				Obj->SetObjectPropertyValue(
					Value,
					Tile
				);
			}
		}



		else if (Name == TEXT("Acceptance Radius"))
		{
			if (FDoubleProperty* Double =
				CastField<FDoubleProperty>(Property))
			{
				Double->SetPropertyValue(
					Value,
					Acceptance
				);
			}

			else if (FFloatProperty* Float =
				CastField<FFloatProperty>(Property))
			{
				Float->SetPropertyValue(
					Value,
					Acceptance
				);
			}
		}



		else if (Name == TEXT("Walk lengh Radius"))
		{
			if (FDoubleProperty* Double =
				CastField<FDoubleProperty>(Property))
			{
				Double->SetPropertyValue(
					Value,
					WalkLength
				);
			}

			else if (FFloatProperty* Float =
				CastField<FFloatProperty>(Property))
			{
				Float->SetPropertyValue(
					Value,
					WalkLength
				);
			}
		}



		else if (Name == TEXT("Dynamic repath"))
		{
			if (FBoolProperty* Bool =
				CastField<FBoolProperty>(Property))
			{
				Bool->SetPropertyValue(
					Value,
					bDynamicRepath
				);
			}
		}



		else if (Name == TEXT("Target Reached"))
		{
			FScriptDelegate Delegate;

			Delegate.BindUFunction(
				this,
				GET_FUNCTION_NAME_CHECKED(
					UNPCMovementAsyncAction,
					HandleTargetReached
				)
			);


			if (FDelegateProperty* DP =
				CastField<FDelegateProperty>(Property))
			{
				DP->SetPropertyValue(
					Value,
					Delegate
				);
			}
		}



		else if (Name == TEXT("Acceptance Radius Reached"))
		{
			FScriptDelegate Delegate;

			Delegate.BindUFunction(
				this,
				GET_FUNCTION_NAME_CHECKED(
					UNPCMovementAsyncAction,
					HandleAcceptanceRadiusReached
				)
			);


			if (FDelegateProperty* DP =
				CastField<FDelegateProperty>(Property))
			{
				DP->SetPropertyValue(
					Value,
					Delegate
				);
			}
		}



		else if (Name == TEXT("Walk length Radius Reached"))
		{
			FScriptDelegate Delegate;

			Delegate.BindUFunction(
				this,
				GET_FUNCTION_NAME_CHECKED(
					UNPCMovementAsyncAction,
					HandleWalkLengthRadiusReached
				)
			);


			if (FDelegateProperty* DP =
				CastField<FDelegateProperty>(Property))
			{
				DP->SetPropertyValue(
					Value,
					Delegate
				);
			}
		}
	}



	MovementComponent->ProcessEvent(
		Function,
		Params
	);
}




void UNPCMovementAsyncAction::HandleTargetReached()
{
	TargetReached.Broadcast();

	SetReadyToDestroy();
}




void UNPCMovementAsyncAction::HandleAcceptanceRadiusReached()
{
	AcceptanceRadiusReached.Broadcast();
}




void UNPCMovementAsyncAction::HandleWalkLengthRadiusReached()
{
	WalkLengthRadiusReached.Broadcast();
}