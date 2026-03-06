#include "DashPointSystem.h"

// Sets default values for this component's properties
UDashPointSystem::UDashPointSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDashPointSystem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UDashPointSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDashPointSystem::LoseAllDashPoints()
{
	dashPointCount = 0;
	isDashEnabled = false;
	OnAllPointsLost.Broadcast();
}

void UDashPointSystem::GetDashPoint()
{
	dashPointCount++;
	OnPointEarned.Broadcast();

	if (dashPointCount >= maxDashPoints)
	{
		isDashEnabled = true;
		OnAllPointsEarned.Broadcast();
	}
}

