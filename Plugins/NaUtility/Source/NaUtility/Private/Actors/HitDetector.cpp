#include "Actors/HitDetector.h"
#include "Components/BoxComponent.h"

/* Hit detector interface */

void AHitDetectorInterface::BeginPlay() {
	Super::BeginPlay();


}

void AHitDetectorInterface::CalledOnHitDetected(const FHitResult & HitResult) {
	OnHitDetected(HitResult);
	OnHitDetected_BP(HitResult);
}

void AHitDetectorInterface::AddIgnore(AActor* Ignore) {
	IgnoreActors.Emplace(Ignore);
}

void AHitDetectorInterface::AddIgnoreMulti(const TArray<AActor*> Ignore) {
	IgnoreActors.Append(Ignore);
}

void AHitDetectorInterface::ResumeIgnored(AActor* InActor) {
	IgnoreActors.Remove(InActor);
}

void AHitDetectorInterface::ResumeIgnoredMulti(const TArray<AActor*> InActorSet) {
	for (auto & InActor : InActorSet) {
		ResumeIgnored(InActor);
	}
}

// General templated tick detection implementation
void AHitDetectorInterface::Tick(float dt) {
	
	Super::Tick(dt);

	// Clear array first
	HitResultTemp.Empty();

	if (bOpened) {
		// Custom implementation of hit detection
		Tick_DetectHit(dt, HitResultTemp);
		

		// Generate OnHitDetected events
		AActor* HitActorTemp = nullptr;

		for (int i = 0; i < HitResultTemp.Num(); ++i) {

			HitActorTemp = HitResultTemp[i].GetActor();
			check(HitActorTemp);

			if (!IgnoreActors.Contains(HitActorTemp)) {
				UE_LOG(LogTemp, Log, TEXT("HitDetected: %s"), *AActor::GetDebugName(HitActorTemp));
				// Generate OnHitDetected event and add to ignore list
				OnHitDetected(HitResultTemp[i]);
				OnHitDetected_BP(HitResultTemp[i]);
				if (OnHitDetected_Delegate.IsBound())
					OnHitDetected_Delegate.ExecuteIfBound(HitResultTemp[i]);

				IgnoreActors.Emplace(HitActorTemp);
			}
		}
	}

}


/* Box hit detector */

ABoxHitDetector::ABoxHitDetector(){

	PrimaryActorTick.bCanEverTick = true;

	RootComponent = SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	TemplateBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TemplateBox"));
	TemplateBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	
}

void ABoxHitDetector::OnConstruction(const FTransform& trans) {

	Super::OnConstruction(trans);

	// As the box component is just a template, disable its collision
	TemplateBox->SetCollisionProfileName(TEXT("NoCollision"));
	LastLocation = GetActorLocation();

}

void ABoxHitDetector::BeginPlay() {

	Super::BeginPlay();


}

void ABoxHitDetector::Tick(float dt) {
	
	Super::Tick(dt);

	
}


void ABoxHitDetector::Tick_DetectHit_BoxHitDetector(float DeltaTime, TArray<FHitResult> & HitResult) {

	if(!bOpened)
		bIsLastLocationInitialized = false;
	else if (bIsLastLocationInitialized == false) {
		LastLocation = TemplateBox->GetComponentLocation();
		bIsLastLocationInitialized = true;
	}

	FVector ThisLocation = TemplateBox->GetComponentLocation();
	if (bIsLastLocationInitialized&&(!bIsFirstTick))
		UKismetSystemLibrary::BoxTraceMulti(this, LastLocation, ThisLocation, TemplateBox->GetScaledBoxExtent(), TemplateBox->GetComponentRotation(), TraceChannel, bTraceComplex, IgnoreActors, DrawDebugType, HitResult, true, TraceColor, TraceHitColor, DrawTime);

	LastLocation = ThisLocation;
	
	bIsFirstTick = false;
}


