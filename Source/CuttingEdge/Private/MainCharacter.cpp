#include "MainCharacter.h"


// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MontageController = CreateDefaultSubobject<APlayMontageController>(TEXT("MontageController"));

	CombatSystem = CreateDefaultSubobject<UCombatSystem>(TEXT("CombatSystem"));
	CombatMode = CreateDefaultSubobject<UPlayerCombatMode>(TEXT("CombatMode"));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 225.0f; // The camera follows at this distance behind the character	
	CameraBoom->SocketOffset = FVector(0.0f, 65.0f, 50.0f);
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Init default camera post proccess effect
	StandartCameraEffect = FollowCamera->PostProcessSettings;

	// Init a slowmo effect post process
	SlowMotionEffect.WhiteTemp = 15000;
	SlowMotionEffect.bOverride_WhiteTemp = true;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMainCharacter::StartSlowMotion(float globalTimeDilation)
{
	GetWorldSettings()->SetTimeDilation(globalTimeDilation);

	CustomTimeDilation = (globalTimeDilation > 0) ? (1 / globalTimeDilation) : 0;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, *(FString::Printf(TEXT("CHARACTER DILATION: %f"), CustomTimeDilation)));

	FollowCamera->PostProcessSettings = SlowMotionEffect;
	
}

void AMainCharacter::EndSlowMotion()
{
	GetWorldSettings()->SetTimeDilation(1.0f);
	CustomTimeDilation = 1.0f;

	FollowCamera->PostProcessSettings = StandartCameraEffect;
	
}

void AMainCharacter::HighlightTarget()
{
	FVector StartPosition = FollowCamera->GetComponentLocation();
	//TODO: extact float number to a value
	FVector EndPosition = FollowCamera->GetComponentLocation() + FollowCamera->GetForwardVector() * 3000;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

	if (GetWorld()->LineTraceSingleByObjectType(HitResult, StartPosition, EndPosition, ObjectParams, Params))
	{
		// if we've got a hit with smth
		if (HitResult.GetActor()->Implements<UEnemyInterface>())
		{
			// if hit object implements the interface
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
			if (SelectedActor)
			{
				// and if there is an actor that is selected
				// we deselect him
				IEnemyInterface::Execute_DeselectThisActor(SelectedActor);
				// and select new one
				IEnemyInterface::Execute_SelectThisActor(HitResult.GetActor());
				SelectedActor = HitResult.GetActor();
			}
			else
				// if no actor is selected
			{
				// then select this actor
				IEnemyInterface::Execute_SelectThisActor(HitResult.GetActor());
				SelectedActor = HitResult.GetActor();
			}
		}
		else
		{
			// if the object we hit does not implement the interface
			if (SelectedActor)
			{
				IEnemyInterface::Execute_DeselectThisActor(SelectedActor);
				SelectedActor = nullptr;
			}
		}
	}
	else
	{
		// if we hit nothing
		if (SelectedActor)
		{
			// but some actor is selected, we deselect him
			IEnemyInterface::Execute_DeselectThisActor(SelectedActor);
			SelectedActor = nullptr;
		}
	}
}

void AMainCharacter::DehighlightTarget()
{
	if (SelectedActor)
	{
		IEnemyInterface::Execute_DeselectThisActor(SelectedActor);
		SelectedActor = nullptr;
	}
}

void AMainCharacter::GetSelectedEnemyLocation(FVector& EnemyLocation, FRotator& EnemyRotation, float Offset)
{
	EnemyLocation = (GetActorLocation() - SelectedActor->GetActorLocation()).GetSafeNormal(0.0001f) * Offset + SelectedActor->GetActorLocation();
	EnemyRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), SelectedActor->GetActorLocation());
}

void AMainCharacter::RotatePlayerTowardsTarget(AActor* TargetActor, float InterpolationSpeed)
{
	FRotator CurrentCameraRotation = CameraBoom->GetComponentRotation();
	FRotator TargetCameraRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
	float InterpolatedRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), 
		TargetCameraRotation, GetWorld()->GetDeltaSeconds(), InterpolationSpeed).Yaw;
	SetActorRotation(FRotator(0, InterpolatedRotation, 0), ETeleportType::TeleportPhysics);

	CameraBoom->SetWorldRotation(CurrentCameraRotation);
}