/*
* EZ-Mobile-Ads - unreal engine 4 ads plugin
*
* Copyright (C) 2017 feiwu <feixuwu@outlook.com> All Rights Reserved.
*/

#pragma once

#include "UObject/NoExportTypes.h"
#include "FacebookSetting.generated.h"

/**
 * 
 */
UCLASS(transient, config = Engine)
class UFacebookSetting : public UObject
{
	GENERATED_BODY()
	
	// if the android facebook enable
	UPROPERTY(Config, EditAnywhere, Category = Android, Meta = (DisplayName = "If Facebook Android Enable"))
	bool bAndroidEnabled;

	// the banner adunit
	UPROPERTY(Config, EditAnywhere, Category = Android, Meta = (DisplayName = "Android Banner AdUnit"))
	FString AndroidBannerUnit;

	// the interstitial adunit
	UPROPERTY(Config, EditAnywhere, Category = Android, Meta = (DisplayName = "Android Interstitial AdUnit"))
	FString AndroidInterstitialUnit;

	// the rewardedvideo adunit
	UPROPERTY(Config, EditAnywhere, Category = Android, Meta = (DisplayName = "Android RewardVideo AdUnit"))
	FString AndroidRewardedVideoAdUnit;
    
	
#if WITH_EDITOR
	
	// UObject interface
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
};
