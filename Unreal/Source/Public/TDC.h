#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDC.generated.h"

struct FPropertyChangedEvent;

UCLASS(Blueprintable)
class BLUEPRINTARCHIVE_API ATDC : public AActor
{
	GENERATED_BODY()

public:
	/** Constructs the torpedo data computer actor with historical default values. */
	ATDC();

protected:
	/** Initializes the calculator state when play begins. */
	virtual void BeginPlay() override;
	/** Rebuilds the solution when the actor is constructed or edited in the world. */
	virtual void OnConstruction(const FTransform& Transform) override;
#if WITH_EDITOR
	/** Rebuilds the solution when editable properties change in the Details panel. */
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:
	/** Advances the parallax servo simulation by the frame delta time in seconds. */
	virtual void Tick(float DeltaTime) override;

	/** Interval between parallax servo updates, in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDC|Timing")
	float ParallaxUpdateInterval;

	/** Maximum parallax motor speed, in degrees per second. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDC|Timing", meta = (ClampMin = "0.0"))
	float ParallaxMotorRateDegreesPerSecond;

	/** Maximum absolute parallax correction, in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDC|Timing", meta = (ClampMin = "0.0", ClampMax = "25.0"))
	float MaxParallaxCorrectionDegrees;

	/** Selected torpedo speed, in knots. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetTorpedoSpeed, Category = "TDC|Inputs", meta = (ClampMin = "10.0", ClampMax = "44.0", UIMin = "10.0", UIMax = "44.0"))
	float TorpedoSpeed;

	/** Target speed, in knots. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetTargetSpeed, Category = "TDC|Inputs", meta = (ClampMin = "0.0", ClampMax = "55.0", UIMin = "0.0", UIMax = "55.0"))
	float TargetSpeed;

	/** Current parallax correction delta, in degrees. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Outputs")
	float ParallaxCorrection;

	/** Distance to target at launch, in hectometers. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetDistanceToTarget, Category = "TDC|Inputs", meta = (ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"))
	float DistanceToTarget;

	/** Current target-to-torpedo speed quotient vg/vt, unitless. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Outputs")
	float Quotient;

	/** Current signed angle on the bow gamma, in degrees from -180 to 180. Positive values are bow right and negative values are bow left. Editable input that back-solves the derived impact angle alpha. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetAngleOnTheBow, Category = "TDC|Inputs", meta = (DisplayName = "Angle On The Bow", ClampMin = "-180.0", ClampMax = "180.0", UIMin = "-180.0", UIMax = "180.0"))
	float AngleOnTheBow;

	/** True when the target bow is to the right, false when the target bow is to the left. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Outputs", meta = (DisplayName = "Bow To The Right"))
	bool bBowToTheRight;

	/** Human-readable bow-side label for UI and debugging. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Outputs", meta = (DisplayName = "Bow Side Label"))
	FString BowSideLabel;

	/** Torpedo impact angle alpha, in degrees. This is a derived output of the current solution. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Outputs")
	float ImpactAngle;

	/** Displayed deflection angle beta magnitude, in degrees, rounded to the calculator's acceptance-table style 10-degree step. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Outputs")
	float DeflectionAngle;

	/** Displayed signed deflection angle beta, in degrees, derived from the rounded beta magnitude. Negative values are starboard and positive values are port. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Outputs")
	float SignedDeflectionAngle;

	/** If true, the triangle dial shows impact angle alpha instead of deflection angle beta. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDC|Display")
	bool bDisplayImpactAngleOnTriangleDial;

	/** Current value shown on the triangle dial, in degrees. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Outputs")
	float TriangleDialAngle;

	/** Gyro angle rho sent to the torpedo, in degrees. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Outputs")
	float GyroAngle;

	/** Target bearing omega from the sighting device, in degrees from 0 to 360. Values from 0 to 180 are starboard and values from 180 to 360 are port. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetTargetBearing, Category = "TDC|Inputs", meta = (ClampMin = "0.0", ClampMax = "360.0", UIMin = "0.0", UIMax = "360.0"))
	float TargetBearing;

	/** Entered own-turn correction value added to bearing, in degrees or scaled turn units. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetTurnRatioCorrection, Category = "TDC|Inputs")
	float TurnRatioCorrection;

	/** Scale applied to the entered turn correction before adding it to bearing, unitless. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDC|Inputs", meta = (ClampMin = "0.0"))
	float TurnRatioScale;

	/** True for forward tubes, false for rear tubes. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetParallaxForward, Category = "TDC|Inputs")
	bool bParallaxForward;

	/** Target length, in meters. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetTargetLength, Category = "TDC|Inputs", meta = (ClampMin = "1.0", UIMin = "1.0"))
	float TargetLength;

	/** Salvo spread angle psi, in degrees. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Outputs")
	float SpreadAngle;

	/** Current servo shaft position delta1, in radians. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Debug")
	float Delta1;

	/** Current comparator target delta2, in radians. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TDC|Debug")
	float Delta2;

	/** Enables the automatic parallax motor loop. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetParallaxMotorEnabled, Category = "TDC|Inputs")
	bool bEnableParallaxMotor;

	/** Manual parallax setting used when the motor is disabled, in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TDC|Inputs", meta = (ClampMin = "-25.0", ClampMax = "25.0", UIMin = "-25.0", UIMax = "25.0"))
	float ManualParallax;

	/** Back-solves an impact angle from the requested signed angle on the bow gamma, in degrees. */
	UFUNCTION(BlueprintCallable, Category = "TDC")
	void SetAngleOnTheBow(float Angle);

	/** Sets the distance to target in hectometers, clamped to the supported 0-100 hectometer range. */
	UFUNCTION(BlueprintCallable, Category = "TDC")
	void SetDistanceToTarget(float Distance);

	/** Sets the torpedo speed in knots, clamped to the supported 10-44 knot range. */
	UFUNCTION(BlueprintCallable, Category = "TDC")
	void SetTorpedoSpeed(float Speed);

	/** Sets the target speed in knots, clamped to the supported 0-55 knot range. */
	UFUNCTION(BlueprintCallable, Category = "TDC")
	void SetTargetSpeed(float Speed);

	/** Sets the target bearing omega in degrees from 0 to 360. */
	UFUNCTION(BlueprintCallable, Category = "TDC")
	void SetTargetBearing(float Angle);

	/** Sets the entered own-turn correction value. */
	UFUNCTION(BlueprintCallable, Category = "TDC")
	void SetTurnRatioCorrection(float Correction);

	/** Selects forward or rear tube geometry for parallax. */
	UFUNCTION(BlueprintCallable, Category = "TDC")
	void SetParallaxForward(bool bNewValue);

	/** Sets the target length in meters. */
	UFUNCTION(BlueprintCallable, Category = "TDC")
	void SetTargetLength(float NewValue);

	/** Enables or disables the automatic parallax motor. */
	UFUNCTION(BlueprintCallable, Category = "TDC")
	void SetParallaxMotorEnabled(bool bEnabled);

	/** Recomputes the full solution and settles parallax to equilibrium. */
	UFUNCTION(BlueprintCallable, Category = "TDC")
	void RecalculateSolution();

	/** Returns the current parallax correction delta1 as degrees. */
	UFUNCTION(BlueprintPure, Category = "TDC")
	float GetDelta1() const;

	/** Returns the current comparator demand delta2 as signed degrees. */
	UFUNCTION(BlueprintPure, Category = "TDC")
	float GetDelta2() const;

	/** Returns the dimensionless maximum-distance factor based on alpha and beta. */
	UFUNCTION(BlueprintPure, Category = "TDC")
	float GetMaximumDistanceToTarget() const;

	/** Returns the current triangle dial display value in degrees. */
	UFUNCTION(BlueprintPure, Category = "TDC")
	float GetTriangleDialAngle() const;

	/** Converts degrees to radians. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static float DegreesToRadians(float Degrees);

	/** Converts radians to degrees. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static float RadiansToDegrees(float Radians);

	/** Truncates a floating-point value to the requested number of decimal places. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static float TruncateDecimals(float Number, int32 Digits);

	/** Computes the speed quotient vg/vt from target and torpedo speeds in knots. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static float ComputeQuotientFromSpeeds(float InTargetSpeed, float InTorpedoSpeed);

	/** Computes deflection angle beta from impact angle alpha in degrees and speeds in knots. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static float ComputeDeflectionAngleFromInputs(float InImpactAngle, float InTargetSpeed, float InTorpedoSpeed);

	/** Converts beta magnitude into a signed angle using the 0-360 target bearing direction. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static float ComputeSignedDeflectionAngle(float InDeflectionAngleMagnitude, float InTargetBearing = 0.0f);

	/** Computes gyro angle rho in degrees from bearing, parallax, signed beta, and turn correction. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static float ComputeGyroAngleFromInputs(float InTargetBearing, float InParallaxCorrection, float InSignedDeflectionAngle, float InTurnRatioCorrection = 0.0f, float InTurnRatioScale = 0.4f);

	/** Computes the sine-law range factor e/st for the current alpha and beta, unitless. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static float ComputeMaximumDistanceFactorFromInputs(float InImpactAngle, float InDeflectionAngle);

	/** Computes maximum target distance in the same distance unit as the supplied torpedo run. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static float ComputeMaximumDistanceFromInputs(float InImpactAngle, float InDeflectionAngle, float InMaximumTorpedoRun);

	/** Computes spread angle psi from target length in meters, distance in hectometers, and angle-on-bow in degrees. Signed bow angles use their magnitude. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static float ComputeSpreadAngleFromInputs(float InTargetLength, float InDistanceToTarget, float InAngleOnTheBow, float InTargetSpeed, float InTorpedoSpeed);

	/** Computes the triangle dial display value in degrees for early or later calculator variants. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static float ComputeTriangleDialAngleFromInputs(float InImpactAngle, float InDeflectionAngle, bool bInDisplayImpactAngleOnTriangleDial = true);

	/** Computes equivalent-point Cartesian coordinates in meters for a given gyro angle and tube group. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static FVector2D ComputeEquivalentPointCartesianFromGyro(float InGyroAngleDegrees, bool bForwardTube);

	/** Computes equivalent-point polar coordinates where X is meters and Y is radians. */
	UFUNCTION(BlueprintPure, Category = "TDC|Math")
	static FVector2D ComputeEquivalentPointPolarFromGyro(float InGyroAngleDegrees, bool bForwardTube);

private:
	/** Continuous deflection-angle solution used internally for triangle math and alpha/gamma consistency, in degrees. */
	float ContinuousDeflectionAngle;
	/** Clamps editable inputs into the supported historical ranges. */
	void ApplyInputClamps();
	/** Refreshes quotient, beta, dial state, and spread from the current public inputs. */
	void RefreshSolutionFromInputs();
	/** Updates the stored vg/vt quotient from the current speeds. */
	void UpdateQuotient();
	/** Advances the parallax loop by the supplied time step in seconds. */
	void UpdateParallax(float StepTime);
	/** Recomputes beta from the current alpha and speed settings. */
	void CalculateDeflectionAngle();
	/** Recomputes the current salvo spread angle psi. */
	void CalculateSpread();
	/** Updates the displayed triangle dial value for the selected historical variant. */
	void UpdateTriangleDialAngle();
	/** Normalizes an angle to the [0, 360) degree range. */
	float NormalizeDegrees(float AngleDegrees) const;
	/** Returns target bearing plus turn correction, in degrees. */
	float GetCorrectedTargetBearingDegrees() const;
	/** Back-solves alpha from a desired gamma value, in degrees. */
	bool SolveImpactAngleFromBow(float DesiredAngleOnTheBow, int32 MaxIterations = 32, float ToleranceDegrees = 0.001f);
	/** Iterates the parallax loop until the servo reaches equilibrium. */
	void SolveParallaxToEquilibrium(int32 MaxIterations = 4096, float ToleranceRadians = 0.0001f);
	/** Updates all derived display and solution values from the current servo shaft position. */
	void UpdateDerivedStateFromDelta1();
	/** Computes the comparator demand delta2 from the current gyro and geometry state. */
	float CalculateDelta2FromCurrentState() const;
	/** Returns the equivalent-point Cartesian coordinates in meters for the current tube group. */
	FVector2D CalculateCartesianCoordinateOfIdealTorpedoStartPoint(float AngleDegrees) const;
	/** Converts equivalent-point Cartesian coordinates to polar coordinates where X is meters and Y is radians. */
	FVector2D CalculatePolarCoordinateOfIdealTorpedoStartPoint(float X, float Y) const;
	/** Converts the public hectometer distance input to meters for internal geometry math. */
	float GetDistanceToTargetMeters() const;
};
