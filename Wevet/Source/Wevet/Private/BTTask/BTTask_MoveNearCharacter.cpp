// Copyright 2018 wevet works All Rights Reserved.

#include "BTTask/BTTask_MoveNearCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "AI/AIControllerBase.h"
#include "AI/AICharacterBase.h"
#include "Engine.h"
#include "NavigationSystem.h"
#include "Interface/AIPawnOwner.h"

UBTTask_MoveNearCharacter::UBTTask_MoveNearCharacter() : Super()
{
	NodeName = TEXT("BTTask_MoveNearCharacter");
	BlackboardKey.SelectedKeyName = FName(TEXT("Target"));
}

EBTNodeResult::Type UBTTask_MoveNearCharacter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner()))
	{
		APawn* ControlPawn = AIController->GetPawn();
		IAIPawnOwner* Interface = Cast<IAIPawnOwner>(ControlPawn);

		if (!ControlPawn || !Interface)
		{
			return EBTNodeResult::Failed;
		}

		const float SearchRadius = IAIPawnOwner::Execute_GetMeleeDistance(ControlPawn);
		FVector SearchOrigin = FVector::ZeroVector;
		if (AActor* Target = IAIPawnOwner::Execute_GetTarget(ControlPawn))
		{
			SearchOrigin = Target->GetActorLocation();
			FNavLocation ResultLocation;
			UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(AIController);
			if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(SearchOrigin, SearchRadius, ResultLocation))
			{
				AIController->SetBlackboardPatrolLocation(ResultLocation.Location);
			}
		}
	}
	return EBTNodeResult::Succeeded;
}

