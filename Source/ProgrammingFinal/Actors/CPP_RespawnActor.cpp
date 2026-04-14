// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "Actors/CPP_RespawnActor.h"


ACPP_RespawnActor::ACPP_RespawnActor()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	// Create and assign the default scene root.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create a box component to visualize the respawn area or trigger overlaps if needed
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(DefaultSceneRoot);

	// Initialize the respawn location to the actor's starting location
	RespawnLocation = GetActorLocation();

}

/**
 * BeginPlay
 *
 * Called when the game starts or when the actor is spawned.
 * Used to initialize any logic that should run at the start of gameplay.
 */
void ACPP_RespawnActor::BeginPlay()
{
	Super::BeginPlay();
}