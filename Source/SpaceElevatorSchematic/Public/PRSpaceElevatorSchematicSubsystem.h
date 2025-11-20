#pragma once

#include "CoreMinimal.h"
#include "FGBuildableSpaceElevator.h"
#include "Subsystem/ModSubsystem.h"
#include "FGSchematicManager.h"
#include "PRSpaceElevatorSchematicSubsystem.generated.h"

// It would rule if this file would work

/**
 Subsystem taken from AniViRus's resource sink subsystem.
 Stores schematic manager system and modifies behaviour of Space Elevator to make it fill milestones at inputs.
 */
UCLASS()
class SPACEELEVATORSCHEMATIC_API APRSpaceElevatorSchematicSubsystem : public AModSubsystem
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
protected:
	bool hooksInitialized = false;
	FDelegateHandle hookHandler;
	UPROPERTY()
	AFGSchematicManager* mCachedSchematicManagerSubsystem;
};
