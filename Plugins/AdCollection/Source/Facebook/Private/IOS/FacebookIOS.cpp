/*
* EZ-Mobile-Ads - unreal engine 4 ads plugin
*
* Copyright (C) 2017 feiwu <feixuwu@outlook.com> All Rights Reserved.
*/

#include "Facebook.h"
#include "IOSAppDelegate.h"
#import <AdsUtil/AdsUtil.h>
#include "Async/TaskGraphInterfaces.h"

void FFacebookModule::ShowBanner(enAdsBannerPos pos)
{
	UIViewController* curViewController = (UIViewController*)[IOSAppDelegate GetDelegate].IOSController;

	dispatch_async(dispatch_get_main_queue(), ^{
		[[FacebookHelper GetDelegate] ShowBanner:curViewController isBottom : pos == enAdsBannerPos::enAdsBannerPos_Bottom];
		});
}

void FFacebookModule::HideBanner()
{
	dispatch_async(dispatch_get_main_queue(), ^{
		[[FacebookHelper GetDelegate] HideBanner];
		});
}

void FFacebookModule::ShowInterstitialAd()
{
	UIViewController* curViewController = (UIViewController*)[IOSAppDelegate GetDelegate].IOSController;

	dispatch_async(dispatch_get_main_queue(), ^{
		[[FacebookHelper GetDelegate] ShowInterstitialAds:curViewController];
		});
}

void FFacebookModule::PlayRewardedVideo()
{
	UIViewController* curViewController = (UIViewController*)[IOSAppDelegate GetDelegate].IOSController;

	dispatch_async(dispatch_get_main_queue(), ^{
		[[FacebookHelper GetDelegate] Play:curViewController];
		});
}

bool FFacebookModule::IsBannerReady()
{
	NSMutableDictionary *resultDictionary = [NSMutableDictionary dictionaryWithCapacity : 1];
	[[FacebookHelper GetDelegate] performSelectorOnMainThread:@selector(IsBannerReady : ) withObject:resultDictionary waitUntilDone : YES];

	NSNumber* number = (NSNumber*)[resultDictionary objectForKey : @"RetValue"];
	bool ret = (bool)[number boolValue];

	return ret;
}

bool FFacebookModule::IsInterstitalReady()
{
	NSMutableDictionary *resultDictionary = [NSMutableDictionary dictionaryWithCapacity : 1];
	[[FacebookHelper GetDelegate] performSelectorOnMainThread:@selector(IsInterstitalReady : ) withObject:resultDictionary waitUntilDone : YES];

	NSNumber* number = (NSNumber*)[resultDictionary objectForKey : @"RetValue"];
	bool ret = (bool)[number boolValue];

	return ret;
}

bool FFacebookModule::IsRewardedVideoReady()
{
	NSMutableDictionary *resultDictionary = [NSMutableDictionary dictionaryWithCapacity : 1];
	[[FacebookHelper GetDelegate] performSelectorOnMainThread:@selector(IsPlayable : ) withObject:resultDictionary waitUntilDone : YES];

	NSNumber* number = (NSNumber*)[resultDictionary objectForKey : @"RetValue"];
	bool ret = (bool)[number boolValue];

	return ret;
}

static void IOS_FacebookPlayComplete()
{
	DECLARE_CYCLE_STAT(TEXT("FSimpleDelegateGraphTask.nativePlayRewardedComplete"), STAT_FSimpleDelegateGraphTask_nativePlayRewardedComplete, STATGROUP_TaskGraphTasks);
	FSimpleDelegateGraphTask::CreateAndDispatchWhenReady(
		FSimpleDelegateGraphTask::FDelegate::CreateLambda([=]()
	{
		FPlatformMisc::LowLevelOutputDebugStringf(TEXT("nativbe playrewarded completed\n"));
		FFacebookModule* pModule = FModuleManager::Get().LoadModulePtr<FFacebookModule>(TEXT("Facebook"));
		if (pModule == nullptr)
		{
			return;
		}

		FRewardedStatus status;
		status.AdType = EAdType::Facebook;


		pModule->TriggerPlayRewardCompleteDelegates(status);
	}),
		GET_STATID(STAT_FSimpleDelegateGraphTask_nativePlayRewardedComplete),
		nullptr,
		ENamedThreads::GameThread
		);
}

void FFacebookModule::StartupModule()
{
	bool isEnable = false;

	GConfig->GetBool(TEXT("/Script/AdCollectionEditor.FacebookSetting"), TEXT("bIOSEnabled"), isEnable, GEngineIni);
	if (isEnable)
	{
		FString bannerUnit;
		GConfig->GetString(TEXT("/Script/AdCollectionEditor.FacebookSetting"), TEXT("IOSBannerUnit"), bannerUnit, GEngineIni);

		FString InterstitalUnit;
		GConfig->GetString(TEXT("/Script/AdCollectionEditor.FacebookSetting"), TEXT("IOSInterstitialUnit"), InterstitalUnit, GEngineIni);

		FString rewardedUnit;
		GConfig->GetString(TEXT("/Script/AdCollectionEditor.FacebookSetting"), TEXT("IOSRewardedVideoAdUnit"), rewardedUnit, GEngineIni);


		UIViewController* curViewController = (UIViewController*)[IOSAppDelegate GetDelegate].IOSController;

		dispatch_async(dispatch_get_main_queue(), ^{
			[[FacebookHelper GetDelegate] InitSDK:curViewController BannerUnit : [NSString stringWithFString : bannerUnit]  InterstitalUnit : [NSString stringWithFString : InterstitalUnit] RewardedUnit : [NSString stringWithFString : rewardedUnit] callback : &IOS_FacebookPlayComplete];
			});
	}

    
}



void FFacebookModule::ShutdownModule()
{
	
}
