// Fill out your copyright notice in the Description page of Project Settings.


#include "ShowPawn_Base.h"

#include "Camera/CameraActor.h"

// Sets default values
AShowPawn_Base::AShowPawn_Base()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	IsTurn = false;
	bFindCameraComponentWhenViewTarget = true;
	LockX = false;
	LockY = false;
	IsAutoTurnCamera = false;
	TurnSpeed = 0;
	PressCtrl = false;
	PressAlt = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	OtherCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OtherCamera"));
	ArrowOrignLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("OrginLocation"));
	ArrowRShouldLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("RshouldLocation"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->TargetArmLength = 1000.0f;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		Mesh->SetStaticMesh(SphereVisualAsset.Object);
		Mesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Mesh->SetWorldScale3D(FVector(1.f));
	}

	ArrowRShouldLocation->SetupAttachment(Mesh);
	ArrowOrignLocation->SetupAttachment(Camera);
	SpringArmComponent->SetupAttachment(Mesh);
	Camera->SetupAttachment(SpringArmComponent);
	OtherCamera->SetupAttachment(Mesh);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	Camera->SetActive(true);
	OtherCamera->SetActive(false);
	
	

}

// Called when the game starts or when spawned
void AShowPawn_Base::BeginPlay()
{
	Super::BeginPlay();
	//TurnCamera();
}

// Called every frame
void AShowPawn_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AutoTurnCamera();
}

// Called to bind functionality to input
void AShowPawn_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("TurnX", this, &AShowPawn_Base::TurnCameraX);
	PlayerInputComponent->BindAxis("TurnY", this, &AShowPawn_Base::TurnCameraY);
	PlayerInputComponent->BindAction("Turn", IE_Pressed, this, &AShowPawn_Base::OnTurn);
	PlayerInputComponent->BindAction("Turn", IE_Released, this, &AShowPawn_Base::OffTurn);
	PlayerInputComponent->BindAction("Scale", IE_Pressed, this, &AShowPawn_Base::OnAlt);
	PlayerInputComponent->BindAction("Scale", IE_Released, this, &AShowPawn_Base::OffAlt);
	PlayerInputComponent->BindAction("OffSet", IE_Pressed, this, &AShowPawn_Base::OnCtrl);
	PlayerInputComponent->BindAction("OffSet", IE_Released, this, &AShowPawn_Base::OffCtrl);
	PlayerInputComponent->BindAction(EKeys::Q.GetFName(), IE_Released, this, &AShowPawn_Base::TurnCamera);
}

void AShowPawn_Base::TurnCameraX(float x)
{
	if(LockX)
	{
		return;
	}
	if(IsTurn && !PressAlt && !PressCtrl){
		/*FRotator Rotator = SpringArmComponent->GetRelativeRotation();
		Rotator.Add(0, x, 0);
		SpringArmComponent->SetRelativeRotation(Rotator);*/

		/*FRotator Rotator = SpringArmComponent->GetRelativeRotation();
		Rotator.Yaw += x;
		SpringArmComponent->SetRelativeRotation(Rotator);*/

		SpringArmComponent->AddRelativeRotation(FQuat(FVector::UpVector, x * PI / 180));
		UE_LOG(LogTemp, Log, TEXT("x轴旋转%f度"), x);
	}else if(IsTurn && PressAlt && !PressCtrl)
	{
		//缩放
		SpringArmComponent->TargetArmLength += x*10;
	}else if(IsTurn && !PressAlt && PressCtrl)
	{
		//位移
		AddActorLocalOffset(FVector(x*10, 0, 0));
	}
}

void AShowPawn_Base::TurnCameraY( float y)
{
	if (LockY)
	{
		return;
	}
	if (IsTurn && !PressAlt && !PressCtrl) {
		/*FRotator Rotator = SpringArmComponent->GetRelativeRotation();
		Rotator.Add(y, 0, 0);
		SpringArmComponent->SetRelativeRotation(Rotator);*/

		/*FRotator Rotator = SpringArmComponent->GetRelativeRotation();
		Rotator.Pitch += y;
		SpringArmComponent->SetRelativeRotation(Rotator);*/

		//SpringArmComponent->AddRelativeRotation(FQuat(FVector::LeftVector,y*PI/180));
		
		
		FQuat Quat = SpringArmComponent->GetRelativeRotation().Quaternion() * FQuat(FVector::LeftVector, y * PI / 180);
		SpringArmComponent->SetRelativeRotation(Quat);
		UE_LOG(LogTemp, Log, TEXT("y轴旋转%f度"), y);
	}
	else if (IsTurn && PressAlt && !PressCtrl)
	{
		//缩放
		SpringArmComponent->TargetArmLength += y*10;
	}
	else if (IsTurn && !PressAlt && PressCtrl)
	{
		//位移
		AddActorLocalOffset(FVector(0, y*10, 0));
	}
}


void AShowPawn_Base::OnTurn()
{
	IsTurn = true;
}

void AShowPawn_Base::OffTurn()
{
	IsTurn = false;
	
}

void AShowPawn_Base::TurnCamera()
{
	/*AActor* Actor = (AActor*)CameraActor;
	if(!Actor)
	{
		return;
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetViewTargetWithBlend(Actor, 3.f);*/
	//OtherCamera->SetActive(true);
	//Camera->SetActive(false);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	PlayerController->SetViewTargetWithBlend(this, 3);
	ACameraActor* CameraActorz = (GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass())) ;
	CameraActorz->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	CameraActorz->SetActorRelativeTransform(OtherCamera->GetRelativeTransform());
	PlayerController->SetViewTargetWithBlend(CameraActorz, 3.f);
	
}

void AShowPawn_Base::AutoTurnCamera()
{
	if(IsAutoTurnCamera)
		SpringArmComponent->AddRelativeRotation(FQuat(FVector::UpVector, TurnSpeed * PI / 180));
}

void AShowPawn_Base::OnCtrl()
{
	PressCtrl = true;
}

void AShowPawn_Base::OffCtrl()
{
	PressCtrl = false;
}

void AShowPawn_Base::OnAlt()
{
	PressAlt = true;
}

void AShowPawn_Base::OffAlt()
{
	PressAlt = false;
}



