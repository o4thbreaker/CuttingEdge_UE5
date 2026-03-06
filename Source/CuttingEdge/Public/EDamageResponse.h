#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EDamageResponse : uint8
{
	None,
	HitReaction,
	Stagger,
	Stun,
	Knockback
};
