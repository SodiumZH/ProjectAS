#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NaMobSkillCollision.generated.h"

/* Mob Skill Collision is an interface of hit detector applied on skills. */

class ANaMob;
class ANaMobSkill;
class USceneComponent;
class UShapeComponent;
class UBoxComponent;
class UCapsuleComponent;
class USphereComponent;
class UPrimitiveComponent;
class UStaticMeshComponent;
class UStaticMesh;
class UChildActorComponent;
class AHitDetectorInterface;

UENUM()
enum class ESkillCollisionShape :uint8 {
	SCS_Sphere	UMETA(DisplayName = "Sphere"),
	SCS_Capsule	UMETA(DisplayName = "Capsule"),
	SCS_Box 	UMETA(DisplayName = "Box"),
	SCS_StaticMesh UMETA(DisplayName = "StaticMesh")
};

UCLASS(BlueprintType)
class ANaMobSkillCollision :public AActor {

	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	ANaMobSkillCollision();

	// Construction script
	virtual void OnConstruction(const FTransform & trans) override;

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	/* On skill collision initialized.
	* Use this instead of begin play in mob skill collision, since begin play will cause initialization issues.
	* Will be called on the first frame, after a tick delta time from begin play.
	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Initialized (Skill Collision)"), Category = "Natrium|MobSystem|SkillCollision")
	void Initialized();
	void Initialized_Implementation() {};

protected:

	void SetupSendHitDelegate();



protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UChildActorComponent* HitDetectorSpawner;

	ANaMobSkill* SourceSkill;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "MobSkillCollision")
	FName SocketName;


public:

	/************** Get functions *****************/

	AHitDetectorInterface* GetDetector();
	FORCEINLINE ANaMobSkill* GetSourceSkill() { return SourceSkill; };
	FORCEINLINE FName GetSocketName() { return SocketName; };

	/* Generating */

public:

	/** Make a collision from a skill.
	* @Param SourceSkill The skill owning this collision.
	* @Param Class Applied skill collision class.
	* @Param InTransform Relative transform.
	* @Param AttachToComponent Component this collision should attach. If this param is left null, skill will attach to the root component.
	* @Param SocketName Socket of attachment of this skill.
	* @Param DoAttachment If set false, the skill actor will not attach to anything and generate with world transform.
	*/
	static ANaMobSkillCollision* MakeCollisionByClass(
		ANaMobSkill* SourceSkill,
		TSubclassOf<ANaMobSkillCollision> Class,
		const FTransform & InTranform,
		USceneComponent* AttachToComponent = nullptr,
		FName Socket = NAME_None,
		bool DoAttachment = true
	);

	void Destroyed() override;

	/* Collision detecting */

	// If true, the collision will not detect collision to the mob owning it (i.e. source mob of the source mob skill)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MobSkillCollision")
	bool bIgnoreSourceMob = true;




	/* Notify */

	// Send hit event to source skill
	UFUNCTION()
	void SendSkillHit(const FHitResult& HitResult);

	UFUNCTION(BlueprintNativeEvent, Category = "Natrium|MobSystem|SkillCollision")
	void OnSkillHit(const FHitResult& HitResult);
	void OnSkillHit_Implementation(const FHitResult& HitResult){};
	

};

