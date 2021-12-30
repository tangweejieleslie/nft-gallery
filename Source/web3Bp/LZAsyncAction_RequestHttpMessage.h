// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LZAsyncAction_RequestHttpMessage.generated.h"

/*
USTRUCT(Blueprintable)
struct FNftStruct {
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite) TArray<FString> names;
	UPROPERTY() TArray<FString> fileUrls;
};*/

// Event that will be the 'Completed' exec wire in the blueprint node along with all parameters as output pins.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHttpRequestCompleted, const TArray<FString>&, nftNames, const TArray<FString>&, nftFileUrls, bool, bSuccess);

/**
 *
 */
UCLASS()
class WEB3BP_API ULZAsyncAction_RequestHttpMessage : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

protected:

	void HandleRequestCompleted(FString ResponseString, bool bSuccess);

public:

	/** Execute the actual load */
	virtual void Activate() override;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "HTTP", WorldContext = "WorldContextObject"))
		static ULZAsyncAction_RequestHttpMessage* AsyncRequestHTTP(UObject* WorldContextObject, FString URL, FString Chain, FString Address);

	UPROPERTY(BlueprintAssignable)
		FOnHttpRequestCompleted Completed;

	/* URL to send GET request to */
	FString URL;

	FString Chain;
	FString Address;
};
