// Fill out your copyright notice in the Description page of Project Settings.// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025

#include "Actors/CPP_Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UnrealType.h"
#include "GameFramework/Character.h"
#include "CPP_RespawnActor.h"

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


ACPP_Projectile::ACPP_Projectile()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	// Create and assign the default scene root.
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	RootComponent = DefaultSceneRoot;

	// Create and attach the projectile mesh.
	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
	Bullet->SetupAttachment(DefaultSceneRoot);
	Bullet->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Load a sphere mesh for the projectile.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MaterialSphere(TEXT("/Game/LevelPrototyping/Meshes/SM_MaterialSphere.SM_MaterialSphere"));
	if (SM_MaterialSphere.Succeeded()) {
		Bullet->SetStaticMesh(SM_MaterialSphere.Object);
	}

	// Apply an unlit red material to the projectile mesh.
	static ConstructorHelpers::FObjectFinder<UMaterial> RedUnlit_MI(TEXT("/Game/ESS_Final/Material/RedUnlit_MI.RedUnlit_MI"));
	if (RedUnlit_MI.Succeeded()) {

		Bullet->SetMaterial(0, RedUnlit_MI.Object);
	}

	// Create the sphere collision for detecting the player.
	PlayerDetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerDetectSphere"));
	PlayerDetectSphere->SetupAttachment(DefaultSceneRoot);
	PlayerDetectSphere->InitSphereRadius(CollisionScale * 50.0f);

	// Bind the overlap event to the OnBoxOverlapBegin function.
	PlayerDetectSphere->OnComponentBeginOverlap.AddDynamic(this, &ACPP_Projectile::OnBoxOverlapBegin);

	// Automatically destroy the projectile after its lifespan expires.
	SetLifeSpan(LifeSpan);
}

/**
 * OnBoxOverlapBegin
 *
 * Called when another actor begins overlapping the projectile's collision sphere.
 * If the overlapping actor is the player, reduces HP, optionally teleports,
 * and logs debug messages.
 *
 * @param OverlappedComp - Component that triggered the overlap.
 * @param OtherActor - Actor involved in the overlap.
 * @param OtherComp - Component of the other actor.
 * @param OtherBodyIndex - Body index for multi-body objects.
 * @param bFromSweep - True if the overlap was from a sweep movement.
 * @param SweepResult - Hit result if from sweep.
 */
void ACPP_Projectile::OnBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// Get reference to the player character.
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Check if the overlapping actor is the player.
	if (OtherActor == PlayerCharacter) {
		if (GEngine) {

			// Display multiple debug messages in different colors on the screen.
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Player overlapped with actor!"));
		}

		// Log messages to the output log with different verbosity levels.
		UE_LOG(LogCPP_ESS, Log, TEXT("Player overlapped with actor!"));

		// Reduce player HP using reflection to access the 2DCharHP property.
		FName VarName = FName("2DCharHP");
		FProperty* Property = PlayerCharacter->GetClass()->FindPropertyByName(VarName);

		if (FIntProperty* IntProp = CastField<FIntProperty>(Property)) {
			int32* HP = IntProp->ContainerPtrToValuePtr<int32>(PlayerCharacter);

			if (HP) {

				*HP -= 1;
				// Display a message when the player takes damage.
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("You fainted!"));

				// Teleport player to the designated target if set.
				if (TeleportTarget)
				{
					FVector TargetLocation = TeleportTarget->GetActorLocation();
					FRotator TargetRotation = PlayerCharacter->GetActorRotation();
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
 * BeginPlay
 *
 * Called when the game starts or when the actor is spawned.
 * Initializes the teleport target reference to the ACPP_RespawnActor in the scene.
 */
void ACPP_Projectile::BeginPlay()
{
	Super::BeginPlay();

	// Find the respawn actor in the level and set it as the teleport target.
	TeleportTarget = Cast<AActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACPP_RespawnActor::StaticClass()));
}

/**
 * Tick
 *
 * Called every frame. Handles moving the door smoothly
 * from closed to open or back based on player interaction.
 *
 * @param DeltaTime - Time elapsed since last frame.
 */
void ACPP_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move the projectile forward along its local forward vector.
	FVector Move = GetActorForwardVector() * Speed * DeltaTime;
	SetActorLocation(GetActorLocation() + Move);
}