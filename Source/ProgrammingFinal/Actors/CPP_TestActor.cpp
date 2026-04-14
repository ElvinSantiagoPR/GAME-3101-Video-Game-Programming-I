// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "Actors/CPP_TestActor.h"
//#include "DetailLayoutBuilder.h" created error on build
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/RotatingMovementComponent.h"
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

/**
 * ACPP_TestActor
 *
 * Sets default values for this actor's properties.
 * Creates components for the root, mesh, collision box, and rotating movement.
 * Sets up attachment relationships and initializes default parameters.
 */
ACPP_TestActor::ACPP_TestActor() {
	// Disable ticking to improve performance; not needed for this actor.
	PrimaryActorTick.bCanEverTick = false;

	// Create and assign the default scene root.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create and attach the mesh component.
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(DefaultSceneRoot);

	// Load a default mesh for the actor.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ChamferCubeMesh(TEXT("/Game/LevelPrototyping/Meshes/SM_ChamferCube.SM_ChamferCube"));
	if (ChamferCubeMesh.Succeeded()) {
		MeshComponent->SetStaticMesh(ChamferCubeMesh.Object);
	}

	// Create and attach a box collision component for detecting overlaps.
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(DefaultSceneRoot);
	BoxCollision->SetRelativeScale3D(BoxCollisionScale);

	// Bind the overlap event to the OnBoxOverlapBegin function.
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_TestActor::OnBoxOverlapBegin);

	// Create the rotating movement component and set default rotation rate.
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->RotationRate = RotationRate;
	RotatingMovement->SetUpdatedComponent(MeshComponent);
}

/**
 * PlayerInteract_Implementation
 *
 * Called when the player interacts with this actor.
 * Implementation of the ICPP_Interact_BPI interface function.
 *
 * Displays a debug message on screen for testing purposes.
 */
void ACPP_TestActor::PlayerInteract_Implementation() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("PlayerInteract called on CPP_TestActor!"));
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
void ACPP_TestActor::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	// Get a reference to the player character.
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Check if the overlapping actor is the player.
	if (OtherActor == PlayerCharacter) {
		if (GEngine) {

			// Display multiple debug messages in different colors on the screen.
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Player overlapped with actor!"));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Player overlapped with actor!"));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Player overlapped with actor!"));
		}

		// Log messages to the output log with different verbosity levels.
		UE_LOG(LogCPP_ESS, Log, TEXT("Player overlapped with actor!"));
		UE_LOG(LogCPP_ESS, Warning, TEXT("Player overlapped with actor!"));
		UE_LOG(LogCPP_ESS, Error, TEXT("Player overlapped with actor!"));
	}

	FName VarName = FName("2DCharHP");
	FProperty* Property = OtherActor->GetClass()->FindPropertyByName(VarName);

	if (Property) {
		if (FIntProperty* IntProp = CastField<FIntProperty>(Property)) {
			int32 Value = IntProp->GetPropertyValue_InContainer(OtherActor);

			//IntProp->SetPropertyValue_InContainer(OtherActor, Value -= 1);

			UE_LOG(LogCPP_ESS, Warning, TEXT("2DCharHP: %d"), Value);

			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, FString::Printf(TEXT("2DCharHP: %d"), Value));
			}
		}
		else {
			UE_LOG(LogCPP_ESS, Error, TEXT("Property found but is not an Integer! Actual type: %s"), *Property->GetClass()->GetName());
		}
	}
	else {
		UE_LOG(LogCPP_ESS, Warning, TEXT("Property not found!"));
	}
}

/**
 * BeginPlay
 *
 * Called when the game starts or when the actor is spawned.
 * Used to initialize any logic that should run at the start of gameplay.
 */
void ACPP_TestActor::BeginPlay()
{
	Super::BeginPlay();
	
}