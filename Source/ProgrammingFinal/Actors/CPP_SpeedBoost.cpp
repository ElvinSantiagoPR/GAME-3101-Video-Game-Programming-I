// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "Actors/CPP_SpeedBoost.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UnrealType.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "2D_Character/CPP_2D_SideScrollerGameMode.h"
#include "LogClass/ESS_LOG.h"


ACPP_SpeedBoost::ACPP_SpeedBoost()
{
	
	PrimaryActorTick.bCanEverTick = false;

	// Create and assign the default scene root.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create and setup the visual orb mesh.
	Orb = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Orb"));
	Orb->SetupAttachment(DefaultSceneRoot);
	Orb->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Load and apply the orb mesh.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MaterialSphere(TEXT("/Game/LevelPrototyping/Meshes/SM_MaterialSphere.SM_MaterialSphere"));
	if (SM_MaterialSphere.Succeeded()) {

		Orb->SetStaticMesh(SM_MaterialSphere.Object);

	}
	// Load and apply the unlit material to make the orb visually distinct..
	static ConstructorHelpers::FObjectFinder<UMaterial> YellowUnlit_MI(TEXT("/Game/ESS_Final/Material/YellowUnlit_MI.YellowUnlit_MI"));
	if (YellowUnlit_MI.Succeeded()) {

		Orb->SetMaterial(0, YellowUnlit_MI.Object);
	}

	// Create the sphere collision component for detecting player overlap.
	PlayerDetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectSphere"));
	PlayerDetectSphere->SetupAttachment(DefaultSceneRoot);

	// Set the size of the collision box using the CollisionScale property.
	PlayerDetectSphere->InitSphereRadius(CollisionScale * 50.0f);

	// Bind overlap event to the OnBoxOverlapBegin function.
	PlayerDetectSphere->OnComponentBeginOverlap.AddDynamic(this, &ACPP_SpeedBoost::OnBoxOverlapBegin);

}


/**
 * OnBoxOverlapBegin
 *
 * Called when another actor begins overlapping the box collision component.
 * If the overlapping actor is the player, applies the jump boost, hides the orb,
 * and sets timers for buff duration and orb respawn.
 *
 * @param OverlappedComp - The component that triggered the overlap.
 * @param OtherActor - The other actor involved in the overlap.
 * @param OtherComp - The specific component of the other actor.
 * @param OtherBodyIndex - Body index for multi-body objects.
 * @param bFromSweep - True if the overlap was from a sweep movement.
 * @param SweepResult - Hit result data if from sweep.
 */
void ACPP_SpeedBoost::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// Get a reference to the player character.
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Check if the overlapping actor is the player.
	if (OtherActor == PlayerCharacter) {
		if (GEngine) {

			// Display multiple debug messages in different colors on the screen.
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Player overlapped with actor!"));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Power Up! 10s Speed Boost!"));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Speed Boost will Respawn in 10s!"));
		}

		// Log messages to the output log with different verbosity levels.
		UE_LOG(LogCPP_ESS, Log, TEXT("Player overlapped with actor!"));
		UE_LOG(LogCPP_ESS, Warning, TEXT("Power Up! 10s Speed Boost!"));
		UE_LOG(LogCPP_ESS, Error, TEXT("Speed Boost will Respawn in 10s"));

		// Log messages to the output log with different verbosity levels.
		UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement();
		if (!MoveComp) return;

		// Hide the orb and disable its collision to prevent re-collection.
		Orb->SetVisibility(false);
		PlayerDetectSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Multiply player's max walk speed by the speed multiplier.
		MoveComp->MaxWalkSpeed *= SpeedMultiplier;

		// Set a "bHasSpeedBoost" boolean on the player for tracking the buff state.
		FName VarName = FName("bHasSpeedBoost");
		FProperty* Property = OtherActor->GetClass()->FindPropertyByName(VarName);

		if (Property) {
			if (FBoolProperty* BoolProp = CastField<FBoolProperty>(Property)) {

				BoolProp->SetPropertyValue_InContainer(OtherActor, true);

				UE_LOG(LogCPP_ESS, Warning, TEXT("Has Speed Boost: true"));

				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("Has Speed Boost: true")));
				}
			}
		}

		// Set a timer to remove the jump boost effect after BuffDuration.
		GetWorld()->GetTimerManager().SetTimer(
			BuffTimerHandler,
			[this]() {
				ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

				if (!Player) return;

				auto Movement = Player->GetCharacterMovement();

				if (!Movement) return;

				// Revert the max walk speed back to normal.
				Movement->MaxWalkSpeed /= SpeedMultiplier;

				// Reset the "bHasSpeedBoost" boolean on the player.
				FName VarName = FName("bHasSpeedBoost");
				FProperty* Property = Player->GetClass()->FindPropertyByName(VarName);

				if (Property) {
					if (FBoolProperty* BoolProp = CastField<FBoolProperty>(Property)) {

						BoolProp->SetPropertyValue_InContainer(Player, false);

						UE_LOG(LogCPP_ESS, Warning, TEXT("Has Speed Boost: false"));

						if (GEngine) {
							GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("Has Speed Boost: false")));
						}
					}
				}

				// Debug messages to indicate buff ended and orb will respawn.
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Speed Boost ran out!"));
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Speed Boost will respawn in 10s!"));
				}
			},
			BuffDuration,
			false
		);

		// Set a timer to respawn the orb after BuffDuration + RespawnDelay.
		GetWorld()->GetTimerManager().SetTimer(
			RespawnTimerHandler,
			[this]() {

				// Make the orb visible and re-enable collision.
				Orb->SetVisibility(true);
				PlayerDetectSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Speed Boost respawned!"));
				}
			},
			(BuffDuration + RespawnDelay),
			false
		);
	}
}

/**
 * BeginPlay
 *
 * Called when the game starts or when the actor is spawned.
 * Used to initialize any logic that should run at the start of gameplay.
 */
void ACPP_SpeedBoost::BeginPlay()
{
	Super::BeginPlay();
	
}