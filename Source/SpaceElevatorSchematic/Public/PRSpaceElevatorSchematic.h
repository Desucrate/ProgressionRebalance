#pragma once

#include "CoreMinimal.h"
#include "FGBuildable.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PRSpaceElevatorSchematic.generated.h"

/**
 * Helper to expose some Buildable properties to blueprints
 */
UCLASS()
class SPACEELEVATORSCHEMATIC_API PRSpaceElevatorSchematic : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static bool BuildableIsDismantled(AFGBuildable* buildable);
};
