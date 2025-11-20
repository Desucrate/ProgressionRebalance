#include "PRSpaceElevatorSchematic.h"

bool PRSpaceElevatorSchematic::BuildableIsDismantled(AFGBuildable* buildable)
{
	return buildable->GetIsDismantled();
}
