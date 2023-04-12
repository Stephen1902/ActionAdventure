// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterBase.generated.h"

struct FAIStimulus;

UCLASS()
class ACTIONADVENTURE_API AEnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Default movement speed for this enemy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float DefaultWalkSpeed = 200.f;

	// Speed this enemy moves at when chasing the player
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float ChasingSpeed = 500.f;
	
	// A series of target points, placed in the world for the AI Enemy to patrol
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (MakeEditWidget = true))
	TArray<TObjectPtr<class ATargetPoint>> TargetPoints;

	UFUNCTION(BlueprintCallable,  Category = "Gameplay")
	void SetTargetPoint(int32 TargetPointIn) { TargetPointID = TargetPointIn; }

	UFUNCTION(BlueprintCallable, BlueprintPure,  Category = "Gameplay")
	bool GetIsChasingPlayer() { return bIsChasingPlayer; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Gameplay")
	int32 GetTargetPointID() { return TargetPointID; }
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void MoveToNextPoint(int32 TargetID);
	
	// Current Target Point the AI is trying to move to
	int32 TargetPointID = 0;

	// Whether this enemy is chasing the player or not
	bool bIsChasingPlayer = false;

	TObjectPtr<class AEnemyAIControllerBase> EnemyController = nullptr;

	FTimerHandle MoveTimerHandle;
	FTimerDelegate TimerDelegate;
};
