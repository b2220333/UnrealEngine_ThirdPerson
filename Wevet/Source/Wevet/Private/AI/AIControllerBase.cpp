// Copyright 2018 wevet works All Rights Reserved.
#include "AIControllerBase.h"
#include "MockCharacter.h"
#include "AICharacterBase.h"
#include "WayPointBase.h"

#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"


AAIControllerBase::AAIControllerBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), 
	BotTypeKeyName(FName(TEXT("BotType"))),
	CanSeePlayerKeyName(FName(TEXT("CanSeePlayer"))),
	CanHearPlayerKeyName(FName(TEXT("CanHearPlayer"))),
	TargetEnemyKeyName(FName(TEXT("TargetEnemy"))),
	PatrolLocationKeyName(FName(TEXT("PatrolLocation")))
{

	BehaviorTreeComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
	BlackboardComponent   = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
	AIPerceptionComponent = ObjectInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("AIPerceptionComponent"));

	SightConfig = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Sight>(this, TEXT("SightConfig"));
	SightConfig->SightRadius = 3000.f;
	SightConfig->LoseSightRadius = 3500.f;
	SightConfig->PeripheralVisionAngleDegrees = 180.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies    = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals   = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	//HearConfig = ObjectInitializer.CreateDefaultSubobject<UAISenseConfig_Hearing>(this, TEXT("HearConfig"));
	//HearConfig->HearingRange = 1000.f;
	//HearConfig->DetectionByAffiliation.bDetectEnemies    = true;
	//HearConfig->DetectionByAffiliation.bDetectFriendlies = true;
	//HearConfig->DetectionByAffiliation.bDetectNeutrals   = true;
	//AIPerceptionComponent->ConfigureSense(*HearConfig);
	//AIPerceptionComponent->SetDominantSense(HearConfig->GetSenseImplementation());
}

void AAIControllerBase::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	AICharacterOwner = Cast<AAICharacterBase>(Pawn);
	if (AICharacterOwner)
	{
		if (AICharacterOwner->BehaviorTree->BlackboardAsset)
		{
			BlackboardComponent->InitializeBlackboard(*AICharacterOwner->BehaviorTree->BlackboardAsset);
		}

		AICharacterOwner->InitializePosses();
		AICharacterOwner->CreateWayPointList(WayPointList);
		BehaviorTreeComponent->StartTree(*AICharacterOwner->BehaviorTree);
	}
}

void AAIControllerBase::UnPossess()
{
	Super::UnPossess();
	BehaviorTreeComponent->StopTree();
}

FGenericTeamId AAIControllerBase::GetGenericTeamId() const
{
	return PTG_TEAM_ID_ENEMY;
}

void AAIControllerBase::Patrolling_Implementation() 
{
}

void AAIControllerBase::CheckEnemySighting_Implementation() 
{
}

void AAIControllerBase::Hunting_Implementation() 
{
}

AWayPointBase* AAIControllerBase::GetRandomAtWayPoint()
{
	if (WayPointList.Num() <= 0)
	{
		return nullptr;
	}
	const int32 RandomIndex = FMath::RandRange(0, WayPointList.Num() - 1);
	return WayPointList[RandomIndex];
}

void AAIControllerBase::SetTargetEnemy(APawn * NewTarget)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsObject(TargetEnemyKeyName, NewTarget);
	}
}

void AAIControllerBase::SetWayPoint(AWayPointBase* NewWayPoint)
{
	if (BlackboardComponent)
	{
		//@TODO
		//BlackboardComponent->SetValueAsObject(TargetEnemyKeyName, NewWayPoint);
	}
}

void AAIControllerBase::SetBlackboardBotType(EBotBehaviorType NewType)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsEnum(BotTypeKeyName, (uint8)NewType);
	}
}

void AAIControllerBase::SetBlackboardSeeActor(const bool NewCanSeeActor)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsBool(CanSeePlayerKeyName, NewCanSeeActor);
	}
}

void AAIControllerBase::SetBlackboardHearActor(const bool NewCanHearActor)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsBool(CanHearPlayerKeyName, NewCanHearActor);
	}
}

void AAIControllerBase::SetBlackboardPatrolLocation(const FVector NewLocation)
{
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsVector(PatrolLocationKeyName, NewLocation);
	}
}

void AAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
	if (ensure(AIPerceptionComponent && AIPerceptionComponent->IsValidLowLevel()))
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerBase::OnTargetPerceptionUpdatedRecieve);
	}
}

void AAIControllerBase::OnTargetPerceptionUpdatedRecieve(AActor* Actor, FAIStimulus Stimulus)
{
	if (AICharacterOwner == nullptr)
	{
		return;
	}

	if (ICombatExecuter::Execute_IsDeath(AICharacterOwner) || AICharacterOwner->GetTargetCharacter())
	{
		return;
	}

	if (AMockCharacter* MockCharacter = Cast<AMockCharacter>(Actor))
	{
		bool bSuccess = (ICombatExecuter::Execute_IsDeath(MockCharacter)) && Stimulus.WasSuccessfullySensed() ? true : false;
		UE_LOG(LogWevetClient, Log, TEXT("WasSuccessfullySensed : %s"), bSuccess ? TEXT("True") : TEXT("false"));

	}
}


