#ifndef DBC_H
#define DBC_H

#include "dbc.h"

#include <stdint.h>

#define STATUS_OK 0
#define STATUS_ERROR -1

// Struct for DRIVER_SELECTED_KP
typedef struct {
	uint8_t TorqueVectoringEnable;
	uint8_t RegenEnable;
	uint8_t ScaleKp;
} DRIVER_SELECTED_KP_t;

// Struct for FRONT_IMU_VELOCITY_NORMAL
typedef struct {
	double NormalVelocityX;
	double NormalVelocityY;
	double NormalVelocityZ;
} FRONT_IMU_VELOCITY_NORMAL_t;

// Struct for PEDALS_AND_STEERING
typedef struct {
	double SteeringAngle;
	int16_t APPS;
	int16_t BPPS;
} PEDALS_AND_STEERING_t;

// Struct for FRONT_IMU_GYROS
typedef struct {
	double GyroX;
	double GyroY;
	double GyroZ;
} FRONT_IMU_GYROS_t;

// Struct for FRONT_IMU_GPS_POS
typedef struct {
	double Latitude;
	double Longitude;
} FRONT_IMU_GPS_POS_t;

// Struct for FRONT_IMU_GPS1_VELOCITY
typedef struct {
	double VelocityN;
	double VelocityE;
	double VelocityD;
} FRONT_IMU_GPS1_VELOCITY_t;

// Struct for FRONT_IMU_ACCELERATION
typedef struct {
	double AccelX;
	double AccelY;
	double AccelZ;
} FRONT_IMU_ACCELERATION_t;

// Struct for FRONT_IMU_ANGLE_RATES
typedef struct {
	double DeltaAngleX;
	double DeltaAngleY;
	double DeltaAngleZ;
} FRONT_IMU_ANGLE_RATES_t;

// Struct for FRONT_IMU_EKF_EULER
typedef struct {
	double Roll;
	double Pitch;
	double Yaw;
} FRONT_IMU_EKF_EULER_t;

// Struct for FRONT_IMU_EKF_VELOCITY_BODY
typedef struct {
	double VelocityX;
	double VelocityY;
	double VelocityZ;
} FRONT_IMU_EKF_VELOCITY_BODY_t;

// Struct for FRONT_IMU_EKF_NED_VEL_ACC
typedef struct {
	double VelocityAccN;
	double VelocityAccE;
	double VelocityAccD;
} FRONT_IMU_EKF_NED_VEL_ACC_t;

// Struct for FRONT_IMU_GPU_VEL_INFO
typedef struct {
	uint32_t TimeStamp;
	uint32_t Status;
} FRONT_IMU_GPU_VEL_INFO_t;

// Struct for FRONT_IMU_GPS_POS_INFO
typedef struct {
	uint32_t TimeStamp;
	uint32_t Status;
} FRONT_IMU_GPS_POS_INFO_t;

// Struct for FRONT_IMU_SLIPS
typedef struct {
	double AngleTrack;
	double AngleSlip;
	double CurvatureRadius;
	uint8_t AutoStatus;
} FRONT_IMU_SLIPS_t;

// Struct for REAR_IMU_GYROS
typedef struct {
	double GyroX;
	double GyroY;
	double GyroZ;
} REAR_IMU_GYROS_t;

// Struct for REAR_IMU_GPS_POS
typedef struct {
	double Latitude;
	double Longitude;
} REAR_IMU_GPS_POS_t;

// Struct for REAR_IMU_GPS1_VELOCITY
typedef struct {
	double VelocityN;
	double VelocityE;
	double VelocityD;
} REAR_IMU_GPS1_VELOCITY_t;

// Struct for REAR_IMU_ACCELERATION
typedef struct {
	double AccelX;
	double AccelY;
	double AccelZ;
} REAR_IMU_ACCELERATION_t;

// Struct for REAR_IMU_ANGLE_RATES
typedef struct {
	double DeltaAngleX;
	double DeltaAngleY;
	double DeltaAngleZ;
} REAR_IMU_ANGLE_RATES_t;

// Struct for REAR_IMU_EKF_EULER
typedef struct {
	double Roll;
	double Pitch;
	double Yaw;
} REAR_IMU_EKF_EULER_t;

// Struct for REAR_IMU_EKF_VELOCITY_BODY
typedef struct {
	double VelocityX;
	double VelocityY;
	double VelocityZ;
} REAR_IMU_EKF_VELOCITY_BODY_t;

// Struct for REAR_IMU_EKF_NED_VEL_ACC
typedef struct {
	double VelocityAccN;
	double VelocityAccE;
	double VelocityAccD;
} REAR_IMU_EKF_NED_VEL_ACC_t;

// Struct for REAR_IMU_GPU_VEL_INFO
typedef struct {
	uint32_t TimeStamp;
	uint32_t Status;
} REAR_IMU_GPU_VEL_INFO_t;

// Struct for REAR_IMU_GPS_POS_INFO
typedef struct {
	uint32_t TimeStamp;
	uint32_t Status;
} REAR_IMU_GPS_POS_INFO_t;

// Struct for REAR_IMU_SLIPS
typedef struct {
	double AngleTrack;
	double AngleSlip;
	double CurvatureRadius;
	uint8_t AutoStatus;
} REAR_IMU_SLIPS_t;

// Struct for FRONT_IMU_STATUS_03
typedef struct {
	uint8_t SolutionMode;
	uint8_t SolAttitudeValid;
	uint8_t SolHeadingValid;
	uint8_t SolVelocityValid;
	uint8_t SolPositionValid;
	uint16_t HeaveStatus;
} FRONT_IMU_STATUS_03_t;

// Struct for WHEEL_SPEEDS
typedef struct {
	double WheelSpeedFR;
	double WheelSpeedFL;
	double WheelSpeedRR;
	double WheelSpeedRL;
} WHEEL_SPEEDS_t;

// Struct for REAR_DAMPERS_REAR_BRAKES
typedef struct {
	double DamperTravelRR;
	double DamperTravelRL;
	double BrakeTempRR;
	double BrakeTempRL;
} REAR_DAMPERS_REAR_BRAKES_t;

// Struct for BRAKE_PRESSURES
typedef struct {
	double BrakePressureFront;
	double BrakePressureRear;
} BRAKE_PRESSURES_t;

// Struct for COOLANT_PRESSURES
typedef struct {
	double CoolantInverterPressure;
	double CoolantPressureFanIN;
	double CoolantPressureFanOUT;
} COOLANT_PRESSURES_t;

// Struct for COOLANT_TEMPS
typedef struct {
	double CoolantTempInverter;
	double CoolantTempFanIN;
	double CoolantTempFanOUT;
} COOLANT_TEMPS_t;

// Struct for DRIVER_DISPLAY_SELECT
typedef struct {
	uint8_t CurrentSelect;
} DRIVER_DISPLAY_SELECT_t;

// Struct for ECU_TractiveState
typedef struct {
	int8_t Tractive_State;
} ECU_TractiveState_t;

// Struct for ECU_STATUS_AND_LAPS
typedef struct {
	double IMD;
	uint32_t LapBeacon;
	double BatteryVoltage;
} ECU_STATUS_AND_LAPS_t;

// Struct for DASHBOARD_PARAMS
typedef struct {
	uint8_t LaunchModeStatus;
	uint8_t CruiseControlActive;
	uint8_t RegenActive;
	uint8_t PedalSensitivity;
} DASHBOARD_PARAMS_t;

// Struct for JETSON_TX_INFO
typedef struct {
	uint8_t Heartbeat;
	uint8_t Ready;
	uint8_t MissionFinish;
	uint8_t MissionSelected;
	double SpeedTarget;
	uint8_t LapCount;
	uint8_t ConesCountActual;
	uint8_t ConesCountAll;
	uint8_t RecorderState;
} JETSON_TX_INFO_t;

// Struct for AV_DEBUG_DISPLAY_1
typedef struct {
	uint8_t ResConnCAN;
	uint8_t JetsonConnCAN;
	uint8_t JetsonLidarDataConn;
	uint8_t JetsonCameraDataConn;
	uint8_t SBGConn;
	uint32_t JetsonIP;
	uint8_t JetsonCpuTemp;
	uint8_t JetsonGpuTemp;
	uint8_t ResPower;
} AV_DEBUG_DISPLAY_1_t;

// Struct for AV_DEBUG_DISPLAY_2
typedef struct {
	uint8_t JetsonPowerConsumption;
	uint8_t JetsonPowerMode;
	uint8_t AvStartupStatus;
	uint8_t SBGSolNStatus;
	uint8_t AsStateRos;
	uint8_t AsMissionRos;
	uint8_t StanleyStatus;
	uint8_t PurePursuitStatus;
	uint8_t LidarNodeStatus;
	uint8_t CameraNodeStatus;
	uint8_t SensorFusionStatus;
	uint8_t LidarDriverStatus;
	uint8_t CameraDriverStatus;
	uint8_t WaypointNodeStatus;
	uint8_t SlamStatus;
	uint8_t StateEstimatorStatus;
	uint8_t MissionControllerStatus;
	uint8_t BrakeHold;
} AV_DEBUG_DISPLAY_2_t;

// Struct for AV_DEBUG_DISPLAY_3
typedef struct {
	double Longitude;
	double Latitude;
	double SBGAccX;
	double SBGAccY;
} AV_DEBUG_DISPLAY_3_t;

// Struct for AV_DEBUG_DISPLAY_4
typedef struct {
	double SteeringReqRos;
	double TorqueReqRos;
	double VelocityReqRos;
	double SteeringActual;
} AV_DEBUG_DISPLAY_4_t;

// Struct for AV_DEBUG_DISPLAY_5
typedef struct {
	double VelocityActual;
	double TorqueActual;
	double AccelActual;
	double AccelReqRos;
} AV_DEBUG_DISPLAY_5_t;

// Struct for ODOMETER_UPDATE
typedef struct {
	uint32_t Odometer;
} ODOMETER_UPDATE_t;

// Struct for ODOMETER_SAVE
typedef struct {
	uint32_t OdometerS;
} ODOMETER_SAVE_t;

// Struct for TIMESTAMP_READING
typedef struct {
	uint32_t Time_UNIX;
} TIMESTAMP_READING_t;

// Struct for RTD_MODES_FLASH
typedef struct {
	uint8_t RTDModeF;
	uint8_t TotalTorqueLimitF;
	uint8_t PWTorqueLF;
	uint32_t PowerLimitF;
	int8_t RegenModeF;
	uint8_t TRegenTorqueLF;
	uint8_t PWRegenTorqueLF;
	uint8_t TorqueVectoringModeF;
	uint8_t LaunchEnableF;
	uint8_t SpeedSetPointF;
} RTD_MODES_FLASH_t;

// Struct for RTD_MODES_SAVE
typedef struct {
	uint8_t RTDMode;
	uint8_t TotalTorqueLimit;
	uint8_t PWTorqueL;
	uint32_t PowerLimit;
	int8_t RegenMode;
	uint8_t TRegenTorqueL;
	uint8_t PWRegenTorqueL;
	uint8_t TorqueVectoringMode;
	uint8_t LaunchEnable;
	uint8_t SpeedSetPoint;
} RTD_MODES_SAVE_t;

// Struct for RTD_MODES_UPDATE
typedef struct {
	uint8_t RTDModeU;
	uint8_t TotalTorqueLimitU;
	uint8_t PWTorqueLU;
	uint32_t PowerLimitU;
	int8_t RegenModeU;
	uint8_t TRegenTorqueLU;
	uint8_t PWRegenTorqueLU;
	uint8_t TorqueVectoringModeU;
	uint8_t LaunchEnableU;
	uint8_t SpeedSetPointU;
} RTD_MODES_UPDATE_t;

// Struct for CANHUB_TEMP_SENSORS
typedef struct {
	double OilTempSensorLeft;
	double OilTempSensorRight;
	double CoolingTempSensorLeft;
	double CoolingTempSensorRight;
} CANHUB_TEMP_SENSORS_t;

// Struct for CANHUB_POTS
typedef struct {
	double CanhubPot1;
	double CanhubPot2;
} CANHUB_POTS_t;

// Struct for CANHUB_THERMOCOUPLES
typedef struct {
	double ThermoFL;
	double ThermoFR;
	double ThermoRL;
	double ThermoRR;
} CANHUB_THERMOCOUPLES_t;

// Struct for CANHUB_STRAIN_LINKS
typedef struct {
	int16_t LinkStrain1;
	int16_t LinkStrain2;
	int16_t LinkStrain3;
	int16_t LinkStrain4;
} CANHUB_STRAIN_LINKS_t;

// Struct for CANHUB_STRAIN_STEERING
typedef struct {
	int16_t SteeringStrain1;
	int16_t SteeringStrain2;
} CANHUB_STRAIN_STEERING_t;


// Unpack function prototypes
int8_t Unpack_DRIVER_SELECTED_KP(DRIVER_SELECTED_KP_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_VELOCITY_NORMAL(FRONT_IMU_VELOCITY_NORMAL_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_PEDALS_AND_STEERING(PEDALS_AND_STEERING_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_GYROS(FRONT_IMU_GYROS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_GPS_POS(FRONT_IMU_GPS_POS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_GPS1_VELOCITY(FRONT_IMU_GPS1_VELOCITY_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_ACCELERATION(FRONT_IMU_ACCELERATION_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_ANGLE_RATES(FRONT_IMU_ANGLE_RATES_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_EKF_EULER(FRONT_IMU_EKF_EULER_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_EKF_VELOCITY_BODY(FRONT_IMU_EKF_VELOCITY_BODY_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_EKF_NED_VEL_ACC(FRONT_IMU_EKF_NED_VEL_ACC_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_GPU_VEL_INFO(FRONT_IMU_GPU_VEL_INFO_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_GPS_POS_INFO(FRONT_IMU_GPS_POS_INFO_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_SLIPS(FRONT_IMU_SLIPS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_IMU_GYROS(REAR_IMU_GYROS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_IMU_GPS_POS(REAR_IMU_GPS_POS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_IMU_GPS1_VELOCITY(REAR_IMU_GPS1_VELOCITY_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_IMU_ACCELERATION(REAR_IMU_ACCELERATION_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_IMU_ANGLE_RATES(REAR_IMU_ANGLE_RATES_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_IMU_EKF_EULER(REAR_IMU_EKF_EULER_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_IMU_EKF_VELOCITY_BODY(REAR_IMU_EKF_VELOCITY_BODY_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_IMU_EKF_NED_VEL_ACC(REAR_IMU_EKF_NED_VEL_ACC_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_IMU_GPU_VEL_INFO(REAR_IMU_GPU_VEL_INFO_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_IMU_GPS_POS_INFO(REAR_IMU_GPS_POS_INFO_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_IMU_SLIPS(REAR_IMU_SLIPS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_FRONT_IMU_STATUS_03(FRONT_IMU_STATUS_03_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_WHEEL_SPEEDS(WHEEL_SPEEDS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_REAR_DAMPERS_REAR_BRAKES(REAR_DAMPERS_REAR_BRAKES_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_BRAKE_PRESSURES(BRAKE_PRESSURES_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_COOLANT_PRESSURES(COOLANT_PRESSURES_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_COOLANT_TEMPS(COOLANT_TEMPS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_DRIVER_DISPLAY_SELECT(DRIVER_DISPLAY_SELECT_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_ECU_TractiveState(ECU_TractiveState_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_ECU_STATUS_AND_LAPS(ECU_STATUS_AND_LAPS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_DASHBOARD_PARAMS(DASHBOARD_PARAMS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_JETSON_TX_INFO(JETSON_TX_INFO_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_AV_DEBUG_DISPLAY_1(AV_DEBUG_DISPLAY_1_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_AV_DEBUG_DISPLAY_2(AV_DEBUG_DISPLAY_2_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_AV_DEBUG_DISPLAY_3(AV_DEBUG_DISPLAY_3_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_AV_DEBUG_DISPLAY_4(AV_DEBUG_DISPLAY_4_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_AV_DEBUG_DISPLAY_5(AV_DEBUG_DISPLAY_5_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_ODOMETER_UPDATE(ODOMETER_UPDATE_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_ODOMETER_SAVE(ODOMETER_SAVE_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_TIMESTAMP_READING(TIMESTAMP_READING_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_RTD_MODES_FLASH(RTD_MODES_FLASH_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_RTD_MODES_SAVE(RTD_MODES_SAVE_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_RTD_MODES_UPDATE(RTD_MODES_UPDATE_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_CANHUB_TEMP_SENSORS(CANHUB_TEMP_SENSORS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_CANHUB_POTS(CANHUB_POTS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_CANHUB_THERMOCOUPLES(CANHUB_THERMOCOUPLES_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_CANHUB_STRAIN_LINKS(CANHUB_STRAIN_LINKS_t* _m, const uint8_t* _d, uint8_t len);
int8_t Unpack_CANHUB_STRAIN_STEERING(CANHUB_STRAIN_STEERING_t* _m, const uint8_t* _d, uint8_t len);

// Pack function prototypes
int8_t Pack_DRIVER_SELECTED_KP(const DRIVER_SELECTED_KP_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_VELOCITY_NORMAL(const FRONT_IMU_VELOCITY_NORMAL_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_PEDALS_AND_STEERING(const PEDALS_AND_STEERING_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_GYROS(const FRONT_IMU_GYROS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_GPS_POS(const FRONT_IMU_GPS_POS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_GPS1_VELOCITY(const FRONT_IMU_GPS1_VELOCITY_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_ACCELERATION(const FRONT_IMU_ACCELERATION_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_ANGLE_RATES(const FRONT_IMU_ANGLE_RATES_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_EKF_EULER(const FRONT_IMU_EKF_EULER_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_EKF_VELOCITY_BODY(const FRONT_IMU_EKF_VELOCITY_BODY_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_EKF_NED_VEL_ACC(const FRONT_IMU_EKF_NED_VEL_ACC_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_GPU_VEL_INFO(const FRONT_IMU_GPU_VEL_INFO_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_GPS_POS_INFO(const FRONT_IMU_GPS_POS_INFO_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_SLIPS(const FRONT_IMU_SLIPS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_IMU_GYROS(const REAR_IMU_GYROS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_IMU_GPS_POS(const REAR_IMU_GPS_POS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_IMU_GPS1_VELOCITY(const REAR_IMU_GPS1_VELOCITY_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_IMU_ACCELERATION(const REAR_IMU_ACCELERATION_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_IMU_ANGLE_RATES(const REAR_IMU_ANGLE_RATES_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_IMU_EKF_EULER(const REAR_IMU_EKF_EULER_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_IMU_EKF_VELOCITY_BODY(const REAR_IMU_EKF_VELOCITY_BODY_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_IMU_EKF_NED_VEL_ACC(const REAR_IMU_EKF_NED_VEL_ACC_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_IMU_GPU_VEL_INFO(const REAR_IMU_GPU_VEL_INFO_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_IMU_GPS_POS_INFO(const REAR_IMU_GPS_POS_INFO_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_IMU_SLIPS(const REAR_IMU_SLIPS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_FRONT_IMU_STATUS_03(const FRONT_IMU_STATUS_03_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_WHEEL_SPEEDS(const WHEEL_SPEEDS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_REAR_DAMPERS_REAR_BRAKES(const REAR_DAMPERS_REAR_BRAKES_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_BRAKE_PRESSURES(const BRAKE_PRESSURES_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_COOLANT_PRESSURES(const COOLANT_PRESSURES_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_COOLANT_TEMPS(const COOLANT_TEMPS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_DRIVER_DISPLAY_SELECT(const DRIVER_DISPLAY_SELECT_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_ECU_TractiveState(const ECU_TractiveState_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_ECU_STATUS_AND_LAPS(const ECU_STATUS_AND_LAPS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_DASHBOARD_PARAMS(const DASHBOARD_PARAMS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_JETSON_TX_INFO(const JETSON_TX_INFO_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_AV_DEBUG_DISPLAY_1(const AV_DEBUG_DISPLAY_1_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_AV_DEBUG_DISPLAY_2(const AV_DEBUG_DISPLAY_2_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_AV_DEBUG_DISPLAY_3(const AV_DEBUG_DISPLAY_3_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_AV_DEBUG_DISPLAY_4(const AV_DEBUG_DISPLAY_4_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_AV_DEBUG_DISPLAY_5(const AV_DEBUG_DISPLAY_5_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_ODOMETER_UPDATE(const ODOMETER_UPDATE_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_ODOMETER_SAVE(const ODOMETER_SAVE_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_TIMESTAMP_READING(const TIMESTAMP_READING_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_RTD_MODES_FLASH(const RTD_MODES_FLASH_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_RTD_MODES_SAVE(const RTD_MODES_SAVE_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_RTD_MODES_UPDATE(const RTD_MODES_UPDATE_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_CANHUB_TEMP_SENSORS(const CANHUB_TEMP_SENSORS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_CANHUB_POTS(const CANHUB_POTS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_CANHUB_THERMOCOUPLES(const CANHUB_THERMOCOUPLES_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_CANHUB_STRAIN_LINKS(const CANHUB_STRAIN_LINKS_t* _m, uint8_t* _d, uint8_t len);
int8_t Pack_CANHUB_STRAIN_STEERING(const CANHUB_STRAIN_STEERING_t* _m, uint8_t* _d, uint8_t len);

// Macros to apply scaling and offset
#define UNPACK_SCALE_OFFSET_FRONT_IMU_VELOCITY_NORMAL_NORMALVELOCITYX(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_VELOCITY_NORMAL_NORMALVELOCITYY(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_VELOCITY_NORMAL_NORMALVELOCITYZ(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_VELOCITY_NORMAL_NORMALVELOCITYX(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_VELOCITY_NORMAL_NORMALVELOCITYY(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_VELOCITY_NORMAL_NORMALVELOCITYZ(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_PEDALS_AND_STEERING_STEERINGANGLE(x) ( (((x) * (0.100000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_PEDALS_AND_STEERING_STEERINGANGLE(x) ( (((x) - (0.000000)) / (0.100000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROX(x) ( (((x) * (0.001000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROY(x) ( (((x) * (0.001000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROZ(x) ( (((x) * (0.001000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROX(x) ( (((x) - (0.000000)) / (0.001000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROY(x) ( (((x) - (0.000000)) / (0.001000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROZ(x) ( (((x) - (0.000000)) / (0.001000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LATITUDE(x) ( (((x) * (0.000000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LONGITUDE(x) ( (((x) * (0.000000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LATITUDE(x) ( (((x) - (0.000000)) / (0.000000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LONGITUDE(x) ( (((x) - (0.000000)) / (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYN(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYE(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYD(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYN(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYE(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYD(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELX(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELY(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELZ(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELX(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELY(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELZ(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEX(x) ( (((x) * (0.001000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEY(x) ( (((x) * (0.001000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEZ(x) ( (((x) * (0.001000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEX(x) ( (((x) - (0.000000)) / (0.001000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEY(x) ( (((x) - (0.000000)) / (0.001000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEZ(x) ( (((x) - (0.000000)) / (0.001000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_ROLL(x) ( (((x) * (0.000100)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_PITCH(x) ( (((x) * (0.000100)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_YAW(x) ( (((x) * (0.000100)) + (0.000000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_ROLL(x) ( (((x) - (0.000000)) / (0.000100)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_PITCH(x) ( (((x) - (0.000000)) / (0.000100)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_YAW(x) ( (((x) - (0.000000)) / (0.000100)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYX(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYY(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYZ(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYX(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYY(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYZ(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCN(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCE(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCD(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCN(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCE(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCD(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_SLIPS_ANGLETRACK(x) ( (((x) * (0.000100)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_SLIPS_ANGLESLIP(x) ( (((x) * (0.000100)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_FRONT_IMU_SLIPS_CURVATURERADIUS(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_SLIPS_ANGLETRACK(x) ( (((x) - (0.000000)) / (0.000100)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_SLIPS_ANGLESLIP(x) ( (((x) - (0.000000)) / (0.000100)) )
#define PACK_SCALE_OFFSET_FRONT_IMU_SLIPS_CURVATURERADIUS(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROX(x) ( (((x) * (0.001000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROY(x) ( (((x) * (0.001000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROZ(x) ( (((x) * (0.001000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROX(x) ( (((x) - (0.000000)) / (0.001000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROY(x) ( (((x) - (0.000000)) / (0.001000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROZ(x) ( (((x) - (0.000000)) / (0.001000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LATITUDE(x) ( (((x) * (0.000000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LONGITUDE(x) ( (((x) * (0.000000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LATITUDE(x) ( (((x) - (0.000000)) / (0.000000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LONGITUDE(x) ( (((x) - (0.000000)) / (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYN(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYE(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYD(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYN(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYE(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYD(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELX(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELY(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELZ(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELX(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELY(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELZ(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEX(x) ( (((x) * (0.001000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEY(x) ( (((x) * (0.001000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEZ(x) ( (((x) * (0.001000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEX(x) ( (((x) - (0.000000)) / (0.001000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEY(x) ( (((x) - (0.000000)) / (0.001000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEZ(x) ( (((x) - (0.000000)) / (0.001000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_ROLL(x) ( (((x) * (0.000100)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_PITCH(x) ( (((x) * (0.000100)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_YAW(x) ( (((x) * (0.000100)) + (0.000000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_ROLL(x) ( (((x) - (0.000000)) / (0.000100)) )
#define PACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_PITCH(x) ( (((x) - (0.000000)) / (0.000100)) )
#define PACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_YAW(x) ( (((x) - (0.000000)) / (0.000100)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYX(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYY(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYZ(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYX(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYY(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYZ(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCN(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCE(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCD(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCN(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCE(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCD(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_SLIPS_ANGLETRACK(x) ( (((x) * (0.000100)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_SLIPS_ANGLESLIP(x) ( (((x) * (0.000100)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_IMU_SLIPS_CURVATURERADIUS(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_REAR_IMU_SLIPS_ANGLETRACK(x) ( (((x) - (0.000000)) / (0.000100)) )
#define PACK_SCALE_OFFSET_REAR_IMU_SLIPS_ANGLESLIP(x) ( (((x) - (0.000000)) / (0.000100)) )
#define PACK_SCALE_OFFSET_REAR_IMU_SLIPS_CURVATURERADIUS(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDFR(x) ( (((x) * (0.100000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDFL(x) ( (((x) * (0.100000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDRR(x) ( (((x) * (0.100000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDRL(x) ( (((x) * (0.100000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDFR(x) ( (((x) - (0.000000)) / (0.100000)) )
#define PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDFL(x) ( (((x) - (0.000000)) / (0.100000)) )
#define PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDRR(x) ( (((x) - (0.000000)) / (0.100000)) )
#define PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDRL(x) ( (((x) - (0.000000)) / (0.100000)) )
#define UNPACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_DAMPERTRAVELRR(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_DAMPERTRAVELRL(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_BRAKETEMPRR(x) ( (((x) * (0.100000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_BRAKETEMPRL(x) ( (((x) * (0.100000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_DAMPERTRAVELRR(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_DAMPERTRAVELRL(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_BRAKETEMPRR(x) ( (((x) - (0.000000)) / (0.100000)) )
#define PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_BRAKETEMPRL(x) ( (((x) - (0.000000)) / (0.100000)) )
#define UNPACK_SCALE_OFFSET_BRAKE_PRESSURES_BRAKEPRESSUREFRONT(x) ( (((x) * (0.100000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_BRAKE_PRESSURES_BRAKEPRESSUREREAR(x) ( (((x) * (0.100000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_BRAKE_PRESSURES_BRAKEPRESSUREFRONT(x) ( (((x) - (0.000000)) / (0.100000)) )
#define PACK_SCALE_OFFSET_BRAKE_PRESSURES_BRAKEPRESSUREREAR(x) ( (((x) - (0.000000)) / (0.100000)) )
#define UNPACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTINVERTERPRESSURE(x) ( (((x) * (0.100000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTPRESSUREFANIN(x) ( (((x) * (0.100000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTPRESSUREFANOUT(x) ( (((x) * (0.100000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTINVERTERPRESSURE(x) ( (((x) - (0.000000)) / (0.100000)) )
#define PACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTPRESSUREFANIN(x) ( (((x) - (0.000000)) / (0.100000)) )
#define PACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTPRESSUREFANOUT(x) ( (((x) - (0.000000)) / (0.100000)) )
#define UNPACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPINVERTER(x) ( (((x) * (0.100000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPFANIN(x) ( (((x) * (0.100000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPFANOUT(x) ( (((x) * (0.100000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPINVERTER(x) ( (((x) - (0.000000)) / (0.100000)) )
#define PACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPFANIN(x) ( (((x) - (0.000000)) / (0.100000)) )
#define PACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPFANOUT(x) ( (((x) - (0.000000)) / (0.100000)) )
#define UNPACK_SCALE_OFFSET_ECU_STATUS_AND_LAPS_IMD(x) ( (((x) * (0.100000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_ECU_STATUS_AND_LAPS_BATTERYVOLTAGE(x) ( (((x) * (0.100000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_ECU_STATUS_AND_LAPS_IMD(x) ( (((x) - (0.000000)) / (0.100000)) )
#define PACK_SCALE_OFFSET_ECU_STATUS_AND_LAPS_BATTERYVOLTAGE(x) ( (((x) - (0.000000)) / (0.100000)) )
#define UNPACK_SCALE_OFFSET_JETSON_TX_INFO_SPEEDTARGET(x) ( (((x) * (0.100000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_JETSON_TX_INFO_SPEEDTARGET(x) ( (((x) - (0.000000)) / (0.100000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_LONGITUDE(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_LATITUDE(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_SBGACCX(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_SBGACCY(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_LONGITUDE(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_LATITUDE(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_SBGACCX(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_SBGACCY(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_STEERINGREQROS(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_TORQUEREQROS(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_VELOCITYREQROS(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_STEERINGACTUAL(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_STEERINGREQROS(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_TORQUEREQROS(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_VELOCITYREQROS(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_STEERINGACTUAL(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_VELOCITYACTUAL(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_TORQUEACTUAL(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_ACCELACTUAL(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_ACCELREQROS(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_VELOCITYACTUAL(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_TORQUEACTUAL(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_ACCELACTUAL(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_ACCELREQROS(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_OILTEMPSENSORLEFT(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_OILTEMPSENSORRIGHT(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_COOLINGTEMPSENSORLEFT(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_COOLINGTEMPSENSORRIGHT(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_OILTEMPSENSORLEFT(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_OILTEMPSENSORRIGHT(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_COOLINGTEMPSENSORLEFT(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_COOLINGTEMPSENSORRIGHT(x) ( (((x) - (0.000000)) / (0.010000)) )
#define UNPACK_SCALE_OFFSET_CANHUB_POTS_CANHUBPOT1(x) ( (((x) * (0.001000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_CANHUB_POTS_CANHUBPOT2(x) ( (((x) * (0.001000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_CANHUB_POTS_CANHUBPOT1(x) ( (((x) - (0.000000)) / (0.001000)) )
#define PACK_SCALE_OFFSET_CANHUB_POTS_CANHUBPOT2(x) ( (((x) - (0.000000)) / (0.001000)) )
#define UNPACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMOFL(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMOFR(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMORL(x) ( (((x) * (0.010000)) + (0.000000)) )
#define UNPACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMORR(x) ( (((x) * (0.010000)) + (0.000000)) )
#define PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMOFL(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMOFR(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMORL(x) ( (((x) - (0.000000)) / (0.010000)) )
#define PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMORR(x) ( (((x) - (0.000000)) / (0.010000)) )


#endif
