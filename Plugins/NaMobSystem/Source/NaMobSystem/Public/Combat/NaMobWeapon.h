#pragma once

/* NaMobWeapon is the basic weapon class for NaMob.
* It can be attached to the "hand" socket(s) (specified manually) of NaMob to act as weapons.
* The root is a static mesh (visible). 
* A method of weapon collision generation is also defined.
* If it doesn't have an owner (Owner == nullptr or invalid), the root static mesh will simulate physics, enabling the weapon to drop on the ground.
*/

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NaMobSkillCollision.h"
#include "NaMobWeapon.generated.h"

class ANaMob;
class ANaMobSkill;
class ANaMobSkillCollision;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FMobWeaponCollisionSpawnInfo {

	GENERATED_USTRUCT_BODY()

public:

	// Class of collision.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ANaMobSkillCollision> Class;

	// Relative transform. To determine the transform, you can add a skill collision as child component to the root, adjust the transform, copy to this parameter and remove the collision.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform = FTransform::Identity;

	// Component the collision should attach to. If not specified, it will attach to the root component (i.e. static mesh).
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* AttachToComponent = nullptr;

	// Socket of the weapon. Works only when the collision should attach to a component which has sockets. Note: this is not the mob socket.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponSocketName = NAME_None;

};

UCLASS(BlueprintType)
class NAMOBSYSTEM_API ANaMobWeapon : public AActor {

	GENERATED_BODY()


public:

	ANaMobWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	/* Attachment */
protected:

	virtual void BeginPlay() override;

	/* On weapon initialized. 
	* Use this instead of begin play in weapon, since begin play will cause initialization issues.
	* Will be called on the first frame, after a tick delta time from begin play.
	*/
	UFUNCTION(BlueprintNativeEvent, meta = (DisplayName = "Initialized (Weapon)"), Category = "Natrium|MobSystem|Weapon")
	void Initialized();
	void Initialized_Implementation() {};
	
	ANaMob* OwnerMob;

	FName SocketName;

	FName RegisterName;

public:

	//UFUNCTION(BlueprintPure, meta = (DefaultToSelf), Category = "Natrium|MobSystem")
	inline ANaMob* GetOwnerMob() { return OwnerMob; };

	//UFUNCTION(BlueprintPure, meta = (DefaultToSelf), Category = "Natrium|MobSystem")
	inline bool HasOwner();

	inline FName GetSocketName() { return SocketName; };

	inline FName GetRegisterName() { return RegisterName; };

	/** Check if there's anything wrong with the ownership of the weapon. 
	* For example: ownerless but not simulating physics, ownered but simulating, not attached to the owner, ...
	* It will print error information to the log, and try fixing if needed.
	* Warning: This is a function for debugging. Do not depend on this to fix ownership errors in runtime!! In release it will not do anything to save resource.
	* @Param TryFixing If true, it will try fixing the problem. Generally it will set the parameters of weapon to adapt the real condition. E.g. if ownered but simulating, it will be set ownerless.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "Natrium|MobSystem|Weapon")
	void CheckOwner(bool TryFixing = false);

	/* Spawn */

public:
	/** Generate a new weapon and give it to a mob. 
	* @Param Class Class of weapon to spawn.
	* @Param Target Owner mob to give the weapon. If enables no attachment, the weapon will spawn in world transform and start to simulate physics as an "ownerless weapon".
	* @Param Transform Relative transform of the weapon. If enables no attachment, this will be the initial world transform.
	* @Param SocketName Socket name this weapon will attach to.
	* @Param RegisterName Name to register at the owner mob. This is the name for searching weapon from mob. 
	* @Param NoAttach If true, it will generate an ownerless weapon without attaching to any mob. A valid mob "Target" is still required as world context.
	@ @ReturnValue The weapon object generated.
	*/
	UFUNCTION(BlueprintCallable, meta = (Keywords = "add give spawn generate pick equip"), Category = "Natrium|MobSystem|Weapon")
	static ANaMobWeapon* AddNewWeapon(TSubclassOf<ANaMobWeapon> Class, ANaMob* Target, const FTransform & Transform, FName Socket = NAME_None, FName RegisterName = NAME_None, bool NoAttachment = false);

	/** Give an existing weapon to a mob. 
	* @Param Weapon weapon to give.
	* @Param Target Mob the weapon will give to.
	* @Param SocketName SocketName Socket name this weapon will attach to.
	* @Param RegisterName Name to register at the owner mob. This is the name for searching weapon from mob.
	* @Param ForceGive If true, it will transfer owner if the weapon already has an owner. Warning: this action have potential risk of unexpected behaviors in the previous owner due to loss of ownership of the weapon.
	* If false, it will not do anything if the weapon already has an owner.
	*/
	
	UFUNCTION(BlueprintCallable, meta = (Keywords = "add give spawn generate pick equip"), Category = "Natrium|MobSystem|Weapon")
	static void GiveWeapon(ANaMobWeapon* Weapon, ANaMob* Target, FName SocketName = NAME_None, FName RegisterName = NAME_None, bool ForceGive = false);

	/** Make a weapon ownerless and drop it on the ground by starting simulating physics. Warning: this action have potential risk of
	* unexpected behaviors in the previous owner due to loss of ownership of the weapon.
	* If the weapon is already ownerless, this function will not do anything.
	*/
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf, Keywords = "drop throw remove delete"), Category = "Natrium|MobSystem|Weapon")
	void DropWeapon();

	/* Weapon Collision */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MobWeapon|Collision")
	FMobWeaponCollisionSpawnInfo CollisionSpawnInfo;

	// Make weapon collision from a skill. Parameters of the collision are defined in the param "Collision Spawn Info". Set that param to change the collision spawning configs.
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf), Category = "Natrium|MobSystem|Weapon")
	ANaMobSkillCollision* MakeWeaponCollision(ANaMobSkill* SourceSkill);

};