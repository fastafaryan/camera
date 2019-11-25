// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera.generated.h"

UCLASS()
class CAFE_API ACamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		float MovementSpeedCalculation();
	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void PanPressed();
	UFUNCTION()
		void PanReleased();
	UFUNCTION()
		void Pan();
	UFUNCTION()
		void ZoomIn();
	UFUNCTION()
		void ZoomOut();
	UFUNCTION()
		void ShiftPressed();
	UFUNCTION()
		void ShiftReleased();
	UFUNCTION()
		void ScreenEdgeMovement();
private:
	UPROPERTY()
		USceneComponent* Scene;
	UPROPERTY()
		class UStaticMeshComponent* Sphere;
	UPROPERTY()
		class USpringArmComponent* SpringArm;
	UPROPERTY()
		class UCameraComponent* Camera;
	UPROPERTY()
		class AMainController* MainController;

	UPROPERTY()
		bool bIsPanning = false; // Used to track if camera panning is enabled
	UPROPERTY()
		bool bDisableCamNav = false; // Used to track if camera navigation is disabled
	UPROPERTY()
		float FastMoveMultiplier = 2; // Used to multiply speed when shift key is hold
	UPROPERTY()
		int32 PanSensivity = 1; // Used to multiply pan speed 
	UPROPERTY()
		int32 ZoomSensivity = 50; // Used to multiply zoom speed 
	UPROPERTY(EditAnywhere)
		int32 DefaultDistance = 250;
	UPROPERTY(EditAnywhere)
		int32 MaxArmDistance = 500;
	UPROPERTY(EditAnywhere)
		int32 MinArmDistance = 100;
	UPROPERTY()
		int32 EdgeMoveSpeedX = 0;
	UPROPERTY()
		int32 EdgeMoveSpeedY = 0;

	UPROPERTY()
		float InitialMousePosX;
	UPROPERTY()
		float InitialMousePosY;
};
