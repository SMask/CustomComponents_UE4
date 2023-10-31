// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"

void AMainPlayerController::SetupInputComponent()
{
	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("End", EKeys::End));
	}

	Super::SetupInputComponent();

	InputComponent->BindAction("End", IE_Pressed, this, &AMainPlayerController::OnEndPressed);
}

void AMainPlayerController::OnEndPressed()
{
	UE_LOG(LogTemp, Log, TEXT("OnEndPressed ======================================================================="));

	TArray<AActor*> ActorArr;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("BP_Actor"), ActorArr);
	for (AActor* Actor : ActorArr)
	{
		UE_LOG(LogTemp, Log, TEXT("OnEndPressed === Name: %s"), *Actor->GetName());

		FVector Origin;
		FVector BoxExtent;
		Actor->GetActorBounds(true, Origin, BoxExtent, false);

		// 碰撞参数
		FCollisionQueryParams CollisonQueryParams(TEXT("QueryParams"), true, NULL);
		CollisonQueryParams.bTraceComplex = true;
		CollisonQueryParams.bReturnPhysicalMaterial = false;
		CollisonQueryParams.AddIgnoredActor(Actor);

		// 起始点和检测结果
		FVector BeginLoc = Actor->GetActorLocation();
		FVector EndLoc = BeginLoc + FVector::DownVector * 100000;
		FHitResult HitResult;

		UE_LOG(LogTemp, Log, TEXT("OnEndPressed === %s === BoxExtent"), *BoxExtent.ToString());
		UE_LOG(LogTemp, Log, TEXT("OnEndPressed === %s === Origin"), *Origin.ToString());
		UE_LOG(LogTemp, Log, TEXT("OnEndPressed === %s === BeginLoc"), *BeginLoc.ToString());

		// 射线检测
		GetWorld()->LineTraceSingleByChannel(HitResult, BeginLoc, EndLoc, ECollisionChannel::ECC_Visibility, CollisonQueryParams);
		if (HitResult.GetActor())
		{
			FVector HitLocation = HitResult.Location;
			FVector ResultLocation(HitLocation);
			ResultLocation.Z = HitLocation.Z + BeginLoc.Z - Origin.Z + BoxExtent.Z;

			UE_LOG(LogTemp, Log, TEXT("OnEndPressed === %s === HitLocation"), *HitLocation.ToString());
			UE_LOG(LogTemp, Log, TEXT("OnEndPressed === %s === ResultLocation"), *ResultLocation.ToString());

			Actor->SetActorLocation(ResultLocation);
		}
	}
}
