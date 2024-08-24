// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Http.h"
#include "HttpRequester.generated.h"


/**
 * 
 */
UCLASS(Blueprintable)
class ETERNALTROOPS_API UHttpRequester : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Http request")
	static FString GetRequest(const FString& url, const FString& uaid, const FString& sign);

	UFUNCTION(BlueprintCallable, Category = "Http request")
	FString TestGet();

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
};

