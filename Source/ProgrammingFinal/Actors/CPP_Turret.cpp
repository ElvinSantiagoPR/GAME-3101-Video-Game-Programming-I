// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "Actors/CPP_Turret.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "LogClass/ESS_LOG.h"



ACPP_Turret::ACPP_Turret()
{
	
	PrimaryActorTick.bCanEverTick = false;

	// Create and assign the default scene root.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create and attach the turret mesh component.
	Turret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret"));
	Turret->SetupAttachment(DefaultSceneRoot);

	// Load and assign a sphere mesh to represent the turret visually.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MaterialSphere(TEXT("/Game/LevelPrototyping/Meshes/SM_MaterialSphere.SM_MaterialSphere"));
	if (SM_MaterialSphere.Succeeded()) {
		Turret->SetStaticMesh(SM_MaterialSphere.Object);
	}

	// Load and apply a red unlit material to the turret mesh.
	static ConstructorHelpers::FObjectFinder<UMaterial> RedUnlit_MI(TEXT("/Game/ESS_Final/Material/RedUnlit_MI.RedUnlit_MI"));
	if (RedUnlit_MI.Succeeded()) {

		Turret->SetMaterial(0, RedUnlit_MI.Object);
	}
}

/**
 * BeginPlay
 *
 * Called when the game starts or when the actor is spawned.
 * Initializes the reference to the player and sets up a repeating fire timer.
 */
void ACPP_Turret::BeginPlay()
{
	Super::BeginPlay();

	// Get a reference to the player pawn for targeting.
	PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Set a repeating timer to call the Fire() function at intervals defined by FireRate.
	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACPP_Turret::Fire, FireRate, true);
}

/**
 * Fire
 *
 * Spawns a projectile aimed at the player if they are within AttackRange.
 * Logs firing events to both screen and output log for debugging purposes.
 */
void ACPP_Turret::Fire() {

	// Check if the player reference is valid.
	if (IsValid(PlayerRef)) {

		// Calculate the direction vector from turret to player.
		FVector Direction = PlayerRef->GetActorLocation() - GetActorLocation();

		// Calculate the distance to the player.
		float Distance = Direction.Size();
		
		// Only fire if the player is within the turret's attack range.
		if (Distance > AttackRange) return;

		// Spawn the projectile actor at the turret's location, facing the player.
		AActor* shot = GetWorld()->SpawnActor<AActor>(
			ProjectileClass,
			GetActorLocation(),
			Direction.Rotation()
		);

		// Debug messages for visual confirmation and logging.
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Turret Fired!"));
		}
		UE_LOG(LogCPP_ESS, Log, TEXT("Turret Fired!"));
	}
	else {

		// If player reference is invalid, attempt to reacquire it.
		PlayerRef = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
}