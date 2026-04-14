// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "Actors/CPP_Elevator.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
//#include "GameFramework/RotatingMovementComponent.h"
#include "UObject/UnrealType.h"
#include "GameFramework/Character.h"

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

ACPP_Elevator::ACPP_Elevator()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	// Create and assign the default scene root.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create the elevator mesh component and attach it to root.
	Elevator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Elevator"));
	Elevator->SetupAttachment(DefaultSceneRoot);

	// Load a basic cube mesh for the door.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChamferCubeMesh(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube"));
	if (ChamferCubeMesh.Succeeded()) {
		Elevator->SetStaticMesh(ChamferCubeMesh.Object);
	}

	// Load and apply an unlit blue material to the elevator.
	static ConstructorHelpers::FObjectFinder<UMaterial> BlueUnlit_MI(TEXT("/Game/ESS_Final/Material/BlueUnlit_MI.BlueUnlit_MI"));
	if (BlueUnlit_MI.Succeeded()) {

		Elevator->SetMaterial(0, BlueUnlit_MI.Object);
	}

	// Create the detection box component and attach to root.
	PlayerDetectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerDetectBox"));
	PlayerDetectBox->SetupAttachment(DefaultSceneRoot);

	// Set the size of the collision box using the CollisionScale property.
	PlayerDetectBox->SetBoxExtent(CollisionScale * 50.0f);

	// Bind the overlap event to the OnBoxOverlapBegin function.
	PlayerDetectBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Elevator::OnBoxOverlapBegin);

}

/**
 * PlayerInteract_Implementation
 *
 * Called when the player interacts with this actor.
 * Implementation of the ICPP_Interact_BPI interface function.
 * Starts moving the elevator up or down if it is not currently moving.
 */
void ACPP_Elevator::PlayerInteract_Implementation() {

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("PlayerInteract called on CPP_Elevator!"));
	}

	// Flip the elevator movement direction if not already moving.
	if (!bIsMoving) {
		bIsGoingUp = !bIsGoingUp;
		bIsMoving = true;
	}
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
void ACPP_Elevator::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
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
	}
}

/**
 * BeginPlay
 *
 * Called when the game starts or when the actor is spawned.
 * Used to initialize any logic that should run at the start of gameplay.
 */
void ACPP_Elevator::BeginPlay()
{
	Super::BeginPlay();
	
	// Store the down and up locations for movement interpolation.
	DownLocation = RootComponent->GetRelativeLocation();
	UpLocation = DownLocation + FVector(0.0f, 0.0f, MoveDistance);
}

/**
 * Tick
 *
 * Called every frame. Handles moving the door smoothly
 * from closed to open or back based on player interaction.
 *
 * @param DeltaTime - Time elapsed since last frame.
 */
void ACPP_Elevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMoving) return;

	// Get current elevator location and determine target location.
	FVector CurrentLocation = RootComponent->GetRelativeLocation();
	FVector TargetLocation = bIsGoingUp ? UpLocation : DownLocation;

	// Interpolate elevator position smoothly toward the target.
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);
	RootComponent->SetRelativeLocation(NewLocation);

	// Stop movement when the elevator reaches the target location.
	if (FVector::Dist(NewLocation, TargetLocation) < 1.0f) {
		bIsMoving = false;
	}
}

