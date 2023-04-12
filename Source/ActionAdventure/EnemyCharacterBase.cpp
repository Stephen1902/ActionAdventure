// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course


#include "EnemyCharacterBase.h"

#include "ActionAdventureCharacter.h"
#include "EnemyAIControllerBase.h"
#include "TimerManager.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
/*
	if (TargetPoints.Num() > 0)
	{
		
	}
*/
	EnemyController = Cast<AEnemyAIControllerBase>(this->GetController());
/*	if (EnemyController && TargetPoints.Num() > 0)
	{
		TimerDelegate.BindUFunction(this, "MoveToNextPoint", TargetPointID);
		MoveToNextPoint(0);
	}
*/
}


// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AEnemyCharacterBase::MoveToNextPoint(int32 TargetID)
{
	TargetPointID = TargetID;
	if (const ATargetPoint* CurrentTarget = Cast<ATargetPoint>(TargetPoints[TargetPointID]))
	{
		const FVector TargetPointLocation = CurrentTarget->GetActorLocation();
		//this->MoveToLocation(TargetPointLocation);

		// Increment the target point to send the AI to the next one
		TargetPointID++;

		if (TargetPointID == TargetPoints.Num())
		{
			TargetPointID = 0;
		}
		
		GetWorldTimerManager().SetTimer(MoveTimerHandle, TimerDelegate, 2.0f, false);
	}
}
/*
void AEnemyCharacterBase::MoveToPlayerCharacter()
{

}
*/
