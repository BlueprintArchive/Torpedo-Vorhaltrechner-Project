#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GyroReciever.generated.h"

class ATDC;

UENUM(BlueprintType)
enum class EGyroRecieverAngleSettingMode : uint8
{
	SpreadManualGyroAutomatic UMETA(DisplayName = "Streuwinkel von Hand, Schusswinkel automatisch"),
	SpreadManual UMETA(DisplayName = "Streuwinkel von Hand"),
	GyroManual UMETA(DisplayName = "Schusswinkel von Hand"),
};

UENUM(BlueprintType)
enum class EGyroRecieverSalvoSize : uint8
{
	SingleShot UMETA(DisplayName = "Einzelschuss"),
	TwoTorpedoSalvo UMETA(DisplayName = "2er Facher"),
	ThreeTorpedoSalvo UMETA(DisplayName = "3er Facher"),
	FourTorpedoSalvo UMETA(DisplayName = "4er Facher"),
};

UENUM(BlueprintType)
enum class EGyroRecieverHighSpeedSwitchPosition : uint8
{
	Neutral UMETA(DisplayName = "Neutral"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right"),
};

UCLASS(Blueprintable)
class BLUEPRINTARCHIVE_API AGyroReciever : public AActor
{
	GENERATED_BODY()

public:
	AGyroReciever();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

public:
	virtual void Tick(float DeltaTime) override;

	/** The TDC that transmits the desired gyro and spread angles. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Links")
	TObjectPtr<ATDC> TDCRef;

	/** If false, the selsyn link from the TDC is considered broken. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Links")
	bool bTransmissionLinkOperational;

	/** If false, the machine amplifier cannot drive the follow-up servo. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Servo")
	bool bMachineAmplifierOperational;

	/** If false, the DC motor cannot rotate the gyro setting train. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Servo")
	bool bDcMotorOperational;

	/** Current switch position controlling whether gyro is automatic or hand-set. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Controls")
	EGyroRecieverAngleSettingMode AngleSettingMode;

	/** Selected salvo gearing mode. Can only be changed when spread is zero. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Controls")
	EGyroRecieverSalvoSize SalvoSize;

	/** Manual boost switch for fast left/right alignment. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Controls")
	EGyroRecieverHighSpeedSwitchPosition HighSpeedSwitchPosition;

	/** Current gyro angle received from the TDC transmitter, in degrees. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gyro Reciever|Dials")
	float ReceivedGyroAngle;

	/** Current spread recommendation received from the TDC transmitter, in degrees. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gyro Reciever|Dials")
	float ReceivedSpreadAngle;

	/** Actual base gyro angle currently set by the receiver's gearing, in degrees. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gyro Reciever|Dials")
	float CurrentGyroSetting;

	/** Current manually entered spread angle passed into the differential gearing, in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter = SetManualSpreadAngle, Category = "Gyro Reciever|Dials", meta = (ClampMin = "0.0", ClampMax = "20.0", UIMin = "0.0", UIMax = "20.0"))
	float ManualSpreadAngle;

	/** Current absolute alignment error between the TDC order and the actual gyro setting, in degrees. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gyro Reciever|Indicators")
	float AlignmentErrorDegrees;

	/** Lit when the actual set gyro angle matches the TDC order within tolerance. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gyro Reciever|Indicators")
	bool bAlignmentLampLit;

	/** Lit when the receiver is not following the TDC order closely enough. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gyro Reciever|Indicators")
	bool bNotFollowingLampLit;

	/** Left high-speed lamp. Indicates the faster setting direction for large left turns. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gyro Reciever|Indicators")
	bool bLeftHighSpeedLampLit;

	/** Right high-speed lamp. Indicates the faster setting direction for large right turns. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gyro Reciever|Indicators")
	bool bRightHighSpeedLampLit;

	/** Tube selection used by the selected salvo mode. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Tubes")
	TArray<bool> SelectedTubes;

	/** Final gyro order delivered to each torpedo tube after spread gearing. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gyro Reciever|Tubes")
	TArray<float> TubeGyroAngles;

	/** Spread multiplier applied to each tube in the active salvo configuration. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gyro Reciever|Tubes")
	TArray<float> TubeSpreadFactors;

	/** Normal automatic follow-up speed, in degrees per second. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Tuning", meta = (ClampMin = "0.0"))
	float FollowUpSpeedDegPerSec;

	/** Faster catch-up speed used when the TDC changes faster than the nominal follow-up rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Tuning", meta = (ClampMin = "0.0"))
	float CatchUpSpeedDegPerSec;

	/** Manual Schnellgang speed, in degrees per second. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Tuning", meta = (ClampMin = "0.0"))
	float HighSpeedDegPerSec;

	/** Alignment tolerance used by the Deckungslampe. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Tuning", meta = (ClampMin = "0.0"))
	float AlignmentToleranceDeg;

	/** Large-error threshold for lighting the left/right Schnellgang direction lamps. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Tuning", meta = (ClampMin = "0.0"))
	float HighSpeedAssistThresholdDeg;

	/** Required spread proximity to zero before the salvo size switch may be changed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gyro Reciever|Tuning", meta = (ClampMin = "0.0"))
	float SpreadZeroToleranceDeg;

	/** Rebuilds the dial and tube state from the current settings without advancing time. */
	UFUNCTION(BlueprintCallable, Category = "Gyro Reciever")
	void RecalculateReceiver();

	/** Reads the latest gyro and spread orders from the TDC. */
	UFUNCTION(BlueprintCallable, Category = "Gyro Reciever")
	void SyncFromTDC();

	/** Applies hand-wheel input to whichever setting is currently selected by the mode switch. */
	UFUNCTION(BlueprintCallable, Category = "Gyro Reciever")
	void ApplyHandwheelInput(float DeltaDegrees);

	/** Directly sets the manual spread angle, in degrees. */
	UFUNCTION(BlueprintCallable, BlueprintSetter, Category = "Gyro Reciever")
	void SetManualSpreadAngle(float NewAngle);

	/** Directly sets the current gyro angle in manual mode, in degrees. */
	UFUNCTION(BlueprintCallable, Category = "Gyro Reciever")
	void SetManualGyroAngle(float NewAngle);

	/** Changes the angle-setting mode. */
	UFUNCTION(BlueprintCallable, Category = "Gyro Reciever")
	void SetAngleSettingMode(EGyroRecieverAngleSettingMode NewMode);

	/** Attempts to change the salvo gearing. Fails unless spread is effectively zero. */
	UFUNCTION(BlueprintCallable, Category = "Gyro Reciever")
	bool SetSalvoSize(EGyroRecieverSalvoSize NewSalvoSize);

	/** Changes the Schnellgang switch position. */
	UFUNCTION(BlueprintCallable, Category = "Gyro Reciever")
	void SetHighSpeedSwitchPosition(EGyroRecieverHighSpeedSwitchPosition NewPosition);

	/** Sets whether a tube is selected for the salvo. */
	UFUNCTION(BlueprintCallable, Category = "Gyro Reciever")
	void SetTubeSelected(int32 TubeIndex, bool bSelected);

	/** Returns the final gyro angle delivered to the requested tube. */
	UFUNCTION(BlueprintPure, Category = "Gyro Reciever")
	float GetTubeGyroAngle(int32 TubeIndex) const;

	/** Returns the spread factor applied to the requested tube for the current salvo mode. */
	UFUNCTION(BlueprintPure, Category = "Gyro Reciever")
	float GetTubeSpreadFactor(int32 TubeIndex) const;

	/** Returns the fine 0-10 degree dial value for the received gyro order. */
	UFUNCTION(BlueprintPure, Category = "Gyro Reciever")
	float GetReceivedGyroFineDial() const;

	/** Returns the fine 0-10 degree dial value for the actually set gyro angle. */
	UFUNCTION(BlueprintPure, Category = "Gyro Reciever")
	float GetSetGyroFineDial() const;

private:
	void EnsureTubeArrays();
	void UpdateAutomaticGyro(float DeltaTime, float CommandRateDegPerSec);
	void UpdateIndicatorLamps();
	void UpdateTubeOutputs();
	float GetMotorSpeed(float CommandRateDegPerSec) const;
	float GetHighSpeedSwitchSign() const;
	float GetSpreadFactorForTube(int32 TubeIndex) const;
	static float NormalizeAngleDegrees(float AngleDegrees);
	static float NormalizeFineDial(float AngleDegrees);
};
