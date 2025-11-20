#pragma once

#include "CoreMinimal.h"
#include "FGBuildableSpaceElevator.h"
#include "Subsystem/ModSubsystem.h"
#include "FGSchematicManager.h"

/**
 Subsystem taken from AniViRus's resource sink subsystem.
 Stores schematic manager system and modifies behaviour of Space Elevator to make it fill milestones at inputs.
 */
UCLASS()
class SPACEELEVATORSCHEMATIC_API PRSpaceElevatorSchematicSubsystem : public AModSubsystem
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
protected:
	bool hooksInitialized = false;
	FDelegateHandle hookHandler;
	UFUNCTION(BlueprintImplementableEvent)
	UPROPERTY()
	AFGSchematicManager* mCachedSchematicManagerSubsystem;
};
