#pragma once

#include "CoreMinimal.h"
#include "EDamageType.h"
#include "EDamageResponse.h"
#include "FDamageInfo.generated.h"

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()
	
public:
	/** Amount of applied damage */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "DamageAmount", MakeStructureDefaultValue = "0.000000"))
	double DamageAmount;

	/** Type of applied damage (None, Melee, Critical, etc...) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "DamageType", MakeStructureDefaultValue = "None"))
	EDamageType DamageType;

	/** How to respond to applied damage (None, Stagger, Stun, Knockback, etc...) */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "DamageResponse", MakeStructureDefaultValue = "None"))
	EDamageResponse DamageResponse;

	/** Should this attack ignore invincibility */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ShouldDamageInvincible", MakeStructureDefaultValue = "False"))
	bool ShouldDamageInvincible;

	/** Can this attack be blocked */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "CanBeBlocked", MakeStructureDefaultValue = "False"))
	bool CanBeBlocked;

	/** Can this attack be parried */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "CanBeParried", MakeStructureDefaultValue = "False"))
	bool CanBeParried;

	/** Can this attack interrupt current move interrupt uniterruptable moves */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "ShouldForceInterrupt", MakeStructureDefaultValue = "False"))
	bool ShouldForceInterrupt;
};
