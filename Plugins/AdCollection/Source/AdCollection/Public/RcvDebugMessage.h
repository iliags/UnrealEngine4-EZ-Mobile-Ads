// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AdCollectionBPLibrary.h"
#include "RcvDebugMessage.generated.h"

/**
 * 
 */
UCLASS()
class ADCOLLECTION_API URcvDebugMessage : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	URcvDebugMessage();
	UPROPERTY(BlueprintAssignable)
		FDynamicDebugMessageDelegate OnDebugMessage;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RcvEasyAdsDebugMessage", BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject"), Category = "AdCollection")
		static URcvDebugMessage* RcvEasyAdsDebugMessage(EAdType AdType);

	virtual void Activate() override;

	IAdModuleInterface* FindAdsModule();

private:

	FDebugMessageDelegate RcvDebugMessageDelegate;
	FDelegateHandle RcvDebugMessageDelegateHandle;
	EAdType AdsType;

	void OnRcvDebugMessageCallback(FString debugMessage);
	
};
