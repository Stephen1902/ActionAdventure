// Copyright 2023 DME Games - Made for the GameDev.TV Unreal Action Adventure Course


#include "InteractiveBase.h"

#include "ActionAdventureCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AInteractiveBase::AInteractiveBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Comp"));
	SetRootComponent(SphereComp);

	HandleMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Handle Comp"));
	HandleMeshComp->SetupAttachment(RootComponent);
	HandleMeshComp->CanCharacterStepUpOn = ECB_No;
	HandleMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	HandleMeshComp->SetGenerateOverlapEvents(false);

	FlameMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Flame Comp"));
	FlameMeshComp->SetupAttachment(HandleMeshComp);
	FlameMeshComp->CanCharacterStepUpOn = ECB_No;
	FlameMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	FlameMeshComp->SetGenerateOverlapEvents(false);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh Comp"));
	SkeletalMeshComp->SetupAttachment(RootComponent);
	SkeletalMeshComp->CanCharacterStepUpOn = ECB_No;
	SkeletalMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SkeletalMeshComp->SetGenerateOverlapEvents(false);

	ParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Comp"));
	ParticleSystemComp->SetupAttachment(HandleMeshComp);
	ParticleSystemComp->SetAutoActivate(true);

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Comp"));
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComp->SetDrawSize(FVector2D(300.f, 150.f));
	WidgetComp->SetupAttachment(FlameMeshComp);
	WidgetComp->SetRelativeLocation(FVector(0.f, 0.f, -170.f));
	WidgetComp->SetVisibility(false);
	
}

// Called when the game starts or when spawned
void AInteractiveBase::BeginPlay()
{
	Super::BeginPlay();

	if (bIsInteractive)
	{
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AInteractiveBase::OnBeginOverlap);
		SphereComp->OnComponentEndOverlap.AddDynamic(this, &AInteractiveBase::OnEndOverlap);
	}

	if (bStartParticlesActivated)
	{
		ParticleSystemComp->Activate();
	}
}

void AInteractiveBase::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(0, 2.0f, FColor::Green, TEXT("Overlap Begin"));
	if (WidgetComp->GetWidget())
	{
		WidgetComp->SetVisibility(true);
	}
	
	if (const TObjectPtr<AActionAdventureCharacter> PlayerCharacter = Cast<AActionAdventureCharacter>(OtherActor))
	{
		PlayerCharacter->SetCanInteract(this, true);
	}
	

}

void AInteractiveBase::OnEndOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (WidgetComp->GetWidget())
	{
		WidgetComp->SetVisibility(false);
	}
	
	if (const TObjectPtr<AActionAdventureCharacter> PlayerCharacter = Cast<AActionAdventureCharacter>(OtherActor))
	{
		PlayerCharacter->SetCanInteract(nullptr, false);
	}
}

// Called every frame
void AInteractiveBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

