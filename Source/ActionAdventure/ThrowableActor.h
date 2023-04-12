// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ThrowableActor.generated.h"

UCLASS()
class ACTIONADVENTURE_API AThrowableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AThrowableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set up")
	TObjectPtr<UStaticMeshComponent> MeshComp;

	// Sound to play when this item hits something
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set up")
	TObjectPtr<USoundBase> SoundOnHit;
	
	UFUNCTION()
	void ActorHasHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Speed at which to launch the throwable when it is spawned
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Throwable")
	int32 SpeedToThrow = 1000.f;

	// Called by the actor that spawned this to launch the throwable
	void LaunchOnSpawn(TObjectPtr<class AActionAdventureCharacter> ActorThatSpawned);  
private:
	// A check to ensure the sound only plays once when hit
	bool bHasHit = false;


};
