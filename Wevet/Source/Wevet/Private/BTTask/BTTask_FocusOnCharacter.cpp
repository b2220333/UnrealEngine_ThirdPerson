// Copyright © 2018 wevet works All Rights Reserved.

#include "BTTask_FocusOnCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "AIControllerBase.h"
#include "AICharacterBase.h"
#include "Engine.h"

EBTNodeResult::Type UBTTask_FocusOnCharacter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bool bSuccess = false;
	if (AAIControllerBase* AIController = Cast<AAIControllerBase>(OwnerComp.GetAIOwner()))
	{
		if (AIController->GetAICharacter() && AIController->GetAICharacter()->GetTargetCharacter())
		{
			auto Controller = AIController->GetAICharacter()->GetTargetCharacter()->Controller;
			AIController->SetFocus(Controller);
			bSuccess = true;
		}

	}
	return bSuccess ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

