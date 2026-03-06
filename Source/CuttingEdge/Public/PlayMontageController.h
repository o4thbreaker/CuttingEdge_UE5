// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "PlayMontageController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAnimMontagePlayDelegate, FName, NotifyName, UAnimMontage*, PlayedMontage);

UCLASS()
class CUTTINGEDGE_API APlayMontageController : public AActor
{
	GENERATED_BODY()

public:

	APlayMontageController();

	// Called when Montage finished playing and wasn't interrupted
	UPROPERTY(BlueprintAssignable)
	FOnAnimMontagePlayDelegate OnMontageCompleted;

	// Called when Montage starts blending out and is not interrupted
	UPROPERTY(BlueprintAssignable)
	FOnAnimMontagePlayDelegate OnMontageBlendOut;

	// Called when Montage has been interrupted (or failed to play)
	UPROPERTY(BlueprintAssignable)
	FOnAnimMontagePlayDelegate OnMontageInterrupted;


	UFUNCTION()
	void PlayMontage(class USkeletalMeshComponent* InSkeletalMeshComponent, class UAnimMontage* MontageToPlay, float PlayRate);

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY()
	TWeakObjectPtr<UAnimInstance> AnimInstancePtr;

	UPROPERTY()
	int32 MontageInstanceID;

	UPROPERTY()
	uint32 bInterruptedCalledBeforeBlendingOut : 1;

	FOnMontageEnded MontageEndedDelegate;

	FOnMontageBlendingOutStarted BlendingOutDelegate;
};
