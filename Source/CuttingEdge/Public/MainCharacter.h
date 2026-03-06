// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayMontageController.h"
#include "PlayerCombatMode.h"
#include "CombatSystem.h"
#include "EnemyInterface.h"
#include "EnemyBase.h"
#include "DrawDebugHelpers.h"
#include "MainCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAimStartDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAimEndDelegate);

class AWorldSettings;
struct FPostProcessSettings;

UCLASS()
class CUTTINGEDGE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

/* VARIABLES */

public:

	/* COMPONENTS */

	UPROPERTY()
	APlayMontageController* MontageController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System", meta = (AllowPrivateAccess = "true"))
	UCombatSystem* CombatSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System", meta = (AllowPrivateAccess = "true"))
	UPlayerCombatMode* CombatMode;

	/* CAMERA-RELATED STUFF */

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Slowmo effect */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FPostProcessSettings SlowMotionEffect;

	/** Standart camera effect */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	FPostProcessSettings StandartCameraEffect;

	/* DELEGATES */

	/** On start aim during special move delegate **/
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAimStartDelegate OnAimStart;

	/** On end aim during special move delegate **/
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAimEndDelegate OnAimEnd;



	/** Actor that is being highlighted **/
	UPROPERTY(BlueprintType, BlueprintReadWrite, EditAnywhere, Category = "HighlightCharacter")
	AActor* SelectedActor = nullptr;

public:
	// Sets default values for this character's properties
	AMainCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "TimeManipulation")
	void StartSlowMotion(float globalTimeDilation = 0.2f);

	UFUNCTION(BlueprintCallable, Category = "TimeManipulation")
	void EndSlowMotion();

	UFUNCTION(BlueprintCallable, Category = "HighlightCharacter")
	void HighlightTarget();

	UFUNCTION(BlueprintCallable, Category = "HighlightCharacter")
	void DehighlightTarget();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Misc")
	void GetSelectedEnemyLocation(FVector& EnemyLocation, FRotator& EnemyRotation, float Offset = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "Misc")
	void RotatePlayerTowardsTarget(AActor* TargetActor, float InterpolationSpeed);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
