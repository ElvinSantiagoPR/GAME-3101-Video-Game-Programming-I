// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "Actors/CPP_MovingPlatform.h"

// Sets default values
ACPP_MovingPlatform::ACPP_MovingPlatform()
{
	// Enable ticking to allow per-frame movement updates.
	PrimaryActorTick.bCanEverTick = true;

	// Create and assign the default scene root.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create the platform mesh component and attach it to the root.
	Platform = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	Platform->SetupAttachment(DefaultSceneRoot);

	// Load and assign a chamfered cube mesh to the platform.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChamferCubeMesh(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube"));
	if (ChamferCubeMesh.Succeeded()) {
		Platform->SetStaticMesh(ChamferCubeMesh.Object);
	}

}

// Called when the game starts or when spawned
void ACPP_MovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	// Store the initial position as the left boundary of movement.
	LeftLocation = RootComponent->GetRelativeLocation();

	// Calculate the right boundary based on MoveDistance along the X-axis.
	RightLocation = LeftLocation + FVector(MoveDistance, 0.0f, 0.0f);


}

/**
 * Tick
 *
 * Called every frame. Handles moving the door smoothly
 * from closed to open or back based on player interaction.
 *
 * @param DeltaTime - Time elapsed since last frame.
 */
void ACPP_MovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Do nothing if the platform is not set to move.
	if (!bIsMoving) return;

	// Get the current position of the platform.
	FVector CurrentLocation = RootComponent->GetRelativeLocation();

	// Determine the current target location based on movement direction.
	FVector TargetLocation = bIsGoingRight ? RightLocation : LeftLocation;

	// Interpolate towards the target location at a constant speed.

	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, MoveSpeed);

	// Update the platform's position.
	RootComponent->SetRelativeLocation(NewLocation);

	// Check if the platform is very close to the target; if so, reverse direction.
	if (FVector::Dist(NewLocation, TargetLocation) < 1.0f)
	{
		bIsGoingRight = !bIsGoingRight;
	}
	
}