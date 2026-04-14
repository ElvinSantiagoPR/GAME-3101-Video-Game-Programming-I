// Elvin Santiago Santiago
// Game 3101 - Programming for Video Games I
// Final Project
// Dec 12, 2025


#include "2D_Character/CPP_2D_PaperZDCharacter.h"

// Sets default values
ACPP_2D_PaperZDCharacter::ACPP_2D_PaperZDCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the spring arm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	// Position the spring arm behind and above the character
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 400.0f; // Distance from the character
	SpringArmComponent->bUsePawnControlRotation = false; // We don’t rotate it with the controller for 2D
	SpringArmComponent->bDoCollisionTest = false; // Optional, prevents zooming in on obstacles

	// Create and attach the camera to the spring arm
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	// Adjust camera angle — side view for 2D
	CameraComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); // Face the X-axis
	CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
	CameraComponent->OrthoWidth = 2048.0f; // Adjust this to control zoom level
}

// Called when the game starts or when spawned
void ACPP_2D_PaperZDCharacter::BeginPlay()
{
	Super::BeginPlay();
}
