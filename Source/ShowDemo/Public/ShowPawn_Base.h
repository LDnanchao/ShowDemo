// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShowPawn_Base.generated.h"

UCLASS()
class SHOWDEMO_API AShowPawn_Base : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShowPawn_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UStaticMeshComponent* Mesh;
	//方向指示，肩部坐标标记
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UArrowComponent* ArrowRShouldLocation;
	//摄像机坐标标记
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UArrowComponent* ArrowOrignLocation;
	//摄像机
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UCameraComponent* OtherCamera;
	//弹簧组件
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	ACameraActor* CameraActor;
	bool IsTurn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool LockX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool LockY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAutoTurnCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TurnSpeed;
	bool PressCtrl;
	bool PressAlt;
public:
	void TurnCameraX(float x);
	void TurnCameraY(float y);
	void OnTurn();
	void OffTurn();
	void TurnCamera();
	void AutoTurnCamera();
	void OnCtrl();
	void OffCtrl();
	void OnAlt();
	void OffAlt();

};