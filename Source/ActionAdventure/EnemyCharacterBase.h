// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacterBase.generated.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAIPerceptionComponent> AIPerceptionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;

	// A series of target points, placed in the world for the AI Enemy to patrol
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"), meta = (MakeEditWidget = true))
	TArray<TObjectPtr<class ATargetPoint>> TargetPoints;
	
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>&FoundActors);

	UFUNCTION(BlueprintCallable,  Category = "Gameplay")
	void SetTargetPoint(int32 TargetPointIn) { TargetPointID = TargetPointIn; }

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

	TObjectPtr<class AEnemyAIControllerBase> EnemyController = nullptr;

	FTimerHandle MoveTimerHandle;
	FTimerDelegate TimerDelegate;
};
