// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/PostProcessComponent.h"
#include "PostProcessVolumeActor.generated.h"

UCLASS()
class CUSTOMCOMPONENTS_UE4_API APostProcessVolumeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APostProcessVolumeActor();
	APostProcessVolumeActor(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PostProcessVolume)
	UBoxComponent* BoxCollision;	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PostProcessVolume)
	UPostProcessComponent* PostProcessComponent;
};
