// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "Actors/CPP_BoulderCheck.h"
#include "Actors/CPP_Boulder.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

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

/**
 * ACPP_BoulderCheck
 *
 * This actor is used to detect when the boulder reaches a specific area.
 * It contains a collision box that listens for overlap events and a blocking
 * wall that becomes enabled or disabled based on the boulder’s position.
 *
 * This class can be expanded to trigger cutscenes, open doors, or
 * activate gameplay events when the boulder enters the detection zone.
 */
ACPP_BoulderCheck::ACPP_BoulderCheck()
{
	// Disable Tick() for performance as this actor does not require per-frame updates.
	PrimaryActorTick.bCanEverTick = false;

	// Create and assign the Default Scene Root component.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create the blocking wall mesh component and attach to root.
	BlockingWall = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockingWall"));
	BlockingWall->SetupAttachment(DefaultSceneRoot);

	// Load and apply the mesh for the blocking wall.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChamferCubeMesh(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube"));
	if (ChamferCubeMesh.Succeeded())
	{
		BlockingWall->SetStaticMesh(ChamferCubeMesh.Object);
	}

	// Set collision profile so it blocks the path.
	BlockingWall->SetCollisionProfileName(TEXT("BlockAll"));

	// Load and apply the unlit material for the blocking wall.
	static ConstructorHelpers::FObjectFinder<UMaterial> BlueUnlit_MI(TEXT("/Game/ESS_Final/Material/BlueUnlit_MI.BlueUnlit_MI"));
	if (BlueUnlit_MI.Succeeded()) {

		BlockingWall->SetMaterial(0, BlueUnlit_MI.Object);
	}

	// Create the detection box component and attach to root.
	BoulderDetectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoulderDetectBox"));
	BoulderDetectBox->SetupAttachment(DefaultSceneRoot);

	// Set the size of the collision box using the BoxCollisionScale property.
	BoulderDetectBox->SetBoxExtent(BoxCollisionScale * 50.0f);

	// Set collision profile to trigger to detect overlaps.
	BoulderDetectBox->SetCollisionProfileName("Trigger");

	// Bind the overlap event to the OnBoxOverlapBegin function.
	BoulderDetectBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_BoulderCheck::OnBoxOverlapBegin);

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
void ACPP_BoulderCheck::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Safety check that prevents the function from running if the inputs are invalid.
	if (!OtherActor || !OtherComp) return;

	// Cast the overlapping actor to ACPP_Boulder.
	ACPP_Boulder* Boulder = Cast<ACPP_Boulder>(OtherActor);

	if (Boulder) {

		// Display debug message on screen.
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Path Cleared!"));
		}
		// Log messages to the output log with different verbosity levels.
		UE_LOG(LogCPP_ESS, Log, TEXT("Path Cleared!"));

		// Hide the blocking wall and disable its collision.
		if (BlockingWall) {
			BlockingWall->SetVisibility(false, true);
			BlockingWall->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
}

/**
 * BeginPlay
 *
 * Called when the game starts or when the actor is spawned.
 * Used to initialize any logic that should run at the start of gameplay.
 */
void ACPP_BoulderCheck::BeginPlay()
{
	Super::BeginPlay();
}