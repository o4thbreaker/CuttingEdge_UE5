#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/DecalComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "IDamagable.h"
#include "DamageSystem.h"
#include "CombatSystem.h"
#include "EnemyInterface.h"
#include "EEnemyHitDirection.h"
#include "EnemyBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class CUTTINGEDGE_API AEnemyBase : public ACharacter, public IIDamagable, public IEnemyInterface
{
	GENERATED_BODY()

public:

	/* VARIABLES */

	/* COMPONENTS */

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = DamageSystem, meta = (AllowPrivateAccess = "true"))
	UDamageSystem* DamageSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat System", meta = (AllowPrivateAccess = "true"))
	UCombatSystem* CombatSystem;

	/* ARROWS TO HIT DIRECTIONS */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Arrows")
	UArrowComponent* _FrontArrow = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Arrows")
	UArrowComponent* _BackArrow = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Arrows")
	UArrowComponent* _LeftArrow = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Arrows")
	UArrowComponent* _RightArrow = nullptr;

	/* DECALS */

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Decal")
	UDecalComponent* HighlightDecal = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Decal")
	UDecalComponent* LockOnDecal = nullptr;

	/* DAMAGE */

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool IsAttacking = false;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	bool IsWieldingWeapon = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Teams")
	int32 TeamNumber = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Attack Tokens System")
	int32 TokenUsedInCurrentAttack = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Attack Tokens System")
	TMap<AActor*, int32> ReservedAttackTokens;

	/* DELEGATES */

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAttackEnd OnAttackEndEvent;


public:
	// Sets default values for this character's properties
	AEnemyBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Misc")
	void GetEnemyHitDirection(FVector HitLocation, EEnemyHitDirection& HitDirection, float ClosestArrowDistance = 500.0f);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage System")
	void Death();
	virtual void Death_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage System")
	void DamageResponse(EDamageResponse DamageResponse, AActor* DamageDealer);
	void DamageResponse_Implementation(EDamageResponse DamageResponse, AActor* DamageDealer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage System")
	void Blocked(bool CanBeParried, AActor* DamageDealer);
	void Blocked_Implementation(bool CanBeParried, AActor* DamageDealer);

	UFUNCTION()
	void GetEnemyReference_Implementation(AEnemyBase*& EnemyReference) override;

	UFUNCTION()
	void DefaultAttack_Implementation(AActor* AttackTarget) override;

	UFUNCTION()
	void AttackStart_Implementation(AActor* AttackTarget, int TokensNeeded, bool& Success) override; 

	UFUNCTION()
	void AttackEnd_Implementation(AActor* AttackTarget) override;

	UFUNCTION()
	void GetTeamNumber_Implementation(int32& _TeamNumber) override;

	UFUNCTION()
	void Heal_Implementation(float HealAmount, float& NewHealth) override;

	UFUNCTION()
	void GetCurrentHealth_Implementation(float& Health) override;

	UFUNCTION()
	void GetMaxHealth_Implementation(float& MaxHealth) override;

	UFUNCTION()
	void IsDead_Implementation(bool& isDead) override;

	UFUNCTION()
	void IsPerformingAttack_Implementation(bool& _IsAttacking) override;

	UFUNCTION()
	void GetIdealRangeToAttack_Implementation(float& AttackRadius, float& DefendRadius) override;

	UFUNCTION()
	void IsSelected_Implementation(bool& IsSelected) override;

	UFUNCTION()
	void SetMovementSpeed_Implementation(EMovementModes MovementMode, float& SpeedValue) override;

	UFUNCTION()
	void TakeDamage_Implementation(FDamageInfo DamageInfo, AActor* DamageCauser, bool& WasDamaged) override;

	UFUNCTION()
	void ReserveAttackToken_Implementation(const int Amount, bool& Success) override;

	UFUNCTION()
	void ReturnAttackToken_Implementation(const int Amount) override;

	UFUNCTION()
	void StoreAttackTokens_Implementation(AActor* AttackTarget, int32 TokensAmount) override;

	UFUNCTION()
	void SelectThisActor_Implementation() override;

	UFUNCTION()
	void DeselectThisActor_Implementation() override;

	UFUNCTION()
	void LockOnThisActor_Implementation() override;

	UFUNCTION()
	void LockOffThisActor_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
