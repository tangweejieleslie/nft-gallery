// Fill out your copyright notice in the Description page of Project Settings.

#include "LZAsyncAction_RequestHttpMessage.h"

void ULZAsyncAction_RequestHttpMessage::Activate()
{
	// build GetRequest String
	// TODO: use *FString::Printf(TEXT("%s"), *String) instead for cleaner look
	FString AccountAddr = "fmnXTt5sUnpcAAWd6gzdrZQKpFan6WPmGZZqDaJHrZC";
	FString GetRequestPart1 = "https://api.all.art/v1/wallet/";
	FString GetRequest = GetRequestPart1 + AccountAddr;

	// Create HTTP Request
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("GET");
	HttpRequest->SetHeader("Content-Type", "application/json");
	HttpRequest->SetHeader("Authorization", "9cf0b50c-3c2b-4e8d-9a28-19987e63d669");
	HttpRequest->SetURL(GetRequest);

	// Setup Async response
	HttpRequest->OnProcessRequestComplete().BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
		{
			FString ResponseString = "";
			if (bSuccess)
			{
				ResponseString = Response->GetContentAsString();
			}

			this->HandleRequestCompleted(ResponseString, bSuccess);
		});

	// Handle actual request
	HttpRequest->ProcessRequest();
}


void ULZAsyncAction_RequestHttpMessage::HandleRequestCompleted(FString ResponseString, bool bSuccess)
{
	//FString OutString;
	//FNftStruct nftStruct;
	TArray<FString> nftNames;
	TArray<FString> nftFileUrls;

	if (bSuccess)
	{
		/* Deserialize object */
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<>::Create(ResponseString);
		FJsonSerializer::Deserialize(JsonReader, JsonObject);

		TArray<TSharedPtr<FJsonValue>> JsonNfts = JsonObject->GetArrayField(TEXT("unlistedNfts"));

		for (int i = 0; i < JsonNfts.Num(); i++)
		{
			TSharedPtr<FJsonValue> value = JsonNfts[i];
			TSharedPtr<FJsonObject> json = value->AsObject();

			FString name = json->GetStringField("Title");
			FString fileUrl = json->GetStringField("jsonUrl");

			nftNames.Add(name);
			nftFileUrls.Add(fileUrl);
		}
	}

	Completed.Broadcast(nftNames, nftFileUrls, bSuccess);
}

ULZAsyncAction_RequestHttpMessage* ULZAsyncAction_RequestHttpMessage::AsyncRequestHTTP(UObject* WorldContextObject, FString URL)
{
	// Create Action Instance for Blueprint System
	ULZAsyncAction_RequestHttpMessage* Action = NewObject<ULZAsyncAction_RequestHttpMessage>();
	Action->URL = URL;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}
