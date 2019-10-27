// Fill out your copyright notice in the Description page of Project Settings.

#include "AdCollection.h"
#include "RcvDebugMessage.h"

URcvDebugMessage::URcvDebugMessage() :
	RcvDebugMessageDelegate(FDebugMessageDelegate::CreateUObject(this, &ThisClass::OnRcvDebugMessageCallback))
{

}

URcvDebugMessage* URcvDebugMessage::RcvEasyAdsDebugMessage(EAdType AdType)
{
	URcvDebugMessage* Proxy = NewObject<URcvDebugMessage>();
	Proxy->AdsType = AdType;

	return Proxy;
}

IAdModuleInterface* URcvDebugMessage::FindAdsModule()
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAdType"), true);
	if (!EnumPtr)
	{
		return nullptr;
	}

	FString EnumName = EnumPtr->GetNameByValue((int64)AdsType).ToString();

	FName adPlatformName;
	int32 ScopeIndex = EnumName.Find(TEXT("::"), ESearchCase::CaseSensitive);
	if (ScopeIndex != INDEX_NONE)
	{
		adPlatformName = FName(*EnumName.Mid(ScopeIndex + 2));
	}
	else
	{
		adPlatformName = FName(*EnumName);
	}

	IAdModuleInterface * Module = FModuleManager::Get().LoadModulePtr<IAdModuleInterface>(adPlatformName);

	return Module;
}

void URcvDebugMessage::Activate()
{
	IAdModuleInterface* Module = FindAdsModule();
	if (Module == nullptr)
	{
		return;
	}
	
	RcvDebugMessageDelegateHandle = Module->AddDebugMessageDelegate_Handle(RcvDebugMessageDelegate);
}


void URcvDebugMessage::OnRcvDebugMessageCallback(FString debugMessage)
{
	IAdModuleInterface* Module = FindAdsModule();
	if (Module == nullptr)
	{
		return;
	}

	OnDebugMessage.Broadcast(debugMessage);
}