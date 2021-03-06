// Copyright 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ReloadAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class WEVET_API UReloadAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UReloadAnimNotify(const FObjectInitializer& ObjectInitializer);

protected:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
