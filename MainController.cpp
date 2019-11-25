// Fill out your copyright notice in the Description page of Project Settings.

#include "MainController.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

#define LANDSCAPE ECC_GameTraceChannel1

AMainController::AMainController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMainController::BeginPlay()
{
	Super::BeginPlay();

	// Create a reference to World
	World = GetWorld();
	if (!World)
		UE_LOG(LogTemp, Fatal, TEXT("Pointer to world has failed in CameraPawnController"));
}

void AMainController::Tick(float DeltaTime)
{
	
}

void AMainController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Bind actions for mouse events
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMainController::OnLeftClick);
}

void AMainController::OnLeftClick()
{
	// Stuff to do on left click
}