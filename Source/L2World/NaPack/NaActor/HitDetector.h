#pragma once

#include "CoreMinimal.h"
#include "../NaGlobalHeader.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitDetector.generated.h"

class UBoxComponent;
struct FHitResult;

/* Hit Detector is an actor class which detects overlap hit with other actors.
* Designed for combats that need to detect hit of overlap objects e.g. weapon.
* Hit Detector itself isn't encoded any function of detection itself, but only virtual hit detection report functions. 
* Hit detection should be implemented in inheriting classes.
*/
UCLASS()
class NAPACK_API AHitDetector :public AActor {

	GENERATED_BODY()

protected:

	void CalledOnHitDetected(const FHitResult & HitResult);

public: 

	// C++ actions when hit is detected
	virtual void OnHitDetected(const FHitResult & HitResult) {};

	// Actions when hit is detected
	UFUNCTION(BlueprintNativeEvent, DisplayName = "On Hit Detected")
	void OnHitDetected_BP(const FHitResult & HitResult);
	void OnHitDetected_BP_Implementation(const FHitResult & HitResult) {};

};

UCLASS(BlueprintType)
class NAPACK_API ABoxHitDetector :public AHitDetector {

	GENERATED_BODY()

public:

	ABoxHitDetector();

	virtual void OnConstruction(const FTransform& trans) override;

	virtual void Tick(float dt) override;

protected:

	virtual void BeginPlay() override;

	/* Components */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* TemplateBox;

	/* Hit detection */

	// Main detection function by box sweeping
	void Tick_DetectHit(float dt, TArray<FHitResult> & HitResult);

	TArray<AActor*> IgnoreActors;

	FVector LastLocation;

	TArray<FHitResult> HitResultTemp;

public:

	// If false, it will stop tracing, returning no hit rusult. This value can be set in runtime.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector")
	bool bOpened = true;

	// If true, the hit event will generate only once for an actor, unless the actor is manually retrieved.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector")
	bool bHitOnlyOnce = true;

	// Whether the hit detection will trace complex collision.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector")
	bool bTraceComplex = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector")
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UFUNCTION(BlueprintCallable)
	void AddIgnore(AActor* Ignore);

	UFUNCTION(BlueprintCallable)
	void AddIgnoreMulti(const TArray<AActor*> Ignore);

	UFUNCTION(BlueprintCallable)
	void Retrieve(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	void RetrieveMulti(const TArray<AActor*> InActorSet);

	/* Debug options */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector|DebugOptions")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector|DebugOptions")
	FLinearColor TraceColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector|DebugOptions")
	FLinearColor TraceHitColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitDetector|DebugOptions")
	float DrawTime = 5.0f;




};