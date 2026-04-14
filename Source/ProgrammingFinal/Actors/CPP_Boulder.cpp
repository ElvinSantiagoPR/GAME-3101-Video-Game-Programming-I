// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "Actors/CPP_Boulder.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"

/**
 * ACPP_Boulder
 *
 * Sets default values for this actor's properties and initializes
 * components such as the scene root and mesh. Also configures
 * physics, collision, and material settings for the boulder.
 */
ACPP_Boulder::ACPP_Boulder()
{
	// Disable Tick() since this actor does not need per-frame updates.
	PrimaryActorTick.bCanEverTick = false;

	// Create and assign the Default Scene Root component.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create the Static Mesh Component and attach it to the root.
	Boulder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boulder"));
	Boulder->SetupAttachment(DefaultSceneRoot);

	// Set physics-based collision profile for the boulder.
	Boulder->SetCollisionProfileName("PhysicsActor");

	// Lock movement and rotations to keep behavior consistent for a 2D game.
	Boulder->BodyInstance.bLockYTranslation = true;
	Boulder->BodyInstance.bLockZRotation = true;
	Boulder->BodyInstance.bLockXRotation = true;

	// Enable physics and gravity.
	Boulder->SetEnableGravity(true);
	Boulder->SetSimulatePhysics(true);

	// Update mass values after physics settings are applied.
	Boulder->BodyInstance.UpdateMassProperties();

	// Load and apply the boulder mesh.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MaterialSphere(TEXT("/Game/LevelPrototyping/Meshes/SM_MaterialSphere.SM_MaterialSphere"));
	if (SM_MaterialSphere.Succeeded()) {

		Boulder->SetStaticMesh(SM_MaterialSphere.Object);

	}
	// Load and apply the unlit material.
	static ConstructorHelpers::FObjectFinder<UMaterial> BlueUnlit_MI(TEXT("/Game/ESS_Final/Material/BlueUnlit_MI.BlueUnlit_MI"));
	if (BlueUnlit_MI.Succeeded()) {

		Boulder->SetMaterial(0, BlueUnlit_MI.Object);
	}
}

/**
 * BeginPlay
 *
 * Called when the game starts or when the actor is spawned.
 * Used to initialize any logic that should run at the start of gameplay.
 */
void ACPP_Boulder::BeginPlay()
{
	Super::BeginPlay();
}