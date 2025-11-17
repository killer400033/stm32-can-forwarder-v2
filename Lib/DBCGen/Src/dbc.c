#include "dbc.h"

// Unpack signals from PEDALS_AND_STEERING
int8_t Unpack_PEDALS_AND_STEERING(PEDALS_AND_STEERING_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting SteeringAngle
	_m->SteeringAngle = (double)UNPACK_SCALE_OFFSET_PEDALS_AND_STEERING_STEERINGANGLE(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting APPS
	_m->APPS = ((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u);
	// Extracting BPPS
	_m->BPPS = ((_d[4] & 0xffu) << 8u) | ((_d[5] & 0xffu) >> 0u);

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_GYROS
int8_t Unpack_FRONT_IMU_GYROS(FRONT_IMU_GYROS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting GyroX
	_m->GyroX = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROX(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting GyroY
	_m->GyroY = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROY(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting GyroZ
	_m->GyroZ = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROZ(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_GPS_POS
int8_t Unpack_FRONT_IMU_GPS_POS(FRONT_IMU_GPS_POS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting Latitude
	_m->Latitude = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LATITUDE(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u) | ((_d[2] & 0xffu) << 16u) | ((_d[3] & 0xffu) << 24u));
	// Extracting Longitude
	_m->Longitude = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LONGITUDE(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u) | ((_d[6] & 0xffu) << 16u) | ((_d[7] & 0xffu) << 24u));

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_GPS1_VELOCITY
int8_t Unpack_FRONT_IMU_GPS1_VELOCITY(FRONT_IMU_GPS1_VELOCITY_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting VelocityN
	_m->VelocityN = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYN(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting VelocityE
	_m->VelocityE = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYE(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting VelocityD
	_m->VelocityD = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYD(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_ACCELERATION
int8_t Unpack_FRONT_IMU_ACCELERATION(FRONT_IMU_ACCELERATION_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting AccelX
	_m->AccelX = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELX(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting AccelY
	_m->AccelY = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELY(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting AccelZ
	_m->AccelZ = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELZ(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_ANGLE_RATES
int8_t Unpack_FRONT_IMU_ANGLE_RATES(FRONT_IMU_ANGLE_RATES_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting DeltaAngleX
	_m->DeltaAngleX = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEX(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting DeltaAngleY
	_m->DeltaAngleY = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEY(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting DeltaAngleZ
	_m->DeltaAngleZ = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEZ(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_EKF_EULER
int8_t Unpack_FRONT_IMU_EKF_EULER(FRONT_IMU_EKF_EULER_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting Roll
	_m->Roll = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_ROLL(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting Pitch
	_m->Pitch = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_PITCH(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting Yaw
	_m->Yaw = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_YAW(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_EKF_VELOCITY_BODY
int8_t Unpack_FRONT_IMU_EKF_VELOCITY_BODY(FRONT_IMU_EKF_VELOCITY_BODY_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting VelocityX
	_m->VelocityX = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYX(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting VelocityY
	_m->VelocityY = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYY(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting VelocityZ
	_m->VelocityZ = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYZ(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_EKF_NED_VEL_ACC
int8_t Unpack_FRONT_IMU_EKF_NED_VEL_ACC(FRONT_IMU_EKF_NED_VEL_ACC_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting VelocityAccN
	_m->VelocityAccN = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCN(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting VelocityAccE
	_m->VelocityAccE = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCE(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting VelocityAccD
	_m->VelocityAccD = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCD(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_GPU_VEL_INFO
int8_t Unpack_FRONT_IMU_GPU_VEL_INFO(FRONT_IMU_GPU_VEL_INFO_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting TimeStamp
	_m->TimeStamp = ((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u) | ((_d[2] & 0xffu) << 16u) | ((_d[3] & 0xffu) << 24u);
	// Extracting Status
	_m->Status = ((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u) | ((_d[6] & 0xffu) << 16u) | ((_d[7] & 0xffu) << 24u);

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_GPS_POS_INFO
int8_t Unpack_FRONT_IMU_GPS_POS_INFO(FRONT_IMU_GPS_POS_INFO_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting TimeStamp
	_m->TimeStamp = ((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u) | ((_d[2] & 0xffu) << 16u) | ((_d[3] & 0xffu) << 24u);
	// Extracting Status
	_m->Status = ((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u) | ((_d[6] & 0xffu) << 16u) | ((_d[7] & 0xffu) << 24u);

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_SLIPS
int8_t Unpack_FRONT_IMU_SLIPS(FRONT_IMU_SLIPS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 7u) return STATUS_ERROR;

	// Extracting AngleTrack
	_m->AngleTrack = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_SLIPS_ANGLETRACK(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting AngleSlip
	_m->AngleSlip = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_SLIPS_ANGLESLIP(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting CurvatureRadius
	_m->CurvatureRadius = (double)UNPACK_SCALE_OFFSET_FRONT_IMU_SLIPS_CURVATURERADIUS(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));
	// Extracting AutoStatus
	_m->AutoStatus = ((_d[6] & 0xffu) >> 0u);

	return STATUS_OK;
}

// Unpack signals from REAR_IMU_GYROS
int8_t Unpack_REAR_IMU_GYROS(REAR_IMU_GYROS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting GyroX
	_m->GyroX = (double)UNPACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROX(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting GyroY
	_m->GyroY = (double)UNPACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROY(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting GyroZ
	_m->GyroZ = (double)UNPACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROZ(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from REAR_IMU_GPS_POS
int8_t Unpack_REAR_IMU_GPS_POS(REAR_IMU_GPS_POS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting Latitude
	_m->Latitude = (double)UNPACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LATITUDE(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u) | ((_d[2] & 0xffu) << 16u) | ((_d[3] & 0xffu) << 24u));
	// Extracting Longitude
	_m->Longitude = (double)UNPACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LONGITUDE(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u) | ((_d[6] & 0xffu) << 16u) | ((_d[7] & 0xffu) << 24u));

	return STATUS_OK;
}

// Unpack signals from REAR_IMU_GPS1_VELOCITY
int8_t Unpack_REAR_IMU_GPS1_VELOCITY(REAR_IMU_GPS1_VELOCITY_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting VelocityN
	_m->VelocityN = (double)UNPACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYN(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting VelocityE
	_m->VelocityE = (double)UNPACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYE(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting VelocityD
	_m->VelocityD = (double)UNPACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYD(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from REAR_IMU_ACCELERATION
int8_t Unpack_REAR_IMU_ACCELERATION(REAR_IMU_ACCELERATION_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting AccelX
	_m->AccelX = (double)UNPACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELX(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting AccelY
	_m->AccelY = (double)UNPACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELY(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting AccelZ
	_m->AccelZ = (double)UNPACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELZ(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from REAR_IMU_ANGLE_RATES
int8_t Unpack_REAR_IMU_ANGLE_RATES(REAR_IMU_ANGLE_RATES_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting DeltaAngleX
	_m->DeltaAngleX = (double)UNPACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEX(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting DeltaAngleY
	_m->DeltaAngleY = (double)UNPACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEY(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting DeltaAngleZ
	_m->DeltaAngleZ = (double)UNPACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEZ(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from REAR_IMU_EKF_EULER
int8_t Unpack_REAR_IMU_EKF_EULER(REAR_IMU_EKF_EULER_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting Roll
	_m->Roll = (double)UNPACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_ROLL(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting Pitch
	_m->Pitch = (double)UNPACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_PITCH(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting Yaw
	_m->Yaw = (double)UNPACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_YAW(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from REAR_IMU_EKF_VELOCITY_BODY
int8_t Unpack_REAR_IMU_EKF_VELOCITY_BODY(REAR_IMU_EKF_VELOCITY_BODY_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting VelocityX
	_m->VelocityX = (double)UNPACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYX(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting VelocityY
	_m->VelocityY = (double)UNPACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYY(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting VelocityZ
	_m->VelocityZ = (double)UNPACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYZ(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from REAR_IMU_EKF_NED_VEL_ACC
int8_t Unpack_REAR_IMU_EKF_NED_VEL_ACC(REAR_IMU_EKF_NED_VEL_ACC_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting VelocityAccN
	_m->VelocityAccN = (double)UNPACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCN(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting VelocityAccE
	_m->VelocityAccE = (double)UNPACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCE(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting VelocityAccD
	_m->VelocityAccD = (double)UNPACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCD(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));

	return STATUS_OK;
}

// Unpack signals from REAR_IMU_GPU_VEL_INFO
int8_t Unpack_REAR_IMU_GPU_VEL_INFO(REAR_IMU_GPU_VEL_INFO_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting TimeStamp
	_m->TimeStamp = ((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u) | ((_d[2] & 0xffu) << 16u) | ((_d[3] & 0xffu) << 24u);
	// Extracting Status
	_m->Status = ((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u) | ((_d[6] & 0xffu) << 16u) | ((_d[7] & 0xffu) << 24u);

	return STATUS_OK;
}

// Unpack signals from REAR_IMU_GPS_POS_INFO
int8_t Unpack_REAR_IMU_GPS_POS_INFO(REAR_IMU_GPS_POS_INFO_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting TimeStamp
	_m->TimeStamp = ((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u) | ((_d[2] & 0xffu) << 16u) | ((_d[3] & 0xffu) << 24u);
	// Extracting Status
	_m->Status = ((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u) | ((_d[6] & 0xffu) << 16u) | ((_d[7] & 0xffu) << 24u);

	return STATUS_OK;
}

// Unpack signals from REAR_IMU_SLIPS
int8_t Unpack_REAR_IMU_SLIPS(REAR_IMU_SLIPS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 7u) return STATUS_ERROR;

	// Extracting AngleTrack
	_m->AngleTrack = (double)UNPACK_SCALE_OFFSET_REAR_IMU_SLIPS_ANGLETRACK(((_d[0] & 0xffu) >> 0u) | ((_d[1] & 0xffu) << 8u));
	// Extracting AngleSlip
	_m->AngleSlip = (double)UNPACK_SCALE_OFFSET_REAR_IMU_SLIPS_ANGLESLIP(((_d[2] & 0xffu) >> 0u) | ((_d[3] & 0xffu) << 8u));
	// Extracting CurvatureRadius
	_m->CurvatureRadius = (double)UNPACK_SCALE_OFFSET_REAR_IMU_SLIPS_CURVATURERADIUS(((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u));
	// Extracting AutoStatus
	_m->AutoStatus = ((_d[6] & 0xffu) >> 0u);

	return STATUS_OK;
}

// Unpack signals from FRONT_IMU_STATUS_03
int8_t Unpack_FRONT_IMU_STATUS_03(FRONT_IMU_STATUS_03_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting SolutionMode
	_m->SolutionMode = ((_d[0] & 0x0fu) >> 0u);
	// Extracting SolAttitudeValid
	_m->SolAttitudeValid = ((_d[0] & 0x10u) >> 4u);
	// Extracting SolHeadingValid
	_m->SolHeadingValid = ((_d[0] & 0x20u) >> 5u);
	// Extracting SolVelocityValid
	_m->SolVelocityValid = ((_d[0] & 0x40u) >> 6u);
	// Extracting SolPositionValid
	_m->SolPositionValid = ((_d[0] & 0x80u) >> 7u);
	// Extracting HeaveStatus
	_m->HeaveStatus = ((_d[4] & 0xffu) >> 0u) | ((_d[5] & 0xffu) << 8u);

	return STATUS_OK;
}

// Unpack signals from WHEEL_SPEEDS
int8_t Unpack_WHEEL_SPEEDS(WHEEL_SPEEDS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting WheelSpeedFR
	_m->WheelSpeedFR = (double)UNPACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDFR(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting WheelSpeedFL
	_m->WheelSpeedFL = (double)UNPACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDFL(((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u));
	// Extracting WheelSpeedRR
	_m->WheelSpeedRR = (double)UNPACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDRR(((_d[4] & 0xffu) << 8u) | ((_d[5] & 0xffu) >> 0u));
	// Extracting WheelSpeedRL
	_m->WheelSpeedRL = (double)UNPACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDRL(((_d[6] & 0xffu) << 8u) | ((_d[7] & 0xffu) >> 0u));

	return STATUS_OK;
}

// Unpack signals from REAR_DAMPERS_REAR_BRAKES
int8_t Unpack_REAR_DAMPERS_REAR_BRAKES(REAR_DAMPERS_REAR_BRAKES_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting DamperTravelRR
	_m->DamperTravelRR = (double)UNPACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_DAMPERTRAVELRR(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting DamperTravelRL
	_m->DamperTravelRL = (double)UNPACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_DAMPERTRAVELRL(((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u));
	// Extracting BrakeTempRR
	_m->BrakeTempRR = (double)UNPACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_BRAKETEMPRR(((_d[4] & 0xffu) << 8u) | ((_d[5] & 0xffu) >> 0u));
	// Extracting BrakeTempRL
	_m->BrakeTempRL = (double)UNPACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_BRAKETEMPRL(((_d[6] & 0xffu) << 8u) | ((_d[7] & 0xffu) >> 0u));

	return STATUS_OK;
}

// Unpack signals from BRAKE_PRESSURES
int8_t Unpack_BRAKE_PRESSURES(BRAKE_PRESSURES_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 4u) return STATUS_ERROR;

	// Extracting BrakePressureFront
	_m->BrakePressureFront = (double)UNPACK_SCALE_OFFSET_BRAKE_PRESSURES_BRAKEPRESSUREFRONT(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting BrakePressureRear
	_m->BrakePressureRear = (double)UNPACK_SCALE_OFFSET_BRAKE_PRESSURES_BRAKEPRESSUREREAR(((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u));

	return STATUS_OK;
}

// Unpack signals from COOLANT_PRESSURES
int8_t Unpack_COOLANT_PRESSURES(COOLANT_PRESSURES_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting CoolantInverterPressure
	_m->CoolantInverterPressure = (double)UNPACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTINVERTERPRESSURE(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting CoolantPressureFanIN
	_m->CoolantPressureFanIN = (double)UNPACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTPRESSUREFANIN(((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u));
	// Extracting CoolantPressureFanOUT
	_m->CoolantPressureFanOUT = (double)UNPACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTPRESSUREFANOUT(((_d[4] & 0xffu) << 8u) | ((_d[5] & 0xffu) >> 0u));

	return STATUS_OK;
}

// Unpack signals from COOLANT_TEMPS
int8_t Unpack_COOLANT_TEMPS(COOLANT_TEMPS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	// Extracting CoolantTempInverter
	_m->CoolantTempInverter = (double)UNPACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPINVERTER(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting CoolantTempFanIN
	_m->CoolantTempFanIN = (double)UNPACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPFANIN(((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u));
	// Extracting CoolantTempFanOUT
	_m->CoolantTempFanOUT = (double)UNPACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPFANOUT(((_d[4] & 0xffu) << 8u) | ((_d[5] & 0xffu) >> 0u));

	return STATUS_OK;
}

// Unpack signals from DRIVER_DISPLAY_SELECT
int8_t Unpack_DRIVER_DISPLAY_SELECT(DRIVER_DISPLAY_SELECT_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 1u) return STATUS_ERROR;

	// Extracting CurrentSelect
	_m->CurrentSelect = ((_d[0] & 0xffu) >> 0u);

	return STATUS_OK;
}

// Unpack signals from ECU_TractiveState
int8_t Unpack_ECU_TractiveState(ECU_TractiveState_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 1u) return STATUS_ERROR;

	// Extracting Tractive_State
	_m->Tractive_State = ((_d[0] & 0xf0u) >> 4u);

	return STATUS_OK;
}

// Unpack signals from ECU_STATUS_AND_LAPS
int8_t Unpack_ECU_STATUS_AND_LAPS(ECU_STATUS_AND_LAPS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting IMD
	_m->IMD = (double)UNPACK_SCALE_OFFSET_ECU_STATUS_AND_LAPS_IMD(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting LapBeacon
	_m->LapBeacon = ((_d[2] & 0xffu) << 24u) | ((_d[3] & 0xffu) << 16u) | ((_d[4] & 0xffu) << 8u) | ((_d[5] & 0xffu) >> 0u);
	// Extracting BatteryVoltage
	_m->BatteryVoltage = (double)UNPACK_SCALE_OFFSET_ECU_STATUS_AND_LAPS_BATTERYVOLTAGE(((_d[6] & 0xffu) >> 0u));

	return STATUS_OK;
}

// Unpack signals from DASHBOARD_PARAMS
int8_t Unpack_DASHBOARD_PARAMS(DASHBOARD_PARAMS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 1u) return STATUS_ERROR;

	// Extracting LaunchModeStatus
	_m->LaunchModeStatus = ((_d[0] & 0xc0u) >> 6u);
	// Extracting CruiseControlActive
	_m->CruiseControlActive = ((_d[0] & 0x20u) >> 5u);
	// Extracting RegenActive
	_m->RegenActive = ((_d[0] & 0x10u) >> 4u);
	// Extracting PedalSensitivity
	_m->PedalSensitivity = ((_d[0] & 0x0cu) >> 2u);

	return STATUS_OK;
}

// Unpack signals from JETSON_TX_INFO
int8_t Unpack_JETSON_TX_INFO(JETSON_TX_INFO_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 7u) return STATUS_ERROR;

	// Extracting Heartbeat
	_m->Heartbeat = ((_d[0] & 0x80u) >> 7u);
	// Extracting Ready
	_m->Ready = ((_d[0] & 0x40u) >> 6u);
	// Extracting MissionFinish
	_m->MissionFinish = ((_d[0] & 0x20u) >> 5u);
	// Extracting MissionSelected
	_m->MissionSelected = ((_d[0] & 0x1cu) >> 2u);
	// Extracting SpeedTarget
	_m->SpeedTarget = (double)UNPACK_SCALE_OFFSET_JETSON_TX_INFO_SPEEDTARGET(((_d[0] & 0x03u) << 14u) | ((_d[1] & 0xffu) << 6u) | ((_d[2] & 0xfcu) >> 2u));
	// Extracting LapCount
	_m->LapCount = ((_d[2] & 0x03u) << 6u) | ((_d[3] & 0xfcu) >> 2u);
	// Extracting ConesCountActual
	_m->ConesCountActual = ((_d[3] & 0x03u) << 6u) | ((_d[4] & 0xfcu) >> 2u);
	// Extracting ConesCountAll
	_m->ConesCountAll = ((_d[4] & 0x03u) << 6u) | ((_d[5] & 0xfcu) >> 2u);
	// Extracting RecorderState
	_m->RecorderState = ((_d[5] & 0x02u) >> 1u);

	return STATUS_OK;
}

// Unpack signals from AV_DEBUG_DISPLAY_1
int8_t Unpack_AV_DEBUG_DISPLAY_1(AV_DEBUG_DISPLAY_1_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 7u) return STATUS_ERROR;

	// Extracting ResConnCAN
	_m->ResConnCAN = ((_d[0] & 0x80u) >> 7u);
	// Extracting JetsonConnCAN
	_m->JetsonConnCAN = ((_d[0] & 0x40u) >> 6u);
	// Extracting JetsonLidarDataConn
	_m->JetsonLidarDataConn = ((_d[0] & 0x20u) >> 5u);
	// Extracting JetsonCameraDataConn
	_m->JetsonCameraDataConn = ((_d[0] & 0x10u) >> 4u);
	// Extracting SBGConn
	_m->SBGConn = ((_d[0] & 0x08u) >> 3u);
	// Extracting JetsonIP
	_m->JetsonIP = ((_d[0] & 0x07u) << 29u) | ((_d[1] & 0xffu) << 21u) | ((_d[2] & 0xffu) << 13u) | ((_d[3] & 0xffu) << 5u) | ((_d[4] & 0xf8u) >> 3u);
	// Extracting JetsonCpuTemp
	_m->JetsonCpuTemp = ((_d[4] & 0x07u) << 4u) | ((_d[5] & 0xf0u) >> 4u);
	// Extracting JetsonGpuTemp
	_m->JetsonGpuTemp = ((_d[5] & 0x0fu) << 3u) | ((_d[6] & 0xe0u) >> 5u);
	// Extracting ResPower
	_m->ResPower = ((_d[6] & 0x10u) >> 4u);

	return STATUS_OK;
}

// Unpack signals from AV_DEBUG_DISPLAY_2
int8_t Unpack_AV_DEBUG_DISPLAY_2(AV_DEBUG_DISPLAY_2_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 7u) return STATUS_ERROR;

	// Extracting JetsonPowerConsumption
	_m->JetsonPowerConsumption = ((_d[0] & 0xfeu) >> 1u);
	// Extracting JetsonPowerMode
	_m->JetsonPowerMode = ((_d[0] & 0x01u) << 1u) | ((_d[1] & 0x80u) >> 7u);
	// Extracting AvStartupStatus
	_m->AvStartupStatus = ((_d[1] & 0x78u) >> 3u);
	// Extracting SBGSolNStatus
	_m->SBGSolNStatus = ((_d[1] & 0x07u) >> 0u);
	// Extracting AsStateRos
	_m->AsStateRos = ((_d[2] & 0xe0u) >> 5u);
	// Extracting AsMissionRos
	_m->AsMissionRos = ((_d[2] & 0x1cu) >> 2u);
	// Extracting StanleyStatus
	_m->StanleyStatus = ((_d[2] & 0x03u) << 2u) | ((_d[3] & 0xc0u) >> 6u);
	// Extracting PurePursuitStatus
	_m->PurePursuitStatus = ((_d[3] & 0x3cu) >> 2u);
	// Extracting LidarNodeStatus
	_m->LidarNodeStatus = ((_d[3] & 0x02u) >> 1u);
	// Extracting CameraNodeStatus
	_m->CameraNodeStatus = ((_d[3] & 0x01u) >> 0u);
	// Extracting SensorFusionStatus
	_m->SensorFusionStatus = ((_d[4] & 0x80u) >> 7u);
	// Extracting LidarDriverStatus
	_m->LidarDriverStatus = ((_d[4] & 0x40u) >> 6u);
	// Extracting CameraDriverStatus
	_m->CameraDriverStatus = ((_d[4] & 0x20u) >> 5u);
	// Extracting WaypointNodeStatus
	_m->WaypointNodeStatus = ((_d[4] & 0x1eu) >> 1u);
	// Extracting SlamStatus
	_m->SlamStatus = ((_d[4] & 0x01u) << 3u) | ((_d[5] & 0xe0u) >> 5u);
	// Extracting StateEstimatorStatus
	_m->StateEstimatorStatus = ((_d[5] & 0x1eu) >> 1u);
	// Extracting MissionControllerStatus
	_m->MissionControllerStatus = ((_d[5] & 0x01u) << 3u) | ((_d[6] & 0xe0u) >> 5u);
	// Extracting BrakeHold
	_m->BrakeHold = ((_d[6] & 0x10u) >> 4u);

	return STATUS_OK;
}

// Unpack signals from AV_DEBUG_DISPLAY_3
int8_t Unpack_AV_DEBUG_DISPLAY_3(AV_DEBUG_DISPLAY_3_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting Longitude
	_m->Longitude = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_LONGITUDE(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting Latitude
	_m->Latitude = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_LATITUDE(((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u));
	// Extracting SBGAccX
	_m->SBGAccX = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_SBGACCX(((_d[4] & 0xffu) << 8u) | ((_d[5] & 0xffu) >> 0u));
	// Extracting SBGAccY
	_m->SBGAccY = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_SBGACCY(((_d[6] & 0xffu) << 8u) | ((_d[7] & 0xffu) >> 0u));

	return STATUS_OK;
}

// Unpack signals from AV_DEBUG_DISPLAY_4
int8_t Unpack_AV_DEBUG_DISPLAY_4(AV_DEBUG_DISPLAY_4_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting SteeringReqRos
	_m->SteeringReqRos = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_STEERINGREQROS(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting TorqueReqRos
	_m->TorqueReqRos = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_TORQUEREQROS(((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u));
	// Extracting VelocityReqRos
	_m->VelocityReqRos = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_VELOCITYREQROS(((_d[4] & 0xffu) << 8u) | ((_d[5] & 0xffu) >> 0u));
	// Extracting SteeringActual
	_m->SteeringActual = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_STEERINGACTUAL(((_d[6] & 0xffu) << 8u) | ((_d[7] & 0xffu) >> 0u));

	return STATUS_OK;
}

// Unpack signals from AV_DEBUG_DISPLAY_5
int8_t Unpack_AV_DEBUG_DISPLAY_5(AV_DEBUG_DISPLAY_5_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting VelocityActual
	_m->VelocityActual = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_VELOCITYACTUAL(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting TorqueActual
	_m->TorqueActual = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_TORQUEACTUAL(((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u));
	// Extracting AccelActual
	_m->AccelActual = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_ACCELACTUAL(((_d[4] & 0xffu) << 8u) | ((_d[5] & 0xffu) >> 0u));
	// Extracting AccelReqRos
	_m->AccelReqRos = (double)UNPACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_ACCELREQROS(((_d[6] & 0xffu) << 8u) | ((_d[7] & 0xffu) >> 0u));

	return STATUS_OK;
}

// Unpack signals from ODOMETER_UPDATE
int8_t Unpack_ODOMETER_UPDATE(ODOMETER_UPDATE_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 3u) return STATUS_ERROR;

	// Extracting Odometer
	_m->Odometer = ((_d[0] & 0xffu) << 16u) | ((_d[1] & 0xffu) << 8u) | ((_d[2] & 0xffu) >> 0u);

	return STATUS_OK;
}

// Unpack signals from ODOMETER_SAVE
int8_t Unpack_ODOMETER_SAVE(ODOMETER_SAVE_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 3u) return STATUS_ERROR;

	// Extracting OdometerS
	_m->OdometerS = ((_d[0] & 0xffu) << 16u) | ((_d[1] & 0xffu) << 8u) | ((_d[2] & 0xffu) >> 0u);

	return STATUS_OK;
}

// Unpack signals from TIMESTAMP_READING
int8_t Unpack_TIMESTAMP_READING(TIMESTAMP_READING_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 4u) return STATUS_ERROR;

	// Extracting Time_UNIX
	_m->Time_UNIX = ((_d[0] & 0xffu) << 24u) | ((_d[1] & 0xffu) << 16u) | ((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u);

	return STATUS_OK;
}

// Unpack signals from RTD_MODES_FLASH
int8_t Unpack_RTD_MODES_FLASH(RTD_MODES_FLASH_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 2u) return STATUS_ERROR;

	// Extracting RTDMode
	_m->RTDMode = ((_d[0] & 0xe0u) >> 5u);
	// Extracting TorqueLimit
	_m->TorqueLimit = ((_d[0] & 0x18u) >> 3u);
	// Extracting PowerLimit
	_m->PowerLimit = ((_d[0] & 0x06u) >> 1u);
	// Extracting RegenLimit
	_m->RegenLimit = ((_d[0] & 0x01u) << 1u) | ((_d[1] & 0x80u) >> 7u);
	// Extracting RegenMode
	_m->RegenMode = ((_d[1] & 0x60u) >> 5u);
	// Extracting TorqueVectoringMode
	_m->TorqueVectoringMode = ((_d[1] & 0x18u) >> 3u);
	// Extracting SpeedSetPoint
	_m->SpeedSetPoint = ((_d[1] & 0x04u) >> 2u);
	// Extracting LaunchDisable
	_m->LaunchDisable = ((_d[1] & 0x02u) >> 1u);

	return STATUS_OK;
}

// Unpack signals from RTD_MODES_SAVE
int8_t Unpack_RTD_MODES_SAVE(RTD_MODES_SAVE_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 2u) return STATUS_ERROR;

	// Extracting RTDModeS
	_m->RTDModeS = ((_d[0] & 0xe0u) >> 5u);
	// Extracting TorqueLimitS
	_m->TorqueLimitS = ((_d[0] & 0x18u) >> 3u);
	// Extracting PowerLimitS
	_m->PowerLimitS = ((_d[0] & 0x06u) >> 1u);
	// Extracting RegenLimitS
	_m->RegenLimitS = ((_d[0] & 0x01u) << 1u) | ((_d[1] & 0x80u) >> 7u);
	// Extracting RegenModeS
	_m->RegenModeS = ((_d[1] & 0x60u) >> 5u);
	// Extracting TorqueVectoringModeS
	_m->TorqueVectoringModeS = ((_d[1] & 0x18u) >> 3u);
	// Extracting SpeedSetPointS
	_m->SpeedSetPointS = ((_d[1] & 0x04u) >> 2u);
	// Extracting LaunchDisableS
	_m->LaunchDisableS = ((_d[1] & 0x02u) >> 1u);

	return STATUS_OK;
}

// Unpack signals from RTD_MODES_UPDATE
int8_t Unpack_RTD_MODES_UPDATE(RTD_MODES_UPDATE_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 2u) return STATUS_ERROR;

	// Extracting RTDModeU
	_m->RTDModeU = ((_d[0] & 0xe0u) >> 5u);
	// Extracting TorqueLimitU
	_m->TorqueLimitU = ((_d[0] & 0x18u) >> 3u);
	// Extracting PowerLimitU
	_m->PowerLimitU = ((_d[0] & 0x06u) >> 1u);
	// Extracting RegenLimitU
	_m->RegenLimitU = ((_d[0] & 0x01u) << 1u) | ((_d[1] & 0x80u) >> 7u);
	// Extracting RegenModeU
	_m->RegenModeU = ((_d[1] & 0x60u) >> 5u);
	// Extracting TorqueVectoringModeU
	_m->TorqueVectoringModeU = ((_d[1] & 0x18u) >> 3u);
	// Extracting SpeedSetPointU
	_m->SpeedSetPointU = ((_d[1] & 0x04u) >> 2u);
	// Extracting LaunchDisableU
	_m->LaunchDisableU = ((_d[1] & 0x02u) >> 1u);

	return STATUS_OK;
}

// Unpack signals from CANHUB_TEMP_SENSORS
int8_t Unpack_CANHUB_TEMP_SENSORS(CANHUB_TEMP_SENSORS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting OilTempSensorLeft
	_m->OilTempSensorLeft = (double)UNPACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_OILTEMPSENSORLEFT(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting OilTempSensorRight
	_m->OilTempSensorRight = (double)UNPACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_OILTEMPSENSORRIGHT(((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u));
	// Extracting CoolingTempSensorLeft
	_m->CoolingTempSensorLeft = (double)UNPACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_COOLINGTEMPSENSORLEFT(((_d[4] & 0xffu) << 8u) | ((_d[5] & 0xffu) >> 0u));
	// Extracting CoolingTempSensorRight
	_m->CoolingTempSensorRight = (double)UNPACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_COOLINGTEMPSENSORRIGHT(((_d[6] & 0xffu) << 8u) | ((_d[7] & 0xffu) >> 0u));

	return STATUS_OK;
}

// Unpack signals from CANHUB_POTS
int8_t Unpack_CANHUB_POTS(CANHUB_POTS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 4u) return STATUS_ERROR;

	// Extracting CanhubPot1
	_m->CanhubPot1 = (double)UNPACK_SCALE_OFFSET_CANHUB_POTS_CANHUBPOT1(((_d[0] & 0xffu) << 4u) | ((_d[1] & 0xf0u) >> 4u));
	// Extracting CanhubPot2
	_m->CanhubPot2 = (double)UNPACK_SCALE_OFFSET_CANHUB_POTS_CANHUBPOT2(((_d[2] & 0xffu) << 4u) | ((_d[3] & 0xf0u) >> 4u));

	return STATUS_OK;
}

// Unpack signals from CANHUB_THERMOCOUPLES
int8_t Unpack_CANHUB_THERMOCOUPLES(CANHUB_THERMOCOUPLES_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting ThermoFL
	_m->ThermoFL = (double)UNPACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMOFL(((_d[0] & 0xffu) << 8u) | ((_d[1] & 0xffu) >> 0u));
	// Extracting ThermoFR
	_m->ThermoFR = (double)UNPACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMOFR(((_d[2] & 0xffu) << 8u) | ((_d[3] & 0xffu) >> 0u));
	// Extracting ThermoRL
	_m->ThermoRL = (double)UNPACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMORL(((_d[4] & 0xffu) << 8u) | ((_d[5] & 0xffu) >> 0u));
	// Extracting ThermoRR
	_m->ThermoRR = (double)UNPACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMORR(((_d[6] & 0xffu) << 8u) | ((_d[7] & 0xffu) >> 0u));

	return STATUS_OK;
}

// Unpack signals from CANHUB_STRAIN_LINKS
int8_t Unpack_CANHUB_STRAIN_LINKS(CANHUB_STRAIN_LINKS_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	// Extracting LinkStrain1
	_m->LinkStrain1 = ((_d[0] & 0xffu) << 4u) | ((_d[1] & 0xf0u) >> 4u);
	// Extracting LinkStrain2
	_m->LinkStrain2 = ((_d[2] & 0xffu) << 4u) | ((_d[3] & 0xf0u) >> 4u);
	// Extracting LinkStrain3
	_m->LinkStrain3 = ((_d[4] & 0xffu) << 4u) | ((_d[5] & 0xf0u) >> 4u);
	// Extracting LinkStrain4
	_m->LinkStrain4 = ((_d[6] & 0xffu) << 4u) | ((_d[7] & 0xf0u) >> 4u);

	return STATUS_OK;
}

// Unpack signals from CANHUB_STRAIN_STEERING
int8_t Unpack_CANHUB_STRAIN_STEERING(CANHUB_STRAIN_STEERING_t* _m, const uint8_t* _d, uint8_t len) {
	if (len < 4u) return STATUS_ERROR;

	// Extracting SteeringStrain1
	_m->SteeringStrain1 = ((_d[0] & 0xffu) << 4u) | ((_d[1] & 0xf0u) >> 4u);
	// Extracting SteeringStrain2
	_m->SteeringStrain2 = ((_d[2] & 0xffu) << 4u) | ((_d[3] & 0xf0u) >> 4u);

	return STATUS_OK;
}

// Pack signals from PEDALS_AND_STEERING
int8_t Pack_PEDALS_AND_STEERING(const PEDALS_AND_STEERING_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_PEDALS_AND_STEERING_STEERINGANGLE(_m->SteeringAngle)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_PEDALS_AND_STEERING_STEERINGANGLE(_m->SteeringAngle)) << 0u) & 0xffu);
	_d[2] |= (( ((uint16_t)(_m->APPS)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint16_t)(_m->APPS)) << 0u) & 0xffu);
	_d[4] |= (( ((uint16_t)(_m->BPPS)) >> 8u) & 0xffu);
	_d[5] |= (( ((uint16_t)(_m->BPPS)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_GYROS
int8_t Pack_FRONT_IMU_GYROS(const FRONT_IMU_GYROS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROX(_m->GyroX)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROX(_m->GyroX)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROY(_m->GyroY)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROY(_m->GyroY)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROZ(_m->GyroZ)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GYROS_GYROZ(_m->GyroZ)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_GPS_POS
int8_t Pack_FRONT_IMU_GPS_POS(const FRONT_IMU_GPS_POS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint32_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LATITUDE(_m->Latitude)) << 0u) & 0xffu);
	_d[1] |= (( ((uint32_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LATITUDE(_m->Latitude)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint32_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LATITUDE(_m->Latitude)) >> 16u) & 0xffu);
	_d[3] |= (( ((uint32_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LATITUDE(_m->Latitude)) >> 24u) & 0xffu);
	_d[4] |= (( ((uint32_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LONGITUDE(_m->Longitude)) << 0u) & 0xffu);
	_d[5] |= (( ((uint32_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LONGITUDE(_m->Longitude)) >> 8u) & 0xffu);
	_d[6] |= (( ((uint32_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LONGITUDE(_m->Longitude)) >> 16u) & 0xffu);
	_d[7] |= (( ((uint32_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS_POS_LONGITUDE(_m->Longitude)) >> 24u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_GPS1_VELOCITY
int8_t Pack_FRONT_IMU_GPS1_VELOCITY(const FRONT_IMU_GPS1_VELOCITY_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYN(_m->VelocityN)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYN(_m->VelocityN)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYE(_m->VelocityE)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYE(_m->VelocityE)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYD(_m->VelocityD)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_GPS1_VELOCITY_VELOCITYD(_m->VelocityD)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_ACCELERATION
int8_t Pack_FRONT_IMU_ACCELERATION(const FRONT_IMU_ACCELERATION_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELX(_m->AccelX)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELX(_m->AccelX)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELY(_m->AccelY)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELY(_m->AccelY)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELZ(_m->AccelZ)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ACCELERATION_ACCELZ(_m->AccelZ)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_ANGLE_RATES
int8_t Pack_FRONT_IMU_ANGLE_RATES(const FRONT_IMU_ANGLE_RATES_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEX(_m->DeltaAngleX)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEX(_m->DeltaAngleX)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEY(_m->DeltaAngleY)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEY(_m->DeltaAngleY)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEZ(_m->DeltaAngleZ)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_ANGLE_RATES_DELTAANGLEZ(_m->DeltaAngleZ)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_EKF_EULER
int8_t Pack_FRONT_IMU_EKF_EULER(const FRONT_IMU_EKF_EULER_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_ROLL(_m->Roll)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_ROLL(_m->Roll)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_PITCH(_m->Pitch)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_PITCH(_m->Pitch)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_YAW(_m->Yaw)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_EULER_YAW(_m->Yaw)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_EKF_VELOCITY_BODY
int8_t Pack_FRONT_IMU_EKF_VELOCITY_BODY(const FRONT_IMU_EKF_VELOCITY_BODY_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYX(_m->VelocityX)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYX(_m->VelocityX)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYY(_m->VelocityY)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYY(_m->VelocityY)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYZ(_m->VelocityZ)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_VELOCITY_BODY_VELOCITYZ(_m->VelocityZ)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_EKF_NED_VEL_ACC
int8_t Pack_FRONT_IMU_EKF_NED_VEL_ACC(const FRONT_IMU_EKF_NED_VEL_ACC_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCN(_m->VelocityAccN)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCN(_m->VelocityAccN)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCE(_m->VelocityAccE)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCE(_m->VelocityAccE)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCD(_m->VelocityAccD)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_EKF_NED_VEL_ACC_VELOCITYACCD(_m->VelocityAccD)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_GPU_VEL_INFO
int8_t Pack_FRONT_IMU_GPU_VEL_INFO(const FRONT_IMU_GPU_VEL_INFO_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint32_t)(_m->TimeStamp)) << 0u) & 0xffu);
	_d[1] |= (( ((uint32_t)(_m->TimeStamp)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint32_t)(_m->TimeStamp)) >> 16u) & 0xffu);
	_d[3] |= (( ((uint32_t)(_m->TimeStamp)) >> 24u) & 0xffu);
	_d[4] |= (( ((uint32_t)(_m->Status)) << 0u) & 0xffu);
	_d[5] |= (( ((uint32_t)(_m->Status)) >> 8u) & 0xffu);
	_d[6] |= (( ((uint32_t)(_m->Status)) >> 16u) & 0xffu);
	_d[7] |= (( ((uint32_t)(_m->Status)) >> 24u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_GPS_POS_INFO
int8_t Pack_FRONT_IMU_GPS_POS_INFO(const FRONT_IMU_GPS_POS_INFO_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint32_t)(_m->TimeStamp)) << 0u) & 0xffu);
	_d[1] |= (( ((uint32_t)(_m->TimeStamp)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint32_t)(_m->TimeStamp)) >> 16u) & 0xffu);
	_d[3] |= (( ((uint32_t)(_m->TimeStamp)) >> 24u) & 0xffu);
	_d[4] |= (( ((uint32_t)(_m->Status)) << 0u) & 0xffu);
	_d[5] |= (( ((uint32_t)(_m->Status)) >> 8u) & 0xffu);
	_d[6] |= (( ((uint32_t)(_m->Status)) >> 16u) & 0xffu);
	_d[7] |= (( ((uint32_t)(_m->Status)) >> 24u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_SLIPS
int8_t Pack_FRONT_IMU_SLIPS(const FRONT_IMU_SLIPS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 7u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 7u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_SLIPS_ANGLETRACK(_m->AngleTrack)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_SLIPS_ANGLETRACK(_m->AngleTrack)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_SLIPS_ANGLESLIP(_m->AngleSlip)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_SLIPS_ANGLESLIP(_m->AngleSlip)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_SLIPS_CURVATURERADIUS(_m->CurvatureRadius)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_FRONT_IMU_SLIPS_CURVATURERADIUS(_m->CurvatureRadius)) >> 8u) & 0xffu);
	_d[6] |= (( ((uint8_t)(_m->AutoStatus)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_IMU_GYROS
int8_t Pack_REAR_IMU_GYROS(const REAR_IMU_GYROS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROX(_m->GyroX)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROX(_m->GyroX)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROY(_m->GyroY)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROY(_m->GyroY)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROZ(_m->GyroZ)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GYROS_GYROZ(_m->GyroZ)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_IMU_GPS_POS
int8_t Pack_REAR_IMU_GPS_POS(const REAR_IMU_GPS_POS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint32_t)PACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LATITUDE(_m->Latitude)) << 0u) & 0xffu);
	_d[1] |= (( ((uint32_t)PACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LATITUDE(_m->Latitude)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint32_t)PACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LATITUDE(_m->Latitude)) >> 16u) & 0xffu);
	_d[3] |= (( ((uint32_t)PACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LATITUDE(_m->Latitude)) >> 24u) & 0xffu);
	_d[4] |= (( ((uint32_t)PACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LONGITUDE(_m->Longitude)) << 0u) & 0xffu);
	_d[5] |= (( ((uint32_t)PACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LONGITUDE(_m->Longitude)) >> 8u) & 0xffu);
	_d[6] |= (( ((uint32_t)PACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LONGITUDE(_m->Longitude)) >> 16u) & 0xffu);
	_d[7] |= (( ((uint32_t)PACK_SCALE_OFFSET_REAR_IMU_GPS_POS_LONGITUDE(_m->Longitude)) >> 24u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_IMU_GPS1_VELOCITY
int8_t Pack_REAR_IMU_GPS1_VELOCITY(const REAR_IMU_GPS1_VELOCITY_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYN(_m->VelocityN)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYN(_m->VelocityN)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYE(_m->VelocityE)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYE(_m->VelocityE)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYD(_m->VelocityD)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_GPS1_VELOCITY_VELOCITYD(_m->VelocityD)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_IMU_ACCELERATION
int8_t Pack_REAR_IMU_ACCELERATION(const REAR_IMU_ACCELERATION_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELX(_m->AccelX)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELX(_m->AccelX)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELY(_m->AccelY)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELY(_m->AccelY)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELZ(_m->AccelZ)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ACCELERATION_ACCELZ(_m->AccelZ)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_IMU_ANGLE_RATES
int8_t Pack_REAR_IMU_ANGLE_RATES(const REAR_IMU_ANGLE_RATES_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEX(_m->DeltaAngleX)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEX(_m->DeltaAngleX)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEY(_m->DeltaAngleY)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEY(_m->DeltaAngleY)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEZ(_m->DeltaAngleZ)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_ANGLE_RATES_DELTAANGLEZ(_m->DeltaAngleZ)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_IMU_EKF_EULER
int8_t Pack_REAR_IMU_EKF_EULER(const REAR_IMU_EKF_EULER_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_ROLL(_m->Roll)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_ROLL(_m->Roll)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_PITCH(_m->Pitch)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_PITCH(_m->Pitch)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_YAW(_m->Yaw)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_EULER_YAW(_m->Yaw)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_IMU_EKF_VELOCITY_BODY
int8_t Pack_REAR_IMU_EKF_VELOCITY_BODY(const REAR_IMU_EKF_VELOCITY_BODY_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYX(_m->VelocityX)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYX(_m->VelocityX)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYY(_m->VelocityY)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYY(_m->VelocityY)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYZ(_m->VelocityZ)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_VELOCITY_BODY_VELOCITYZ(_m->VelocityZ)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_IMU_EKF_NED_VEL_ACC
int8_t Pack_REAR_IMU_EKF_NED_VEL_ACC(const REAR_IMU_EKF_NED_VEL_ACC_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCN(_m->VelocityAccN)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCN(_m->VelocityAccN)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCE(_m->VelocityAccE)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCE(_m->VelocityAccE)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCD(_m->VelocityAccD)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_EKF_NED_VEL_ACC_VELOCITYACCD(_m->VelocityAccD)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_IMU_GPU_VEL_INFO
int8_t Pack_REAR_IMU_GPU_VEL_INFO(const REAR_IMU_GPU_VEL_INFO_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint32_t)(_m->TimeStamp)) << 0u) & 0xffu);
	_d[1] |= (( ((uint32_t)(_m->TimeStamp)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint32_t)(_m->TimeStamp)) >> 16u) & 0xffu);
	_d[3] |= (( ((uint32_t)(_m->TimeStamp)) >> 24u) & 0xffu);
	_d[4] |= (( ((uint32_t)(_m->Status)) << 0u) & 0xffu);
	_d[5] |= (( ((uint32_t)(_m->Status)) >> 8u) & 0xffu);
	_d[6] |= (( ((uint32_t)(_m->Status)) >> 16u) & 0xffu);
	_d[7] |= (( ((uint32_t)(_m->Status)) >> 24u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_IMU_GPS_POS_INFO
int8_t Pack_REAR_IMU_GPS_POS_INFO(const REAR_IMU_GPS_POS_INFO_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint32_t)(_m->TimeStamp)) << 0u) & 0xffu);
	_d[1] |= (( ((uint32_t)(_m->TimeStamp)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint32_t)(_m->TimeStamp)) >> 16u) & 0xffu);
	_d[3] |= (( ((uint32_t)(_m->TimeStamp)) >> 24u) & 0xffu);
	_d[4] |= (( ((uint32_t)(_m->Status)) << 0u) & 0xffu);
	_d[5] |= (( ((uint32_t)(_m->Status)) >> 8u) & 0xffu);
	_d[6] |= (( ((uint32_t)(_m->Status)) >> 16u) & 0xffu);
	_d[7] |= (( ((uint32_t)(_m->Status)) >> 24u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_IMU_SLIPS
int8_t Pack_REAR_IMU_SLIPS(const REAR_IMU_SLIPS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 7u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 7u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_SLIPS_ANGLETRACK(_m->AngleTrack)) << 0u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_SLIPS_ANGLETRACK(_m->AngleTrack)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_SLIPS_ANGLESLIP(_m->AngleSlip)) << 0u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_SLIPS_ANGLESLIP(_m->AngleSlip)) >> 8u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_SLIPS_CURVATURERADIUS(_m->CurvatureRadius)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_IMU_SLIPS_CURVATURERADIUS(_m->CurvatureRadius)) >> 8u) & 0xffu);
	_d[6] |= (( ((uint8_t)(_m->AutoStatus)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from FRONT_IMU_STATUS_03
int8_t Pack_FRONT_IMU_STATUS_03(const FRONT_IMU_STATUS_03_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint8_t)(_m->SolutionMode)) << 0u) & 0x0fu);
	_d[0] |= (( ((uint8_t)(_m->SolAttitudeValid)) << 4u) & 0x10u);
	_d[0] |= (( ((uint8_t)(_m->SolHeadingValid)) << 5u) & 0x20u);
	_d[0] |= (( ((uint8_t)(_m->SolVelocityValid)) << 6u) & 0x40u);
	_d[0] |= (( ((uint8_t)(_m->SolPositionValid)) << 7u) & 0x80u);
	_d[4] |= (( ((uint16_t)(_m->HeaveStatus)) << 0u) & 0xffu);
	_d[5] |= (( ((uint16_t)(_m->HeaveStatus)) >> 8u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from WHEEL_SPEEDS
int8_t Pack_WHEEL_SPEEDS(const WHEEL_SPEEDS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDFR(_m->WheelSpeedFR)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDFR(_m->WheelSpeedFR)) << 0u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDFL(_m->WheelSpeedFL)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDFL(_m->WheelSpeedFL)) << 0u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDRR(_m->WheelSpeedRR)) >> 8u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDRR(_m->WheelSpeedRR)) << 0u) & 0xffu);
	_d[6] |= (( ((uint16_t)PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDRL(_m->WheelSpeedRL)) >> 8u) & 0xffu);
	_d[7] |= (( ((uint16_t)PACK_SCALE_OFFSET_WHEEL_SPEEDS_WHEELSPEEDRL(_m->WheelSpeedRL)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from REAR_DAMPERS_REAR_BRAKES
int8_t Pack_REAR_DAMPERS_REAR_BRAKES(const REAR_DAMPERS_REAR_BRAKES_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_DAMPERTRAVELRR(_m->DamperTravelRR)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_DAMPERTRAVELRR(_m->DamperTravelRR)) << 0u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_DAMPERTRAVELRL(_m->DamperTravelRL)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_DAMPERTRAVELRL(_m->DamperTravelRL)) << 0u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_BRAKETEMPRR(_m->BrakeTempRR)) >> 8u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_BRAKETEMPRR(_m->BrakeTempRR)) << 0u) & 0xffu);
	_d[6] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_BRAKETEMPRL(_m->BrakeTempRL)) >> 8u) & 0xffu);
	_d[7] |= (( ((uint16_t)PACK_SCALE_OFFSET_REAR_DAMPERS_REAR_BRAKES_BRAKETEMPRL(_m->BrakeTempRL)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from BRAKE_PRESSURES
int8_t Pack_BRAKE_PRESSURES(const BRAKE_PRESSURES_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 4u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 4u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_BRAKE_PRESSURES_BRAKEPRESSUREFRONT(_m->BrakePressureFront)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_BRAKE_PRESSURES_BRAKEPRESSUREFRONT(_m->BrakePressureFront)) << 0u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_BRAKE_PRESSURES_BRAKEPRESSUREREAR(_m->BrakePressureRear)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_BRAKE_PRESSURES_BRAKEPRESSUREREAR(_m->BrakePressureRear)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from COOLANT_PRESSURES
int8_t Pack_COOLANT_PRESSURES(const COOLANT_PRESSURES_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTINVERTERPRESSURE(_m->CoolantInverterPressure)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTINVERTERPRESSURE(_m->CoolantInverterPressure)) << 0u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTPRESSUREFANIN(_m->CoolantPressureFanIN)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTPRESSUREFANIN(_m->CoolantPressureFanIN)) << 0u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTPRESSUREFANOUT(_m->CoolantPressureFanOUT)) >> 8u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_PRESSURES_COOLANTPRESSUREFANOUT(_m->CoolantPressureFanOUT)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from COOLANT_TEMPS
int8_t Pack_COOLANT_TEMPS(const COOLANT_TEMPS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 6u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 6u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPINVERTER(_m->CoolantTempInverter)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPINVERTER(_m->CoolantTempInverter)) << 0u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPFANIN(_m->CoolantTempFanIN)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPFANIN(_m->CoolantTempFanIN)) << 0u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPFANOUT(_m->CoolantTempFanOUT)) >> 8u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_COOLANT_TEMPS_COOLANTTEMPFANOUT(_m->CoolantTempFanOUT)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from DRIVER_DISPLAY_SELECT
int8_t Pack_DRIVER_DISPLAY_SELECT(const DRIVER_DISPLAY_SELECT_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 1u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 1u; _d[i++] = 0u);

	_d[0] |= (( ((uint8_t)(_m->CurrentSelect)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from ECU_TractiveState
int8_t Pack_ECU_TractiveState(const ECU_TractiveState_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 1u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 1u; _d[i++] = 0u);

	_d[0] |= (( ((uint8_t)(_m->Tractive_State)) << 4u) & 0xf0u);

	return STATUS_OK;
}

// Pack signals from ECU_STATUS_AND_LAPS
int8_t Pack_ECU_STATUS_AND_LAPS(const ECU_STATUS_AND_LAPS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_ECU_STATUS_AND_LAPS_IMD(_m->IMD)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_ECU_STATUS_AND_LAPS_IMD(_m->IMD)) << 0u) & 0xffu);
	_d[2] |= (( ((uint32_t)(_m->LapBeacon)) >> 24u) & 0xffu);
	_d[3] |= (( ((uint32_t)(_m->LapBeacon)) >> 16u) & 0xffu);
	_d[4] |= (( ((uint32_t)(_m->LapBeacon)) >> 8u) & 0xffu);
	_d[5] |= (( ((uint32_t)(_m->LapBeacon)) << 0u) & 0xffu);
	_d[6] |= (( ((uint8_t)PACK_SCALE_OFFSET_ECU_STATUS_AND_LAPS_BATTERYVOLTAGE(_m->BatteryVoltage)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from DASHBOARD_PARAMS
int8_t Pack_DASHBOARD_PARAMS(const DASHBOARD_PARAMS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 1u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 1u; _d[i++] = 0u);

	_d[0] |= (( ((uint8_t)(_m->LaunchModeStatus)) << 6u) & 0xc0u);
	_d[0] |= (( ((uint8_t)(_m->CruiseControlActive)) << 5u) & 0x20u);
	_d[0] |= (( ((uint8_t)(_m->RegenActive)) << 4u) & 0x10u);
	_d[0] |= (( ((uint8_t)(_m->PedalSensitivity)) << 2u) & 0x0cu);

	return STATUS_OK;
}

// Pack signals from JETSON_TX_INFO
int8_t Pack_JETSON_TX_INFO(const JETSON_TX_INFO_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 7u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 7u; _d[i++] = 0u);

	_d[0] |= (( ((uint8_t)(_m->Heartbeat)) << 7u) & 0x80u);
	_d[0] |= (( ((uint8_t)(_m->Ready)) << 6u) & 0x40u);
	_d[0] |= (( ((uint8_t)(_m->MissionFinish)) << 5u) & 0x20u);
	_d[0] |= (( ((uint8_t)(_m->MissionSelected)) << 2u) & 0x1cu);
	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_JETSON_TX_INFO_SPEEDTARGET(_m->SpeedTarget)) >> 14u) & 0x03u);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_JETSON_TX_INFO_SPEEDTARGET(_m->SpeedTarget)) >> 6u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_JETSON_TX_INFO_SPEEDTARGET(_m->SpeedTarget)) << 2u) & 0xfcu);
	_d[2] |= (( ((uint8_t)(_m->LapCount)) >> 6u) & 0x03u);
	_d[3] |= (( ((uint8_t)(_m->LapCount)) << 2u) & 0xfcu);
	_d[3] |= (( ((uint8_t)(_m->ConesCountActual)) >> 6u) & 0x03u);
	_d[4] |= (( ((uint8_t)(_m->ConesCountActual)) << 2u) & 0xfcu);
	_d[4] |= (( ((uint8_t)(_m->ConesCountAll)) >> 6u) & 0x03u);
	_d[5] |= (( ((uint8_t)(_m->ConesCountAll)) << 2u) & 0xfcu);
	_d[5] |= (( ((uint8_t)(_m->RecorderState)) << 1u) & 0x02u);

	return STATUS_OK;
}

// Pack signals from AV_DEBUG_DISPLAY_1
int8_t Pack_AV_DEBUG_DISPLAY_1(const AV_DEBUG_DISPLAY_1_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 7u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 7u; _d[i++] = 0u);

	_d[0] |= (( ((uint8_t)(_m->ResConnCAN)) << 7u) & 0x80u);
	_d[0] |= (( ((uint8_t)(_m->JetsonConnCAN)) << 6u) & 0x40u);
	_d[0] |= (( ((uint8_t)(_m->JetsonLidarDataConn)) << 5u) & 0x20u);
	_d[0] |= (( ((uint8_t)(_m->JetsonCameraDataConn)) << 4u) & 0x10u);
	_d[0] |= (( ((uint8_t)(_m->SBGConn)) << 3u) & 0x08u);
	_d[0] |= (( ((uint32_t)(_m->JetsonIP)) >> 29u) & 0x07u);
	_d[1] |= (( ((uint32_t)(_m->JetsonIP)) >> 21u) & 0xffu);
	_d[2] |= (( ((uint32_t)(_m->JetsonIP)) >> 13u) & 0xffu);
	_d[3] |= (( ((uint32_t)(_m->JetsonIP)) >> 5u) & 0xffu);
	_d[4] |= (( ((uint32_t)(_m->JetsonIP)) << 3u) & 0xf8u);
	_d[4] |= (( ((uint8_t)(_m->JetsonCpuTemp)) >> 4u) & 0x07u);
	_d[5] |= (( ((uint8_t)(_m->JetsonCpuTemp)) << 4u) & 0xf0u);
	_d[5] |= (( ((uint8_t)(_m->JetsonGpuTemp)) >> 3u) & 0x0fu);
	_d[6] |= (( ((uint8_t)(_m->JetsonGpuTemp)) << 5u) & 0xe0u);
	_d[6] |= (( ((uint8_t)(_m->ResPower)) << 4u) & 0x10u);

	return STATUS_OK;
}

// Pack signals from AV_DEBUG_DISPLAY_2
int8_t Pack_AV_DEBUG_DISPLAY_2(const AV_DEBUG_DISPLAY_2_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 7u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 7u; _d[i++] = 0u);

	_d[0] |= (( ((uint8_t)(_m->JetsonPowerConsumption)) << 1u) & 0xfeu);
	_d[0] |= (( ((uint8_t)(_m->JetsonPowerMode)) >> 1u) & 0x01u);
	_d[1] |= (( ((uint8_t)(_m->JetsonPowerMode)) << 7u) & 0x80u);
	_d[1] |= (( ((uint8_t)(_m->AvStartupStatus)) << 3u) & 0x78u);
	_d[1] |= (( ((uint8_t)(_m->SBGSolNStatus)) << 0u) & 0x07u);
	_d[2] |= (( ((uint8_t)(_m->AsStateRos)) << 5u) & 0xe0u);
	_d[2] |= (( ((uint8_t)(_m->AsMissionRos)) << 2u) & 0x1cu);
	_d[2] |= (( ((uint8_t)(_m->StanleyStatus)) >> 2u) & 0x03u);
	_d[3] |= (( ((uint8_t)(_m->StanleyStatus)) << 6u) & 0xc0u);
	_d[3] |= (( ((uint8_t)(_m->PurePursuitStatus)) << 2u) & 0x3cu);
	_d[3] |= (( ((uint8_t)(_m->LidarNodeStatus)) << 1u) & 0x02u);
	_d[3] |= (( ((uint8_t)(_m->CameraNodeStatus)) << 0u) & 0x01u);
	_d[4] |= (( ((uint8_t)(_m->SensorFusionStatus)) << 7u) & 0x80u);
	_d[4] |= (( ((uint8_t)(_m->LidarDriverStatus)) << 6u) & 0x40u);
	_d[4] |= (( ((uint8_t)(_m->CameraDriverStatus)) << 5u) & 0x20u);
	_d[4] |= (( ((uint8_t)(_m->WaypointNodeStatus)) << 1u) & 0x1eu);
	_d[4] |= (( ((uint8_t)(_m->SlamStatus)) >> 3u) & 0x01u);
	_d[5] |= (( ((uint8_t)(_m->SlamStatus)) << 5u) & 0xe0u);
	_d[5] |= (( ((uint8_t)(_m->StateEstimatorStatus)) << 1u) & 0x1eu);
	_d[5] |= (( ((uint8_t)(_m->MissionControllerStatus)) >> 3u) & 0x01u);
	_d[6] |= (( ((uint8_t)(_m->MissionControllerStatus)) << 5u) & 0xe0u);
	_d[6] |= (( ((uint8_t)(_m->BrakeHold)) << 4u) & 0x10u);

	return STATUS_OK;
}

// Pack signals from AV_DEBUG_DISPLAY_3
int8_t Pack_AV_DEBUG_DISPLAY_3(const AV_DEBUG_DISPLAY_3_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_LONGITUDE(_m->Longitude)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_LONGITUDE(_m->Longitude)) << 0u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_LATITUDE(_m->Latitude)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_LATITUDE(_m->Latitude)) << 0u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_SBGACCX(_m->SBGAccX)) >> 8u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_SBGACCX(_m->SBGAccX)) << 0u) & 0xffu);
	_d[6] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_SBGACCY(_m->SBGAccY)) >> 8u) & 0xffu);
	_d[7] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_3_SBGACCY(_m->SBGAccY)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from AV_DEBUG_DISPLAY_4
int8_t Pack_AV_DEBUG_DISPLAY_4(const AV_DEBUG_DISPLAY_4_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_STEERINGREQROS(_m->SteeringReqRos)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_STEERINGREQROS(_m->SteeringReqRos)) << 0u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_TORQUEREQROS(_m->TorqueReqRos)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_TORQUEREQROS(_m->TorqueReqRos)) << 0u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_VELOCITYREQROS(_m->VelocityReqRos)) >> 8u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_VELOCITYREQROS(_m->VelocityReqRos)) << 0u) & 0xffu);
	_d[6] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_STEERINGACTUAL(_m->SteeringActual)) >> 8u) & 0xffu);
	_d[7] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_4_STEERINGACTUAL(_m->SteeringActual)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from AV_DEBUG_DISPLAY_5
int8_t Pack_AV_DEBUG_DISPLAY_5(const AV_DEBUG_DISPLAY_5_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_VELOCITYACTUAL(_m->VelocityActual)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_VELOCITYACTUAL(_m->VelocityActual)) << 0u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_TORQUEACTUAL(_m->TorqueActual)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_TORQUEACTUAL(_m->TorqueActual)) << 0u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_ACCELACTUAL(_m->AccelActual)) >> 8u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_ACCELACTUAL(_m->AccelActual)) << 0u) & 0xffu);
	_d[6] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_ACCELREQROS(_m->AccelReqRos)) >> 8u) & 0xffu);
	_d[7] |= (( ((uint16_t)PACK_SCALE_OFFSET_AV_DEBUG_DISPLAY_5_ACCELREQROS(_m->AccelReqRos)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from ODOMETER_UPDATE
int8_t Pack_ODOMETER_UPDATE(const ODOMETER_UPDATE_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 3u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 3u; _d[i++] = 0u);

	_d[0] |= (( ((uint32_t)(_m->Odometer)) >> 16u) & 0xffu);
	_d[1] |= (( ((uint32_t)(_m->Odometer)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint32_t)(_m->Odometer)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from ODOMETER_SAVE
int8_t Pack_ODOMETER_SAVE(const ODOMETER_SAVE_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 3u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 3u; _d[i++] = 0u);

	_d[0] |= (( ((uint32_t)(_m->OdometerS)) >> 16u) & 0xffu);
	_d[1] |= (( ((uint32_t)(_m->OdometerS)) >> 8u) & 0xffu);
	_d[2] |= (( ((uint32_t)(_m->OdometerS)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from TIMESTAMP_READING
int8_t Pack_TIMESTAMP_READING(const TIMESTAMP_READING_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 4u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 4u; _d[i++] = 0u);

	_d[0] |= (( ((uint32_t)(_m->Time_UNIX)) >> 24u) & 0xffu);
	_d[1] |= (( ((uint32_t)(_m->Time_UNIX)) >> 16u) & 0xffu);
	_d[2] |= (( ((uint32_t)(_m->Time_UNIX)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint32_t)(_m->Time_UNIX)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from RTD_MODES_FLASH
int8_t Pack_RTD_MODES_FLASH(const RTD_MODES_FLASH_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 2u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 2u; _d[i++] = 0u);

	_d[0] |= (( ((uint8_t)(_m->RTDMode)) << 5u) & 0xe0u);
	_d[0] |= (( ((uint8_t)(_m->TorqueLimit)) << 3u) & 0x18u);
	_d[0] |= (( ((uint8_t)(_m->PowerLimit)) << 1u) & 0x06u);
	_d[0] |= (( ((uint8_t)(_m->RegenLimit)) >> 1u) & 0x01u);
	_d[1] |= (( ((uint8_t)(_m->RegenLimit)) << 7u) & 0x80u);
	_d[1] |= (( ((uint8_t)(_m->RegenMode)) << 5u) & 0x60u);
	_d[1] |= (( ((uint8_t)(_m->TorqueVectoringMode)) << 3u) & 0x18u);
	_d[1] |= (( ((uint8_t)(_m->SpeedSetPoint)) << 2u) & 0x04u);
	_d[1] |= (( ((uint8_t)(_m->LaunchDisable)) << 1u) & 0x02u);

	return STATUS_OK;
}

// Pack signals from RTD_MODES_SAVE
int8_t Pack_RTD_MODES_SAVE(const RTD_MODES_SAVE_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 2u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 2u; _d[i++] = 0u);

	_d[0] |= (( ((uint8_t)(_m->RTDModeS)) << 5u) & 0xe0u);
	_d[0] |= (( ((uint8_t)(_m->TorqueLimitS)) << 3u) & 0x18u);
	_d[0] |= (( ((uint8_t)(_m->PowerLimitS)) << 1u) & 0x06u);
	_d[0] |= (( ((uint8_t)(_m->RegenLimitS)) >> 1u) & 0x01u);
	_d[1] |= (( ((uint8_t)(_m->RegenLimitS)) << 7u) & 0x80u);
	_d[1] |= (( ((uint8_t)(_m->RegenModeS)) << 5u) & 0x60u);
	_d[1] |= (( ((uint8_t)(_m->TorqueVectoringModeS)) << 3u) & 0x18u);
	_d[1] |= (( ((uint8_t)(_m->SpeedSetPointS)) << 2u) & 0x04u);
	_d[1] |= (( ((uint8_t)(_m->LaunchDisableS)) << 1u) & 0x02u);

	return STATUS_OK;
}

// Pack signals from RTD_MODES_UPDATE
int8_t Pack_RTD_MODES_UPDATE(const RTD_MODES_UPDATE_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 2u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 2u; _d[i++] = 0u);

	_d[0] |= (( ((uint8_t)(_m->RTDModeU)) << 5u) & 0xe0u);
	_d[0] |= (( ((uint8_t)(_m->TorqueLimitU)) << 3u) & 0x18u);
	_d[0] |= (( ((uint8_t)(_m->PowerLimitU)) << 1u) & 0x06u);
	_d[0] |= (( ((uint8_t)(_m->RegenLimitU)) >> 1u) & 0x01u);
	_d[1] |= (( ((uint8_t)(_m->RegenLimitU)) << 7u) & 0x80u);
	_d[1] |= (( ((uint8_t)(_m->RegenModeU)) << 5u) & 0x60u);
	_d[1] |= (( ((uint8_t)(_m->TorqueVectoringModeU)) << 3u) & 0x18u);
	_d[1] |= (( ((uint8_t)(_m->SpeedSetPointU)) << 2u) & 0x04u);
	_d[1] |= (( ((uint8_t)(_m->LaunchDisableU)) << 1u) & 0x02u);

	return STATUS_OK;
}

// Pack signals from CANHUB_TEMP_SENSORS
int8_t Pack_CANHUB_TEMP_SENSORS(const CANHUB_TEMP_SENSORS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_OILTEMPSENSORLEFT(_m->OilTempSensorLeft)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_OILTEMPSENSORLEFT(_m->OilTempSensorLeft)) << 0u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_OILTEMPSENSORRIGHT(_m->OilTempSensorRight)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_OILTEMPSENSORRIGHT(_m->OilTempSensorRight)) << 0u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_COOLINGTEMPSENSORLEFT(_m->CoolingTempSensorLeft)) >> 8u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_COOLINGTEMPSENSORLEFT(_m->CoolingTempSensorLeft)) << 0u) & 0xffu);
	_d[6] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_COOLINGTEMPSENSORRIGHT(_m->CoolingTempSensorRight)) >> 8u) & 0xffu);
	_d[7] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_TEMP_SENSORS_COOLINGTEMPSENSORRIGHT(_m->CoolingTempSensorRight)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from CANHUB_POTS
int8_t Pack_CANHUB_POTS(const CANHUB_POTS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 4u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 4u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_POTS_CANHUBPOT1(_m->CanhubPot1)) >> 4u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_POTS_CANHUBPOT1(_m->CanhubPot1)) << 4u) & 0xf0u);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_POTS_CANHUBPOT2(_m->CanhubPot2)) >> 4u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_POTS_CANHUBPOT2(_m->CanhubPot2)) << 4u) & 0xf0u);

	return STATUS_OK;
}

// Pack signals from CANHUB_THERMOCOUPLES
int8_t Pack_CANHUB_THERMOCOUPLES(const CANHUB_THERMOCOUPLES_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMOFL(_m->ThermoFL)) >> 8u) & 0xffu);
	_d[1] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMOFL(_m->ThermoFL)) << 0u) & 0xffu);
	_d[2] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMOFR(_m->ThermoFR)) >> 8u) & 0xffu);
	_d[3] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMOFR(_m->ThermoFR)) << 0u) & 0xffu);
	_d[4] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMORL(_m->ThermoRL)) >> 8u) & 0xffu);
	_d[5] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMORL(_m->ThermoRL)) << 0u) & 0xffu);
	_d[6] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMORR(_m->ThermoRR)) >> 8u) & 0xffu);
	_d[7] |= (( ((uint16_t)PACK_SCALE_OFFSET_CANHUB_THERMOCOUPLES_THERMORR(_m->ThermoRR)) << 0u) & 0xffu);

	return STATUS_OK;
}

// Pack signals from CANHUB_STRAIN_LINKS
int8_t Pack_CANHUB_STRAIN_LINKS(const CANHUB_STRAIN_LINKS_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 8u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 8u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)(_m->LinkStrain1)) >> 4u) & 0xffu);
	_d[1] |= (( ((uint16_t)(_m->LinkStrain1)) << 4u) & 0xf0u);
	_d[2] |= (( ((uint16_t)(_m->LinkStrain2)) >> 4u) & 0xffu);
	_d[3] |= (( ((uint16_t)(_m->LinkStrain2)) << 4u) & 0xf0u);
	_d[4] |= (( ((uint16_t)(_m->LinkStrain3)) >> 4u) & 0xffu);
	_d[5] |= (( ((uint16_t)(_m->LinkStrain3)) << 4u) & 0xf0u);
	_d[6] |= (( ((uint16_t)(_m->LinkStrain4)) >> 4u) & 0xffu);
	_d[7] |= (( ((uint16_t)(_m->LinkStrain4)) << 4u) & 0xf0u);

	return STATUS_OK;
}

// Pack signals from CANHUB_STRAIN_STEERING
int8_t Pack_CANHUB_STRAIN_STEERING(const CANHUB_STRAIN_STEERING_t* _m, uint8_t* _d, uint8_t len) {
	if (len < 4u) return STATUS_ERROR;

	for (uint8_t i = 0u; i < 4u; _d[i++] = 0u);

	_d[0] |= (( ((uint16_t)(_m->SteeringStrain1)) >> 4u) & 0xffu);
	_d[1] |= (( ((uint16_t)(_m->SteeringStrain1)) << 4u) & 0xf0u);
	_d[2] |= (( ((uint16_t)(_m->SteeringStrain2)) >> 4u) & 0xffu);
	_d[3] |= (( ((uint16_t)(_m->SteeringStrain2)) << 4u) & 0xf0u);

	return STATUS_OK;
}

