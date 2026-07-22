#include "NPCMovementAsyncAction.h"

#include "Components/ActorComponent.h"
#include "UObject/UnrealType.h"



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




UNPCFollowCharacterAsyncAction* UNPCFollowCharacterAsyncAction::FollowCharacter(
	AAIController* Controller,
	ACharacter* TargetCharacter,
	float AcceptanceRadius,
	float WalkLengthRadius,
	bool DynamicRepath,
	bool FollowAfterReach,
	float RefollowTimer
)
{
	UNPCFollowCharacterAsyncAction* Node = NewObject<UNPCFollowCharacterAsyncAction>();

	Node->NPCController = Controller;
	Node->Character = TargetCharacter;
	Node->Acceptance = AcceptanceRadius;
	Node->WalkLength = WalkLengthRadius;
	Node->bDynamicRepath = DynamicRepath;
	Node->bFollowAfterReach = FollowAfterReach;
	Node->RefollowInterval = RefollowTimer;

	return Node;
}




void UNPCFollowCharacterAsyncAction::Activate()
{
	ExecuteFollowCharacter();
}




void UNPCFollowCharacterAsyncAction::ExecuteFollowCharacter()
{
	if (!NPCController || !Character)
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

	MovementComponent = NPCController->GetComponentByClass(MovementClass);

	if (!MovementComponent)
	{
		return;
	}

	UFunction* Function = MovementComponent->FindFunction(TEXT("Follow Character"));

	if (!Function)
	{
		return;
	}

	uint8* Params = (uint8*)FMemory_Alloca(Function->ParmsSize);
	FMemory::Memzero(Params, Function->ParmsSize);

	for (TFieldIterator<FProperty> It(Function); It; ++It)
	{
		FProperty* Property = *It;
		const FString Name = Property->GetName();
		uint8* Value = Params + Property->GetOffset_ForInternal();

		if (Name == TEXT("Target Character"))
		{
			if (FObjectProperty* ObjectProperty = CastField<FObjectProperty>(Property))
			{
				ObjectProperty->SetObjectPropertyValue(Value, Character);
			}
		}
		else if (Name == TEXT("Acceptance Radius"))
		{
			if (FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(Property))
			{
				DoubleProperty->SetPropertyValue(Value, Acceptance);
			}
			else if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
			{
				FloatProperty->SetPropertyValue(Value, Acceptance);
			}
		}
		else if (Name == TEXT("Walk lengh Radius"))
		{
			if (FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(Property))
			{
				DoubleProperty->SetPropertyValue(Value, WalkLength);
			}
			else if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
			{
				FloatProperty->SetPropertyValue(Value, WalkLength);
			}
		}
		else if (Name == TEXT("Dynamic repath"))
		{
			if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
			{
				BoolProperty->SetPropertyValue(Value, bDynamicRepath);
			}
		}
		else if (Name == TEXT("Follow after reach"))
		{
			if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
			{
				BoolProperty->SetPropertyValue(Value, bFollowAfterReach);
			}
		}
		else if (Name == TEXT("Refollow timer"))
		{
			if (FDoubleProperty* DoubleProperty = CastField<FDoubleProperty>(Property))
			{
				DoubleProperty->SetPropertyValue(Value, RefollowInterval);
			}
			else if (FFloatProperty* FloatProperty = CastField<FFloatProperty>(Property))
			{
				FloatProperty->SetPropertyValue(Value, RefollowInterval);
			}
		}
		else if (Name == TEXT("Acceptance Radius Reached"))
		{
			FScriptDelegate Delegate;
			Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UNPCFollowCharacterAsyncAction, HandleAcceptanceRadiusReached));
			if (FDelegateProperty* DelegateProperty = CastField<FDelegateProperty>(Property))
			{
				DelegateProperty->SetPropertyValue(Value, Delegate);
			}
		}
		else if (Name == TEXT("Walk length Radius Reached"))
		{
			FScriptDelegate Delegate;
			Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UNPCFollowCharacterAsyncAction, HandleWalkLengthRadiusReached));
			if (FDelegateProperty* DelegateProperty = CastField<FDelegateProperty>(Property))
			{
				DelegateProperty->SetPropertyValue(Value, Delegate);
			}
		}
		else if (Name == TEXT("Refollowed"))
		{
			FScriptDelegate Delegate;
			Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UNPCFollowCharacterAsyncAction, HandleRefollowed));
			if (FDelegateProperty* DelegateProperty = CastField<FDelegateProperty>(Property))
			{
				DelegateProperty->SetPropertyValue(Value, Delegate);
			}
		}
	}

	MovementComponent->ProcessEvent(Function, Params);
}




void UNPCFollowCharacterAsyncAction::HandleAcceptanceRadiusReached()
{
	AcceptanceRadiusReached.Broadcast();
}




void UNPCFollowCharacterAsyncAction::HandleWalkLengthRadiusReached()
{
	WalkLengthRadiusReached.Broadcast();
}




void UNPCFollowCharacterAsyncAction::HandleRefollowed()
{
	Refollowed.Broadcast();
}




UNPCStartStaticPathAsyncAction* UNPCStartStaticPathAsyncAction::StartStaticPath(
	AAIController* Controller,
	const TArray<AActor*>& Points,
	bool Looped,
	int32 Iterator
)
{
	UNPCStartStaticPathAsyncAction* Node = NewObject<UNPCStartStaticPathAsyncAction>();

	Node->NPCController = Controller;
	Node->PathPoints.Reserve(Points.Num());
	for (AActor* Point : Points)
	{
		Node->PathPoints.Add(Point);
	}
	Node->bLooped = Looped;
	Node->StartIterator = Iterator;

	return Node;
}




void UNPCStartStaticPathAsyncAction::Activate()
{
	ExecuteStartStaticPath();
}




void UNPCStartStaticPathAsyncAction::ExecuteStartStaticPath()
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

	MovementComponent = NPCController->GetComponentByClass(MovementClass);
	if (!MovementComponent)
	{
		return;
	}

	UFunction* Function = MovementComponent->FindFunction(TEXT("Start Static Path"));
	if (!Function)
	{
		return;
	}

	uint8* Params = (uint8*)FMemory_Alloca(Function->ParmsSize);
	Function->InitializeStruct(Params);

	for (TFieldIterator<FProperty> It(Function); It; ++It)
	{
		FProperty* Property = *It;
		const FString Name = Property->GetName();
		uint8* Value = Params + Property->GetOffset_ForInternal();

		if (Name == TEXT("Points"))
		{
			if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
			{
				if (FObjectProperty* InnerObjectProperty = CastField<FObjectProperty>(ArrayProperty->Inner))
				{
					FScriptArrayHelper ArrayHelper(ArrayProperty, Value);
					ArrayHelper.EmptyValues();
					for (AActor* Point : PathPoints)
					{
						const int32 Index = ArrayHelper.AddValue();
						InnerObjectProperty->SetObjectPropertyValue(ArrayHelper.GetRawPtr(Index), Point);
					}
				}
			}
		}
		else if (Name == TEXT("Looped"))
		{
			if (FBoolProperty* BoolProperty = CastField<FBoolProperty>(Property))
			{
				BoolProperty->SetPropertyValue(Value, bLooped);
			}
		}
		else if (Name == TEXT("Iterator"))
		{
			if (FIntProperty* IntProperty = CastField<FIntProperty>(Property))
			{
				IntProperty->SetPropertyValue(Value, StartIterator);
			}
		}
		else if (Name == TEXT("Path finished"))
		{
			FScriptDelegate Delegate;
			Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UNPCStartStaticPathAsyncAction, HandleFinished));
			if (FDelegateProperty* DelegateProperty = CastField<FDelegateProperty>(Property))
			{
				DelegateProperty->SetPropertyValue(Value, Delegate);
			}
		}
		else if (Name == TEXT("Point reached"))
		{
			FScriptDelegate Delegate;
			Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UNPCStartStaticPathAsyncAction, HandlePointReached));
			if (FDelegateProperty* DelegateProperty = CastField<FDelegateProperty>(Property))
			{
				DelegateProperty->SetPropertyValue(Value, Delegate);
			}
		}
		else if (Name == TEXT("Looped event"))
		{
			FScriptDelegate Delegate;
			Delegate.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UNPCStartStaticPathAsyncAction, HandleOnLooped));
			if (FDelegateProperty* DelegateProperty = CastField<FDelegateProperty>(Property))
			{
				DelegateProperty->SetPropertyValue(Value, Delegate);
			}
		}
	}

	MovementComponent->ProcessEvent(Function, Params);
	Function->DestroyStruct(Params);
}




void UNPCStartStaticPathAsyncAction::HandleFinished()
{
	Finished.Broadcast();
	SetReadyToDestroy();
}




void UNPCStartStaticPathAsyncAction::HandlePointReached()
{
	PointReached.Broadcast();
}




void UNPCStartStaticPathAsyncAction::HandleOnLooped()
{
	OnLooped.Broadcast();
}
