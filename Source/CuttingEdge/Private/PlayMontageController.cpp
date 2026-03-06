// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayMontageController.h"

// Sets default values for this component's properties
APlayMontageController::APlayMontageController()
{
}

void APlayMontageController::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		OnMontageCompleted.Broadcast(NAME_None, Montage);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, *(FString::Printf(TEXT("MONTAGE COMPLETED"))));
	}
	else if (!bInterruptedCalledBeforeBlendingOut)
	{
		OnMontageInterrupted.Broadcast(NAME_None, Montage);
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, *(FString::Printf(TEXT("MONTAGE INTERRUPT BEFORE BLEND OUT"))));
	}
}

void APlayMontageController::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		OnMontageInterrupted.Broadcast(NAME_None, Montage);
		bInterruptedCalledBeforeBlendingOut = true;
	}
	else
	{
		OnMontageBlendOut.Broadcast(NAME_None, Montage);
	}
}

void APlayMontageController::PlayMontage(class USkeletalMeshComponent* InSkeletalMeshComponent,
	class UAnimMontage* MontageToPlay,
	float PlayRate)
{
	bool bPlayedSuccessfully = false;
	if (InSkeletalMeshComponent)
	{
		if (UAnimInstance* AnimInstance = InSkeletalMeshComponent->GetAnimInstance())
		{
			const float MontageLength = AnimInstance->Montage_Play(MontageToPlay, PlayRate, EMontagePlayReturnType::MontageLength);
			bPlayedSuccessfully = (MontageLength > 0.f);

			if (bPlayedSuccessfully)
			{
				BlendingOutDelegate.BindUObject(this, &APlayMontageController::OnMontageBlendingOut);
				AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &APlayMontageController::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);
			}
		}
	}

	if (!bPlayedSuccessfully)
	{
		OnMontageInterrupted.Broadcast(NAME_None, MontageToPlay);
	}
}

