// Fill out your copyright notice in the Description page of Project Settings.


#include "Predator.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <Boid.h>
#include <FlockManager.h>
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
APredator::APredator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    DetectionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionRadius"));
    DetectionRadius->SetupAttachment(RootComponent);
    SetRootComponent(DetectionRadius);


    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PredatorMesh"));
    VisualMesh->SetupAttachment(RootComponent);
}

void APredator::CalculateChaseCohesion(TArray<AActor*>& DetectedBoids)
{
    FVector PreyCenter = FVector::ZeroVector;
    int NumPrey = 0;

    for (AActor* OtherBoid : DetectedBoids)
    {
        PreyCenter += OtherBoid->GetActorLocation();
        NumPrey++;
    }

    if (NumPrey > 0)
    {
        PreyCenter /= NumPrey;
        FVector ToPreyCenter = PreyCenter - GetActorLocation();
        Velocity += ToPreyCenter * CenteringFactor;
    }
}

void APredator::UpdateNearbyBoids()
{  
    //TArray<AActor*> Boids;
    //DetectionRadius->GetOverlappingActors(Boids, ABoid::StaticClass());
    for (ABoid* otherBoid : FlockManager->Boids)
    {
        float distance = FVector::Dist(this->GetActorLocation(), otherBoid->GetActorLocation());
        if (distance <= VisualRange)
        {
            NearbyBoids.Add(otherBoid);
        }
    }

}

void APredator::LimitSpeed()
{
    if (Velocity.Size() > SpeedLimit)
    {
        Velocity = Velocity.GetSafeNormal() * SpeedLimit;
    }
}

void APredator::KeepWithinBounds()
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

// Called when the game starts or when spawned
void APredator::BeginPlay()
{
	Super::BeginPlay();
    GetWorld()->GetTimerManager().SetTimer(StarvationTimer, this, &ThisClass::Starve, TimeToStarve, false);
	
}

void APredator::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    GetWorld()->GetTimerManager().ClearTimer(HungerTimer);
    GetWorld()->GetTimerManager().ClearTimer(StarvationTimer);
}

// Called every frame
void APredator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UpdateNearbyBoids();
    if (bHungry && NearbyBoids.Num() > 0)
    {
        CalculateChaseCohesion(NearbyBoids);
        EatBoid(NearbyBoids);
    }
    LimitSpeed();
    KeepWithinBounds();
    NearbyBoids.Reset();

    FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorForwardVector(), Velocity);
    SetActorRotation(NewRotation, ETeleportType::None);

    FVector NewPosition = GetActorLocation() + (Velocity * DeltaTime);
    SetActorLocation(NewPosition);
}

void APredator::UpdateVisualRangeCollider()
{
    DetectionRadius->SetSphereRadius(VisualRange);

}

void APredator::EatBoid(TArray<AActor*>& boids)
{
    AActor* EatenBoid = nullptr;
    for (AActor* boid : boids)
    {
        if (boid)
        {
            float distance = FVector::Dist(this->GetActorLocation(), boid->GetActorLocation());
            if (distance <= EatRange)
            {
                EatenBoid = boid;
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodEffect, VisualMesh->GetComponentLocation(), FVector::RightVector.Rotation(), FVector(.5f));
                UpdateHunger(false);
                HungerDelegate.BindUFunction(this, "UpdateHunger", true);
                GetWorld()->GetTimerManager().SetTimer(HungerTimer, HungerDelegate, TimeTilHungry, false);
                GetWorld()->GetTimerManager().ClearTimer(StarvationTimer);
                break;
            }
        }
    }
    if (IsValid(EatenBoid))
    {
        EatenBoid->Destroy();
    }

}

void APredator::UpdateHunger(bool bIsHungry)
{
    bHungry = bIsHungry;
    if (bHungry)
    {
        GetWorld()->GetTimerManager().ClearTimer(HungerTimer);
        GetWorld()->GetTimerManager().SetTimer(StarvationTimer, this, &ThisClass::Starve, TimeToStarve, false);
    }
}

void APredator::Starve()
{
    if (!FlockManager)
    {
        FlockManager = Cast<AFlockManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AFlockManager::StaticClass()));
    }
    if (FlockManager)
    {
        FlockManager->Predators.Remove(this);
        this->Destroy();
    }
}

void APredator::SetFlockManager(AActor* Manager)
{
    FlockManager = (AFlockManager*)Manager;
}
