#include "HitDetector.h"
#include "Components/BoxComponent.h"


void AHitDetector::CalledOnHitDetected(const FHitResult & HitResult) {
	OnHitDetected(HitResult);
	OnHitDetected_BP(HitResult);
}


ABoxHitDetector::ABoxHitDetector(){

	PrimaryActorTick.bCanEverTick = true;

	TemplateBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TemplateBox"));
	RootComponent = TemplateBox;
	
	
}

void ABoxHitDetector::OnConstruction(const FTransform& trans) {

	Super::OnConstruction(trans);

	// As the box component is just a template, disable its collision
	TemplateBox->SetCollisionProfileName(TEXT("NoCollision"));

}

void ABoxHitDetector::BeginPlay() {

	Super::BeginPlay();

	LastLocation = GetActorLocation();

}

void ABoxHitDetector::Tick(float dt) {
	Super::Tick(dt);
	Tick_DetectHit(dt, HitResultTemp);

}

void ABoxHitDetector::Tick_DetectHit(float dt, TArray<FHitResult> & HitResult) {

	FVector ThisLocation = GetActorLocation();

	if (!bOpened) {
		LastLocation = ThisLocation;
		HitResultTemp.Empty();
		return;
	}

	HitResultTemp.Empty();

	bool bHit = UKismetSystemLibrary::BoxTraceMulti(this, LastLocation, ThisLocation, TemplateBox->GetScaledBoxExtent(), TemplateBox->GetComponentRotation(), TraceChannel, bTraceComplex, IgnoreActors, DrawDebugType, HitResultTemp, true, TraceColor, TraceHitColor, DrawTime);

	LastLocation = ThisLocation;

	if (!bHit) 
		return;
	

	for (int i = 0; i < HitResult.Num(); i++) {
		if (!IgnoreActors.Contains(HitResultTemp[i].Actor.Get())) {
			CalledOnHitDetected(HitResultTemp[i]);
			IgnoreActors.Emplace(HitResultTemp[i].Actor.Get());
		}
	}

}


void ABoxHitDetector::AddIgnore(AActor* Ignore) {
	IgnoreActors.Emplace(Ignore);
}

void ABoxHitDetector::AddIgnoreMulti(const TArray<AActor*> Ignore) {
	IgnoreActors.Append(Ignore);
}

void ABoxHitDetector::Retrieve(AActor* InActor) {
	IgnoreActors.Remove(InActor);
}

void ABoxHitDetector::RetrieveMulti(const TArray<AActor*> InActorSet) {
	for (auto & InActor : InActorSet) {
		Retrieve(InActor);
	}
}