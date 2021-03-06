// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainLevelScriptActor.generated.h"

class ACharacterBase;
class AMockCharacter;
class AAICharacterBase;

UCLASS()
class WEVET_API AMainLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
	
public:
	AMainLevelScriptActor(const class FObjectInitializer& ObjectInitializer);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	virtual void BeginPlay() override;

protected:
	class AMockCharacter* PlayerCharacter;
	TArray<ACharacterBase*> CharacterArray;
	TArray<AAICharacterBase*> AICharacterArray;

	virtual void CreateCharacter();
	void SetPlayer();

public:
	const TArray<ACharacterBase*>& GetCharacterArray();
	const TArray<AAICharacterBase*>& GetAICharacterArray();
};
