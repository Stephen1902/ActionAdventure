// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course


#include "PickUpCoin.h"

#include "ActionAdventureCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
APickUpCoin::APickUpCoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Root Comp"));
	SetRootComponent(CollisionComp);
	CollisionComp->SetSphereRadius(64.f);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(CollisionComp);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	TextRenderComp1 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render 1"));
	TextRenderComp1->SetupAttachment(MeshComp);
	TextRenderComp1->SetText(FText::FromString("GOLD"));

	TextRenderComp2 = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Text Render 2"));
	TextRenderComp2->SetupAttachment(MeshComp);
	TextRenderComp2->SetText(FText::FromString("GOLD"));
	
	RotatingMovementComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Movement Comp"));
	RotatingMovementComp->RotationRate = FRotator(0.f, 0.f, 1800.f);
}

// Called when the game starts or when spawned
void APickUpCoin::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APickUpCoin::OnBeginOverlap);
	CollisionComp->OnComponentEndOverlap.AddDynamic(this, &APickUpCoin::OnEndOverlap);
}

// Called every frame
void APickUpCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpCoin::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AActionAdventureCharacter* PlayerChar = Cast<AActionAdventureCharacter>(OtherActor))
	{
		PlayerChar->AddCoinToTotal(1);
		this->Destroy();
	}
	
}

void APickUpCoin::OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

