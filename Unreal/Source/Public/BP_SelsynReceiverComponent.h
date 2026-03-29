#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BP_SelsynReceiverComponent.generated.h"

class ATDC;
class USceneComponent;

UENUM(BlueprintType)
enum class ESelsynSignalType : uint8
{
	GyroAngle UMETA(DisplayName = "Gyro Angle"),
	TargetBearing UMETA(DisplayName = "Target Bearing"),
	AngleOnTheBow UMETA(DisplayName = "Angle On The Bow"),
	ParallaxCorrection UMETA(DisplayName = "Parallax Correction"),
};

UENUM(BlueprintType)
enum class ESelsynDriveAxis : uint8
{
	Yaw UMETA(DisplayName = "Yaw"),
	Pitch UMETA(DisplayName = "Pitch"),
	Roll UMETA(DisplayName = "Roll"),
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BLUEPRINTARCHIVE_API UBP_SelsynReceiverComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBP_SelsynReceiverComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** The TDC actor that provides the commanded selsyn angle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selsyn")
	TObjectPtr<ATDC> TDCRef;

	/** Optional component to rotate. Defaults to the owner root if left unset. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selsyn")
	TObjectPtr<USceneComponent> DrivenComponent;

	/** Which TDC output drives this receiver. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selsyn")
	ESelsynSignalType SignalType;

	/** Which relative rotation axis the receiver should drive. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selsyn")
	ESelsynDriveAxis DriveAxis;

	/** Current displayed angle after lag and deadband are applied. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Selsyn|Output")
	float DisplayedAngle;

	/** Maximum receiver motion rate in degrees per second. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selsyn", meta = (ClampMin = "0.0"))
	float MaxSlewDegPerSec;

	/** Minimum error before the receiver moves, in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selsyn", meta = (ClampMin = "0.0"))
	float DeadbandDeg;

	/** Per-instrument calibration offset applied to the incoming command. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selsyn")
	float CalibrationOffsetDeg;

	/** When false, the receiver holds its current displayed position. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selsyn")
	bool bPowered;

	/** Inverts the driven motion for meshes authored in the opposite direction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Selsyn")
	bool bInvertDirection;

	/** Base relative rotation preserved while the selected axis is being driven. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Selsyn|Debug")
	FRotator BaseRelativeRotation;

	/** Returns the currently selected TDC output without calibration. */
	UFUNCTION(BlueprintPure, Category = "Selsyn")
	float GetCommandAngle() const;

	/** Snaps the displayed receiver angle to the current command. */
	UFUNCTION(BlueprintCallable, Category = "Selsyn")
	void SyncToCommandAngle();

	/** Reapplies the current displayed angle to the driven component. */
	UFUNCTION(BlueprintCallable, Category = "Selsyn")
	void ApplyDisplayedAngle();

private:
	/** Returns the explicit driven component or falls back to the owner root. */
	USceneComponent* ResolveDrivenComponent() const;
	/** Updates the cached base rotation from the resolved driven component. */
	void CacheBaseRelativeRotation();
	/** Normalizes an angle to the [0, 360) range. */
	static float NormalizeAngleDegrees(float AngleDegrees);
};
