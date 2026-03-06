#include "CombatSystem.h"

// Sets default values for this component's properties
UCombatSystem::UCombatSystem()
{
	PrimaryComponentTick.bCanEverTick = true;

	RotateTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineComponent"));
}


// Called when the game starts
void UCombatSystem::BeginPlay()
{
	Super::BeginPlay();

	if (CurveFloat)
	{
		FOnTimelineFloat RotateTimelineProgress; // delegate to bind with UpdateTimeline
		RotateTimelineProgress.BindUFunction(this, FName("RotateTimelineUpdate"));

		RotateTimeline->AddInterpFloat(CurveFloat, RotateTimelineProgress);
		RotateTimeline->SetLooping(false);
		RotateTimeline->SetTimelineLength(0.5f);
	}
	
}

// Called every frame
void UCombatSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatSystem::RotateTimelineUpdate(float Value)
{
	FRotator TargetRotation = FRotator::ZeroRotator;
	TargetRotation.Yaw = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetToRotate->GetActorLocation()).Yaw;

	GetOwner()->SetActorRotation(FMath::RInterpTo(GetOwner()->GetActorRotation(), TargetRotation, 0.1f, 1.0f));
	 
}

void UCombatSystem::RotateToTargetActor_Implementation(AActor* ActorToRotateTo)
{
	TargetToRotate = ActorToRotateTo;

	if (TargetToRotate != nullptr)
	{
		RotateTimeline->PlayFromStart();
	}
}

