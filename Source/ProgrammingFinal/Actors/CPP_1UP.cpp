// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "Actors/CPP_1UP.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UnrealType.h"
#include "GameFramework/Character.h"
#include "GameFramework/RotatingMovementComponent.h"

/**
 * LogCPP_ESS
 *
 * Defines a static log category for this C++ class.
 *
 * - Log: Default verbosity level for standard messages.
 * - All: Enables logging of all types (Log, Warning, Error, etc.).
 *
 * This category is used with UE_LOG macros for debugging and tracking events
 * specific to this actor or module.
 */
DEFINE_LOG_CATEGORY_STATIC(LogCPP_ESS, Log, All);

ACPP_1UP::ACPP_1UP() {
	
	PrimaryActorTick.bCanEverTick = false;

	// Create and assign the default scene root.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create the 1UP mesh and attach it to the root.
	HealCross = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealCross"));
	HealCross->SetupAttachment(DefaultSceneRoot);
	HealCross->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Load and assign the mesh for the 1UP.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HealCross_SM(TEXT("/Game/ESS_Final/Meshes/HealCross_SM.HealCross_SM"));
	if (HealCross_SM.Succeeded()) {
		HealCross->SetStaticMesh(HealCross_SM.Object);
	}

	// Load and apply the unlit material to make the 1UP visually distinct.
	static ConstructorHelpers::FObjectFinder<UMaterial> GreenUnlit_MI(TEXT("/Game/ESS_Final/Material/GreenUnlit_MI.GreenUnlit_MI"));
	if (GreenUnlit_MI.Succeeded()) {

		HealCross->SetMaterial(0, GreenUnlit_MI.Object);
	}

	// Create a sphere component to detect player overlaps.
	PlayerDetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectSphere"));
	PlayerDetectSphere->SetupAttachment(DefaultSceneRoot);
	PlayerDetectSphere->InitSphereRadius(CollisionScale * 50.0f);
	PlayerDetectSphere->OnComponentBeginOverlap.AddDynamic(this, &ACPP_1UP::OnBoxOverlapBegin);

	// Add a rotating movement component so the 1UP spins automatically.
	RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	// Rotate around Yaw axis
	RotatingComponent->RotationRate = FRotator(0.0f, 100.0f, 0.0f);
}


void ACPP_1UP::BeginPlay() {
	Super::BeginPlay();
}

/**
	 * OnBoxOverlapBegin
	 *
	 * Called when another actor begins overlapping the box collision component.
	 * Checks if the overlapping actor is the player and logs debug messages.
	 *
	 * @param OverlappedComp - The component that triggered the overlap.
	 * @param OtherActor - The other actor involved in the overlap.
	 * @param OtherComp - The specific component of the other actor.
	 * @param OtherBodyIndex - Body index for multi-body objects.
	 * @param bFromSweep - True if the overlap was from a sweep movement.
	 * @param SweepResult - Hit result data if from sweep.
	 */
void ACPP_1UP::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// Get a reference to the player character.
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Check if the overlapping actor is the player.
	if (OtherActor == PlayerCharacter) {
		if (GEngine) {

			// Display multiple debug messages in different colors on the screen.
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Player overlapped with actor!"));
		}

		// Log messages to the output log with different verbosity levels.
		UE_LOG(LogCPP_ESS, Log, TEXT("Player overlapped with actor!"));

		// Increase the player's 2DCharHP by 1 if the property exists.
		FName VarName = FName("2DCharHP");
		FProperty* Property = OtherActor->GetClass()->FindPropertyByName(VarName);

		if (FIntProperty* IntProp = CastField<FIntProperty>(Property)) {
			int32* HP = IntProp->ContainerPtrToValuePtr<int32>(OtherActor);

			if (HP) {

				*HP += 1;

				// Display feedback that the player gained an extra life.
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Extra life!"));

				// Destroy the 1UP actor after being collected.
				Destroy();
			}
		}
	}
}