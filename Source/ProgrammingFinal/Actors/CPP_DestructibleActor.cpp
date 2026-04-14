// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025


#include "Actors/CPP_DestructibleActor.h"


ACPP_DestructibleActor::ACPP_DestructibleActor()
{
	
	PrimaryActorTick.bCanEverTick = false;

	// Create and assign the default scene root.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create the static mesh component for the destructible box.
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Box"));
	Box->SetupAttachment(DefaultSceneRoot);

	// Load and assign a chamfered cube mesh for the box.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChamferCubeMesh(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube"));
	if (ChamferCubeMesh.Succeeded()) {
		Box->SetStaticMesh(ChamferCubeMesh.Object);
	}

	// Load and apply a blue unlit material for visual distinction.
	static ConstructorHelpers::FObjectFinder<UMaterial> BlueUnlit_MI(TEXT("/Game/ESS_Final/Material/BlueUnlit_MI.BlueUnlit_MI"));
	if (BlueUnlit_MI.Succeeded()) {

		Box->SetMaterial(0, BlueUnlit_MI.Object);
	}

}

/**
 * BeginPlay
 *
 * Called when the game starts or when the actor is spawned.
 * Can be used to initialize any runtime logic for the destructible actor.
 */
void ACPP_DestructibleActor::BeginPlay()
{
	Super::BeginPlay();
}