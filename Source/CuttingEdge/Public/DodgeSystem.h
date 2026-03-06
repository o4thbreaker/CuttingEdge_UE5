#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "PlayMontageController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ActorComponent.h"
#include "DodgeSystem.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUTTINGEDGE_API UDodgeSystem : public UActorComponent
{
	GENERATED_BODY()

/* VARIABLES */
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeDuration = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float MaxDodgeDistance = 0.3f;

	UPROPERTY(BlueprintReadWrite, Category = "Dodge")
	FVector DodgeStartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	UAnimMontage* DodgeMontage;

private:
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	APlayMontageController* MontageController;

	UPROPERTY()
	FTimerHandle DodgeTimeline;

/* FUNCTIONS */
public:	
	// Sets default values for this component's properties
	UDodgeSystem();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	void DodgeBegin();

	UFUNCTION(Category = "Dodge")
	void DodgeOverTime(FVector Location);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Dodge")
	FVector GetFurthestDodgeLocation(FVector Start, FVector End);

	UFUNCTION(BlueprintCallable, Category = "Dodge")
	bool IsValidDodgeLocation(FVector Location, TArray<FHitResult> Hits);

	UFUNCTION(BlueprintPure, Category = "Dodge")
	FVector CalculateDodgeTargetLocation();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
