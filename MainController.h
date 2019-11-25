// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainController.generated.h"

/**
 *
 */
UCLASS()
class CAFE_API AMainController : public APlayerController
{
	GENERATED_BODY()

public:
	AMainController();
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;

public:
	/********** FUNCTIONS START **********/
	UFUNCTION()
		void OnLeftClick();
	/********** FUNCTIONS END **********/


	/********** PROPERTIES START **********/
	UPROPERTY()
		class UWorld* World;
	/********** PROPERTIES END **********/
};
