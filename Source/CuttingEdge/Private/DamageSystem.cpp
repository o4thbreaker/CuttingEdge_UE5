#include "DamageSystem.h"

// Sets default values for this component's properties
UDamageSystem::UDamageSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDamageSystem::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UDamageSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDamageSystem::TakeDamage(const FDamageInfo DamageInfo, AActor* DamageDealer, bool& WasDamaged)
{
	if (!IsDead && (DamageInfo.ShouldDamageInvincible || !IsInvincible))
	{
		if (IsBlocking && DamageInfo.CanBeBlocked)
		{
			// block damage
			OnBlockedEvent.Broadcast(DamageInfo.CanBeParried, DamageDealer);
			WasDamaged = false;
		}
		else
		{
			// do damage
			Health -= DamageInfo.DamageAmount;
			if (Health <= 0)
			{
				IsDead = true;
				OnDeathEvent.Broadcast();
				WasDamaged = true;
			}
			else
			{
				if (DamageInfo.ShouldForceInterrupt || IsInterruptible)
				{
					OnDamageResponseEvent.Broadcast(DamageInfo.DamageResponse, DamageDealer);
					WasDamaged = true;
				}
			}
		}
	}
	else
	{
		//no damage
		WasDamaged = false;
	}
}

void UDamageSystem::Heal(const float HealAmount, float& NewHealth)
{
	if (!IsDead)
	{
		Health = FMath::Clamp((Health + HealAmount), 0.0f, MaxHealth);
		NewHealth = Health;
	}
}

void UDamageSystem::ReserveAttackToken(const int Amount, bool& Success)
{
	if (AttackTokenCount >= Amount)
	{
		AttackTokenCount -= Amount;
		Success = true;
	}
	else
	{
		Success = false;
	}
}

void UDamageSystem::ReturnAttackToken(const int Amount)
{
	AttackTokenCount += Amount;
}

