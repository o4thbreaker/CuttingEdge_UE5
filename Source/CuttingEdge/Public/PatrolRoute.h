// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "PatrolRoute.generated.h"


UCLASS()
class CUTTINGEDGE_API APatrolRoute : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USplineComponent* SplineComponent;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int PatrolIndex;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (ClampMin = "-1", ClampMax = "1", UIMin = "-1", UIMax = "1"))
	int Direction;

public:
	// Sets default values for this actor's properties
	APatrolRoute();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void IncrementPatrolRoute();

	UFUNCTION(BlueprintCallable)
	FVector GetSplinePointAsWorldPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
