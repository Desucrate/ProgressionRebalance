#include "PRSpaceElevatorSchematicSubsystem.h"
#include "SpaceElevatorSchematic.h"
#include "Logging/StructuredLog.h"
#include "FGInventoryComponent.h"
#include "SubsystemActorManager.h"
#include "Patching/NativeHookManager.h"
#include "FGFactoryConnectionComponent.h"

void PRSpaceElevatorSchematicSubsystem::BeginPlay()
{
	Super::BeginPlay();
	if (hooksInitialized) return;
	mCachedSchematicManagerSubsystem = AFGSchematicManager::Get(this);
	//Hooking
	AFGBuildableSpaceElevator* SpaceElevator = GetMutableDefault<AFGBuildableSpaceElevator>();
	hookHandler = SUBSCRIBE_METHOD_VIRTUAL_AFTER(AFGBuildableSpaceElevator::Factory_Tick, SpaceElevator, [this](AFGBuildableSpaceElevator* self, float dt)
		{
			if (!HasAuthority()) return;
			
			TArray<FInventoryItem> items;
			self->ForEachComponent<UFGFactoryConnectionComponent>(true, [&](UFGFactoryConnectionComponent* FactoryConn) {
				if (FactoryConn->IsConnected() && FactoryConn->Factory_PeekOutput(items)) {
					while (!items.IsEmpty()) {
						
						if (mCachedSchematicManagerSubsystem->PayOffOnSchematic(items[0].GetItemClass())) {
							float offset;
							FInventoryItem item;
							FactoryConn->Factory_GrabOutput(item, offset);
							items.RemoveAt(0);
							UE_LOGFMT(LogSpaceElevatorSchematic, Verbose, "Item: {Name}", item.GetItemClass().GetDefaultObject()->mDisplayName.ToString());
						}
						else break;
					}
				}
				});
		});
	//~Hooking
	hooksInitialized = true;
}

void PRSpaceElevatorSchematicSubsystem::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	if (hookHandler.IsValid()) UNSUBSCRIBE_METHOD(AFGBuildableSpaceElevator::Factory_Tick, hookHandler);
}