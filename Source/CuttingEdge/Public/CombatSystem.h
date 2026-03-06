// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatSystem.generated.h"

class UCurveFloat;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotateToTargetActorDelegate, AActor*, TargetToRotate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUTTINGEDGE_API UCombatSystem : public UActorComponent
{
	GENERATED_BODY()

/* VARIABLES */

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AActor* TargetToRotate;

	UPROPERTY(EditAnywhere)
	UCurveFloat* CurveFloat;

private:
	UPROPERTY()
	UTimelineComponent* RotateTimeline;


/* FUNCTIONS */

public:	
	// Sets default values for this component's properties
	UCombatSystem();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void RotateTimelineUpdate(float Value);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage System")
	void RotateToTargetActor(AActor* ActorToRotateTo);
	void RotateToTargetActor_Implementation(AActor* ActorToRotateTo);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
