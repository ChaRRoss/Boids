// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Boid.generated.h"


UCLASS()
class BOIDS_API ABoid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoid();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		FVector Position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		FVector WorldBoundsMin = FVector(-250, -250, 0); // Min world bounds for boid position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		FVector WorldBoundsMax = FVector(250, 250, 0);  // Max world bounds for boid position

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		class USphereComponent* DetectionRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		class UStaticMeshComponent* VisualMesh;


	

	// Boid settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		float VisualRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		float MinDistance; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		float SpeedLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		float CenteringFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		float AvoidFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		float MatchingFactor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		float TurnFactor; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		float FleeFactor; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boid")
		float Margin;
	


	//Boid Algorithm Methods
	void CalculateCohesion(TArray<ABoid*>& DetectedBoids);
	void CalculateSeparation(TArray<ABoid*>& DetectedBoids);
	void CalculateAlignment(TArray<ABoid*>& DetectedBoids);
	void AvoidPredators();
	void LimitSpeed();
	void KeepWithinBounds();
	void UpdateNearbyBoids();
	void SetFlockManager(AActor* Manager);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	TArray<ABoid*> BoidsTooClose;
	TArray<ABoid*> SafeBoids;
	class AFlockManager* FlockManager;

};
