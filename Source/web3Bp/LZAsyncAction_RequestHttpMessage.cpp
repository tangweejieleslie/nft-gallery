// Fill out your copyright notice in the Description page of Project Settings.

#include "LZAsyncAction_RequestHttpMessage.h"

void ULZAsyncAction_RequestHttpMessage::Activate()
{
	// build GetRequest String
	// TODO: use *FString::Printf(TEXT("%s"), *String) instead for cleaner look
	//FString AccountAddr = "4HEszrujgYY6PCNq9y9GqN9SYdeszAVsWeEi9YhVHis2"; //"fmnXTt5sUnpcAAWd6gzdrZQKpFan6WPmGZZqDaJHrZC";


	FString GetRequest;
	if (Chain == "Solana")
	{
		GetRequest = "https://api.all.art/v1/wallet/" + Address;
	}
	else if (Chain == "Ethereum")
	{
		FString getRequestChain = "ethereum";
		FString NumOfNftsToRead = "50";
		FString AccountAddr = Address;
		FString GetRequestPart1 = "https://api.nftport.xyz/v0/accounts/";
		FString GetRequestPart2 = "?chain=";
		FString GetRequestPart3 = "&account_address=";
		FString GetRequestPart4 = "&page_size=";
		FString GetRequestPart5 = "&continuation=None&include=metadata";
		GetRequest = GetRequestPart1 + AccountAddr + GetRequestPart2 + getRequestChain + GetRequestPart3 + AccountAddr + GetRequestPart4 + NumOfNftsToRead + GetRequestPart5;
	}
	else
	{
		GetRequest = "Error";
	}


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

		TArray<TSharedPtr<FJsonValue>> JsonNfts;
		if (Chain == "Solana")
		{
			JsonNfts = JsonObject->GetArrayField(TEXT("unlistedNfts"));
		}
		else if (Chain == "Ethereum")
		{
			JsonNfts = JsonObject->GetArrayField(TEXT("nfts"));
		}

		for (int i = 0; i < JsonNfts.Num(); i++)
		{
			TSharedPtr<FJsonValue> value = JsonNfts[i];
			TSharedPtr<FJsonObject> json = value->AsObject();

			FString name;
			FString fileUrl;

			if (Chain == "Solana")
			{
				name = json->GetStringField("Title");
				fileUrl = json->GetStringField("Preview_URL");
			}
			else if (Chain == "Ethereum")
			{
				name = json->GetStringField("name");
				fileUrl = json->GetStringField("file_url");
			}

			nftNames.Add(name);
			nftFileUrls.Add(fileUrl);
		}
	}

	Completed.Broadcast(nftNames, nftFileUrls, bSuccess);
}

ULZAsyncAction_RequestHttpMessage* ULZAsyncAction_RequestHttpMessage::AsyncRequestHTTP(UObject* WorldContextObject, FString URL, FString Chain, FString Address)
{
	// Create Action Instance for Blueprint System
	ULZAsyncAction_RequestHttpMessage* Action = NewObject<ULZAsyncAction_RequestHttpMessage>();
	Action->URL = URL;
	Action->Chain = Chain;
	Action->Address = Address;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}
