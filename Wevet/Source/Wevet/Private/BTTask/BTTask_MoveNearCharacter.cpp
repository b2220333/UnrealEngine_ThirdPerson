// Copyright © 2018 wevet works All Rights Reserved.

#include "BTTask_MoveNearCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "AIControllerBase.h"
#include "AICharacterBase.h"
#include "Engine.h"


EBTNodeResult::Type UBTTask_MoveNearCharacter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool bSuccess = false;
	if (AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner()))
	{
		if (AIController->GetAICharacter() && AIController->GetAICharacter()->GetTargetCharacter())
		{
			const FVector Location = AIController->GetAICharacter()->GetTargetCharacter()->GetActorLocation();
			AIController->SetBlackboardPatrolLocation(Location);
			bSuccess = true;
		}
		else
		{
			const FVector Location = AIController->GetAICharacter()->GetActorLocation();
			AIController->SetBlackboardPatrolLocation(Location);
		}
	}
	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

