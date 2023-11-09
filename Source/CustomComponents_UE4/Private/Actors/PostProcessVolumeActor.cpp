// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PostProcessVolumeActor.h"

// Sets default values
APostProcessVolumeActor::APostProcessVolumeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 编辑器内的文件，无法修改，主要是看文件路径，只有Runtime下的可以修改
	//UCubeBuilder* CubeBuilder = Cast<UCubeBuilder>(BrushBuilder);
	//if (CubeBuilder)
	//{
	//	UE_LOG(LogTemp, Log, TEXT("APostProcessVolumeActor === Name: %s"), *CubeBuilder->GetName());
	//	CubeBuilder->Y = 600;
	//}

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(300.0f, 100.0f, 100.0f));
	BoxCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	BoxCollision->RegisterComponent();
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->AttachToComponent(BoxCollision, FAttachmentTransformRules::KeepRelativeTransform);

	PostProcessComponent->bUnbound = false;
	PostProcessComponent->Settings.bOverride_ColorContrast = true;
	PostProcessComponent->Settings.ColorContrast = FVector4(0, 1, 0, 1);
}
APostProcessVolumeActor::APostProcessVolumeActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	BoxCollision = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(200.0f, 100.0f, 100.0f));
	BoxCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->AttachToComponent(BoxCollision, FAttachmentTransformRules::KeepRelativeTransform);

	PostProcessComponent->bUnbound = false;
	PostProcessComponent->Settings.bOverride_ColorContrast = true;
	PostProcessComponent->Settings.ColorContrast = FVector4(0, 1, 0, 1);
}

// Called when the game starts or when spawned
void APostProcessVolumeActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APostProcessVolumeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

