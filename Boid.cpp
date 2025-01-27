// Fill out your copyright notice in the Description page of Project Settings.


#include "Boid.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <Predator.h>
#include <FlockManager.h>

// Sets default values
ABoid::ABoid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    DetectionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionRadius"));
    DetectionRadius->SetupAttachment(RootComponent);
    SetRootComponent(DetectionRadius);
    

    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoidMesh"));
    VisualMesh->SetupAttachment(RootComponent);
    
    

}

// Called when the game starts or when spawned
void ABoid::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void ABoid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UpdateNearbyBoids();
    CalculateCohesion(SafeBoids);
    CalculateSeparation(BoidsTooClose);
    CalculateAlignment(SafeBoids);
    AvoidPredators();
    LimitSpeed();
    KeepWithinBounds();
    SafeBoids.Reset();
    BoidsTooClose.Reset();

    FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorForwardVector(), Velocity);
    SetActorRotation(NewRotation, ETeleportType::None);
    
    FVector NewPosition = GetActorLocation() + (Velocity * DeltaTime);
    SetActorLocation(NewPosition);
}

void ABoid::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    
    if (!FlockManager)
    {
        FlockManager = Cast<AFlockManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AFlockManager::StaticClass()));

    }
    FlockManager->Boids.Remove(this);

}

void ABoid::LimitSpeed()
{
    if (Velocity.Size() > SpeedLimit)
    {
        Velocity = Velocity.GetSafeNormal() * SpeedLimit;
    }
}

void ABoid::KeepWithinBounds()
{

    FVector CurrentPosition = GetActorLocation();
    FVector NewVelocity = Velocity;

    if (CurrentPosition.X < WorldBoundsMin.X + Margin)
    {
        NewVelocity.X += TurnFactor * ((WorldBoundsMin.X + Margin) - CurrentPosition.X) / Margin;
    }
    else if (CurrentPosition.X > WorldBoundsMax.X - Margin)
    {
        NewVelocity.X -= TurnFactor * (CurrentPosition.X - (WorldBoundsMax.X - Margin)) / Margin;
    }

    if (CurrentPosition.Y < WorldBoundsMin.Y + Margin)
    {
        NewVelocity.Y += TurnFactor * ((WorldBoundsMin.Y + Margin) - CurrentPosition.Y) / Margin;
    }
    else if (CurrentPosition.Y > WorldBoundsMax.Y - Margin)
    {
        NewVelocity.Y -= TurnFactor * (CurrentPosition.Y - (WorldBoundsMax.Y - Margin)) / Margin;
    }

    Velocity = NewVelocity.GetClampedToMaxSize(SpeedLimit);
}

void ABoid::CalculateCohesion(TArray<ABoid*>& DetectedBoids)
{  
    FVector Center = FVector::ZeroVector;
    int NumNeighbors = 0;

    for (ABoid* OtherBoid : DetectedBoids)
    {
        if (OtherBoid)
        {
            if (OtherBoid != this)
            {
                float Distance = FVector::Dist(GetActorLocation(), OtherBoid->GetActorLocation());
                if (Distance < VisualRange) 
                {
                    Center += OtherBoid->GetActorLocation();
                    NumNeighbors++;
                }
            }
        }
    }

    if (NumNeighbors > 0)
    {
        Center /= NumNeighbors;
        FVector ToCenter = Center - GetActorLocation();
        Velocity += ToCenter * CenteringFactor;
    }

}

void ABoid::CalculateSeparation(TArray<ABoid*>& DetectedBoids)
{
    FVector AvoidDirection = FVector::ZeroVector;
    for (ABoid* OtherBoid : DetectedBoids)
    {
        if (OtherBoid)
        {
            if (OtherBoid != this)
            {
                AvoidDirection += GetActorLocation() - OtherBoid->GetActorLocation();
            }
        }
    }

    Velocity += AvoidDirection * AvoidFactor;
}

void ABoid::CalculateAlignment(TArray<ABoid*>& DetectedBoids)
{
    FVector AverageVelocity = FVector::ZeroVector;
    int NumNeighbors = 0;

    for (ABoid* OtherBoid : DetectedBoids)
    {
        if (OtherBoid)
        {
            if (OtherBoid !=this)
            {  
                AverageVelocity += OtherBoid->Velocity;
                NumNeighbors++;
            }
        }
    }

    if (NumNeighbors > 0)
    {
        AverageVelocity /= NumNeighbors;
        Velocity += (AverageVelocity - Velocity) * MatchingFactor;
    }

}


void ABoid::UpdateNearbyBoids()
{
    //TArray<AActor*> Boids;
    //DetectionRadius->GetOverlappingActors(Boids, ABoid::StaticClass());

    //for(AActor* other : Boids)
    for(ABoid* otherBoid : FlockManager->Boids)
    {
        //ABoid* boid = Cast<ABoid>(other);
        float distance = FVector::Dist(this->GetActorLocation(), otherBoid->GetActorLocation());
        if (otherBoid != this && distance <= VisualRange)
        {
            if (otherBoid != this && distance <= MinDistance)
            {
                BoidsTooClose.Add(otherBoid);
            }
            else
            {
                SafeBoids.Add(otherBoid);
            }
        }
        
        
    }
    
}
void ABoid::SetFlockManager(AActor* Manager)
{
    FlockManager = (AFlockManager*)Manager;
}

void ABoid::AvoidPredators()
{
    for (APredator* predator : FlockManager->Predators)
    {
        float distance = FVector::Dist(this->GetActorLocation(), predator->GetActorLocation());
        if (distance <= VisualRange)
        {
            FVector AvoidDirection = FVector::ZeroVector;       
            AvoidDirection += GetActorLocation() - predator->GetActorLocation();
            Velocity += AvoidDirection * FleeFactor;
        }
    }
}





