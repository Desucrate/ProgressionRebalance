#include "PRSpaceElevatorSchematicSubsystem.h"
#include "SpaceElevatorSchematic.h"
#include "Logging/StructuredLog.h"
#include "FGInventoryComponent.h"
#include "SubsystemActorManager.h"
#include "Patching/NativeHookManager.h"
#include "FGFactoryConnectionComponent.h"

void APRSpaceElevatorSchematicSubsystem::BeginPlay()
{
	Super::BeginPlay();
	if (hooksInitialized) return;
	if (!WITH_EDITOR) {
		mCachedSchematicManagerSubsystem = AFGSchematicManager::Get(this);
		// Hooking
		AFGBuildableSpaceElevator* SpaceElevator = GetMutableDefault<AFGBuildableSpaceElevator>();
		// TODO remove _AFTER
		hookHandler = SUBSCRIBE_METHOD_VIRTUAL_AFTER(AFGBuildableSpaceElevator::Factory_Tick, SpaceElevator, [this](/*auto& scope, */AFGBuildableSpaceElevator* self, float dt)
			{
				if (!HasAuthority()) return;

				// Check for items
				TArray<FInventoryItem> items;
				self->ForEachComponent<UFGFactoryConnectionComponent>(true, [&](UFGFactoryConnectionComponent* FactoryConn) {
					if (FactoryConn->IsConnected() && FactoryConn->Factory_PeekOutput(items)) {
						while (!items.IsEmpty()) {

							// Init array
							FItemAmount ItemAmount(items[0].GetItemClass(), 1);
							TArray<FItemAmount> AmountArray;
							AmountArray.Emplace(ItemAmount);

							// Get Active schematic
							TSubclassOf<UFGSchematic> ActiveSchematic(mCachedSchematicManagerSubsystem->GetActiveSchematic());

							// Check that payment is valid
							if (/*!mCachedSchematicManagerSubsystem->IsSchematicPaidOff(ActiveSchematic) &&*/
								APRSpaceElevatorSchematicSubsystem::IsItemValidForSchematicPayment(ActiveSchematic, ItemAmount))
							{
								if (mCachedSchematicManagerSubsystem->PayOffOnSchematic(ActiveSchematic, AmountArray)) {
									float offset;
									FInventoryItem item;
									FactoryConn->Factory_GrabOutput(item, offset);
									items.RemoveAt(0);
									//UE_LOGFMT(LogProgressionRebalance, Verbose, "Item: {Name}", item.GetItemClass().GetDefaultObject()->mDisplayName.ToString());
								}
								else break;
							}
							else break;
						}
					}
					});
			});
		// ~Hooking
		hooksInitialized = true;
	}
}

void APRSpaceElevatorSchematicSubsystem::EndPlay(const EEndPlayReason::Type endPlayReason)
{
	if (hookHandler.IsValid()) UNSUBSCRIBE_METHOD(AFGBuildableSpaceElevator::Factory_Tick, hookHandler);
}

// I have no idea if the vanilla game has a better way of handling payment validity
bool APRSpaceElevatorSchematicSubsystem::IsItemValidForSchematicPayment(TSubclassOf<UFGSchematic> Schematic, FItemAmount ItemAmount)
{
	TArray<FItemAmount> RemainingCost(mCachedSchematicManagerSubsystem->GetRemainingCostFor(Schematic));
	for (int32 i = 0; i < RemainingCost.Num(); i++)
	{
		if (RemainingCost[i].ItemClass == ItemAmount.ItemClass && RemainingCost[i].Amount >= ItemAmount.Amount)
		{
			return true;
		}
	}
	UE_LOGFMT(LogProgressionRebalance, VeryVerbose, "Space Elevator input failed validity check");
	return false;
}