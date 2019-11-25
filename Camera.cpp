// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "MainController.h"

// Sets default values
ACamera::ACamera()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creating default subobjects and making attachments
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Scene);
	Sphere->SetCollisionProfileName(TEXT("NoCollision"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Sphere);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ACamera::BeginPlay()
{
	Super::BeginPlay();

	SpringArm->SetRelativeRotation(FRotator(-70, 0, 0));
	SpringArm->TargetArmLength = 500;

	// Reference to camera pawn controller
	MainController = Cast<AMainController>(GetController());
	if (MainController == nullptr)
		UE_LOG(LogTemp, Fatal, TEXT("Can't create a reference to AMainController!"));
	MainController->bShowMouseCursor = true;
}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Pan();
	if(!bDisableCamNav)
		ScreenEdgeMovement();
}

// Called to bind functionality to input
void ACamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACamera::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACamera::MoveRight);
	PlayerInputComponent->BindAction("Pan", IE_Pressed, this, &ACamera::PanPressed);
	PlayerInputComponent->BindAction("Pan", IE_Released, this, &ACamera::PanReleased);

	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ACamera::ZoomIn);
	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ACamera::ZoomOut);

	PlayerInputComponent->BindAction("MultiplySpeed", IE_Pressed, this, &ACamera::ShiftPressed);
	PlayerInputComponent->BindAction("MultiplySpeed", IE_Released, this, &ACamera::ShiftReleased);

}

float ACamera::MovementSpeedCalculation()
{
	return UKismetMathLibrary::Clamp(SpringArm->TargetArmLength / 100, 5, 20);
}

/**
*	Fired when W and S keys are pressed. Calls MovementX to calculate movement direction.
*	@param Value - value received from input binding such as 1 for W and -1 for S
*/
void ACamera::MoveForward(float Value)
{
	if (bDisableCamNav == false)
	{
		// Set actor transform and assign to a variable
		FTransform ActorTransform = GetActorTransform();

		// Set new location
		FVector NewLocation = ActorTransform.GetLocation() + UKismetMathLibrary::TransformDirection(ActorTransform, FVector((Value * MovementSpeedCalculation()) * FastMoveMultiplier, 0, 0));
		NewLocation.Z = 200;

		// Set new transform
		FTransform NewTransform = FTransform(ActorTransform.GetRotation(), NewLocation, ActorTransform.GetScale3D());

		// Set new actor transform
		SetActorTransform(NewTransform);
	}
}

/**
*	Fired when D and A keys are pressed. Calls MovementY to calculate movement direction.
*	@param Value - value received from input binding such as 1 for D and -1 for A
*/
void ACamera::MoveRight(float Value)
{
	if (bDisableCamNav == false)
	{
		// Set actor transform and assign to a variable
		FTransform ActorTransform = GetActorTransform();

		// Set new location
		FVector NewLocation = ActorTransform.GetLocation() + UKismetMathLibrary::TransformDirection(ActorTransform, FVector(0, (Value * MovementSpeedCalculation()) * FastMoveMultiplier, 0));
		NewLocation.Z = 200;

		// Set new transform
		FTransform NewTransform = FTransform(ActorTransform.GetRotation(), NewLocation, ActorTransform.GetScale3D());

		// Set new actor transform
		SetActorTransform(NewTransform);
	}
}

void ACamera::PanPressed()
{
	bDisableCamNav = true;
	bIsPanning = true;
	MainController->GetMousePosition(InitialMousePosX, InitialMousePosY);
}

void ACamera::PanReleased()
{
	bDisableCamNav = false;
	bIsPanning = false;
}

void ACamera::Pan()
{
	/* Rotate if middle mouse clicked */
	if (bIsPanning)
	{
		// Calculate which way to rotate
		float AxisValueX = 0, AxisValueY = 0;
		float MousePosX, MousePosY;
		int32 ScreenSizeX, ScreenSizeY;
		MainController->GetMousePosition(MousePosX, MousePosY);
		MainController->GetViewportSize(ScreenSizeX, ScreenSizeY);

		if (MousePosX > InitialMousePosX)
			AxisValueX = (MousePosX - InitialMousePosX) / -200;
		else
			AxisValueX = (InitialMousePosX - MousePosX) / 200;

		if (MousePosY > InitialMousePosY)
			AxisValueY = (MousePosY - InitialMousePosY) / -200;
		else
			AxisValueY = (InitialMousePosY - MousePosY) / 200;


		// Set new rotation
		float NewYaw = GetActorRotation().Yaw + (AxisValueX * PanSensivity);
		float NewPitch = GetActorRotation().Pitch + (AxisValueY * PanSensivity);
		NewPitch = UKismetMathLibrary::FClamp(NewPitch, -25.f, 45.f);

		FRotator NewRotation = FRotator(NewPitch, NewYaw, GetActorRotation().Roll);
		SetActorRotation(NewRotation);
	}
}

void ACamera::ZoomIn()
{
	SpringArm->TargetArmLength = UKismetMathLibrary::FClamp(SpringArm->TargetArmLength - ZoomSensivity, MinArmDistance, MaxArmDistance);
}

void ACamera::ZoomOut()
{
	SpringArm->TargetArmLength = UKismetMathLibrary::FClamp(SpringArm->TargetArmLength + ZoomSensivity, MinArmDistance, MaxArmDistance);
}

void ACamera::ShiftPressed()
{
	FastMoveMultiplier *= 2;
}

void ACamera::ShiftReleased()
{
	FastMoveMultiplier /= 2;
}

void ACamera::ScreenEdgeMovement()
{
	int32 ScreenSizeX, ScreenSizeY;
	MainController->GetViewportSize(ScreenSizeX, ScreenSizeY);

	float MousePosX, MousePosY;
	MainController->GetMousePosition(MousePosX, MousePosY);

	if ((MousePosX / ScreenSizeX) >= 0.975f)
		EdgeMoveSpeedX = 5;
	else if ((MousePosX / ScreenSizeX) <= 0.025f)
		EdgeMoveSpeedX = -5;
	else
		EdgeMoveSpeedX = 0;

	if ((MousePosY / ScreenSizeY) >= 0.975f)
		EdgeMoveSpeedY = -5;
	else if ((MousePosY / ScreenSizeY) <= 0.025f)
		EdgeMoveSpeedY = 5;
	else
		EdgeMoveSpeedY = 0;

	FVector DeltaLocation = FVector(FastMoveMultiplier*EdgeMoveSpeedY, FastMoveMultiplier*EdgeMoveSpeedX, 0);
	AddActorLocalOffset(DeltaLocation);
}
