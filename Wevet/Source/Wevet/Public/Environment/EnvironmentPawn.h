// Copyright © 2018 wevet works All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "EnvironmentPawn.generated.h"

UCLASS()
class WEVET_API AEnvironmentPawn : public APawn
{
	GENERATED_BODY()

public:
	AEnvironmentPawn(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMeshComponent;

public:
	FORCEINLINE class UStaticMeshComponent* GetStaticMeshComponent() const
	{
		return StaticMeshComponent;
	}
};
