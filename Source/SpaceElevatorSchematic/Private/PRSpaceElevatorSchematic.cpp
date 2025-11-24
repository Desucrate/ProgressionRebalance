#include "PRSpaceElevatorSchematic.h"

bool UPRSpaceElevatorSchematic::BuildableIsDismantled(AFGBuildable* buildable)
{
	return buildable->GetIsDismantled();
}
