// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "Actors/CPP_Flamethrower.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UnrealType.h"
#include "GameFramework/Character.h"
#include "CPP_RespawnActor.h"
#include "LogClass/ESS_LOG.h"



ACPP_Flamethrower::ACPP_Flamethrower()
{
 	
	PrimaryActorTick.bCanEverTick = false;

	// Create and assign the default scene root.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create the chimney mesh for visual representation.
	Chimney = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chimney"));
	Chimney->SetupAttachment(DefaultSceneRoot);

	// Load and apply a cylinder mesh to the chimney.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(TEXT("/Game/LevelPrototyping/Meshes/SM_Cylinder.SM_Cylinder"));
	if (CylinderMesh.Succeeded()) {
		Chimney->SetStaticMesh(CylinderMesh.Object);
	}

	// Load and apply an unlit red material to make the chimney visually distinct.
	static ConstructorHelpers::FObjectFinder<UMaterial> RedUnlit_MI(TEXT("/Game/ESS_Final/Material/RedUnlit_MI.RedUnlit_MI"));
	if (RedUnlit_MI.Succeeded()) {

		Chimney->SetMaterial(0, RedUnlit_MI.Object);
	}

	// Create the detection box component and attach to root.
	PlayerDetectBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerDetectBox"));
	PlayerDetectBox->SetupAttachment(DefaultSceneRoot);

	// Set the size of the collision box using the CollisionScale property.
	PlayerDetectBox->SetBoxExtent(CollisionScale * 50.0f);

	// Bind overlap events to the corresponding handler functions.
	PlayerDetectBox->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Flamethrower::OnBoxOverlapBegin);
	PlayerDetectBox->OnComponentEndOverlap.AddDynamic(this, &ACPP_Flamethrower::OnBoxOverlapEnd);

	// Create the Niagara component for the flamethrower effect.
	Fire = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Fire"));
	Fire->SetupAttachment(RootComponent);

	// Load and assign the flame particle system asset.
	static ConstructorHelpers::FObjectFinder <UNiagaraSystem> Flames(TEXT("/Game/ESS_Final/Particles/TorchFireGPU_NP.TorchFireGPU_NP"));
	if (Flames.Succeeded()) {
		Fire->SetAsset(Flames.Object);
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
void ACPP_Flamethrower::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
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

		// Activate the flame particle effect.
		Fire->Activate(true);

		// Reduce the player's health property "2DCharHP".
		FName VarName = FName("2DCharHP");
		FProperty* Property = OtherActor->GetClass()->FindPropertyByName(VarName);

		if (FIntProperty* IntProp = CastField<FIntProperty>(Property)) {
			int32* HP = IntProp->ContainerPtrToValuePtr<int32>(OtherActor);

			if (HP) {

				// Decrease player HP by 1.
				*HP -= 1;
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("You fainted!"));

				// Teleport the player if a TeleportTarget actor is set.
				if (TeleportTarget)
				{
					FVector TargetLocation = TeleportTarget->GetActorLocation();
					FRotator TargetRotation = PlayerCharacter->GetActorRotation(); // keep current rotation
					PlayerCharacter->SetActorLocation(TargetLocation, false, nullptr, ETeleportType::TeleportPhysics);

					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Player teleported!"));
					}
				}
			}
		}
	}

}

/**
 * OnBoxOverlapEnd
 *
 * Called when another actor ends overlapping the box collision component.
 * Can be used to stop effects or reset states when the player leaves the overlap area.
 *
 * @param OverlappedComp - The component that triggered the end overlap.
 * @param OtherActor - The other actor involved in the overlap.
 * @param OtherComp - The specific component of the other actor.
 * @param OtherBodyIndex - Body index for multi-body objects.
 */
void ACPP_Flamethrower::OnBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	// Get a reference to the player character.
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Check if the overlapping actor is the player.
	if (OtherActor == PlayerCharacter) {
		if (GEngine) {

			// Display multiple debug messages in different colors on the screen.
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Player left actor collision!"));
		}

		// Log messages to the output log with different verbosity levels.
		UE_LOG(LogCPP_ESS, Log, TEXT("Player left actor collision!"));

		// Deactivate the flame particle effect.
		Fire->Deactivate();
	}
}

/**
 * BeginPlay
 *
 * Called when the game starts or when the actor is spawned.
 * Deactivates the flame to ensure it is off at start.
 */
void ACPP_Flamethrower::BeginPlay()
{
	Super::BeginPlay();
	
	// Ensure the flamethrower effect is off at the start of the game.
	Fire->DeactivateImmediate();
}