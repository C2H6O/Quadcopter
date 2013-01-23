#include "KalmanFilter.h"

int YZ_plane_angle;
int XZ_plane_angle;
int KFAngles[2];

//Kalman filter variables
MatrixMath matrixMath;

int* adxl345_angles_KF;
float* itg3200_gyroRate_KF;

float A[2][2];
float At[2][2];
float C[2];
float Sw[2][2];

float alpha_noise;
float bias_noise;
float Sz;
float dT = 0.01;

float YZ_x_k[2];
float YZ_x_k_1[2];
float YZ_K_Inn[2];
float YZ_Inn;
float YZ_s;
float YZ_K[2];
float YZ_K_temp1[2][2];
float YZ_K_temp2[2];
float YZ_s_temp1[2];
float YZ_s_temp2[1];
float YZ_P[2][2];
float YZ_A_P[2][2];
float YZ_A_P_At[2][2];
float YZ_K_C[2][2];
float YZ_K_C_P[2][2];
float YZ_K_C_P_At[2][2];
float YZ_A_P_At_K_C_P_At[2][2];

float XZ_x_k[2];
float XZ_x_k_1[2];
float XZ_K_Inn[2];
float XZ_Inn;
float XZ_s;
float XZ_K[2];
float XZ_K_temp1[2][2];
float XZ_K_temp2[2];
float XZ_s_temp1[2];
float XZ_s_temp2[1];
float XZ_P[2][2];
float XZ_A_P[2][2];
float XZ_A_P_At[2][2];
float XZ_K_C[2][2];
float XZ_K_C_P[2][2];
float XZ_K_C_P_At[2][2];
float XZ_A_P_At_K_C_P_At[2][2];

////TEST
float YZ_test_x_k[2];
float YZ_test_x_k_1[2];
float YZ_test_temp[2];

float XZ_test_x_k[2];
float XZ_test_x_k_1[2];
float XZ_test_temp[2];
//\\TEST

KalmanFilter::KalmanFilter(){
  dT = 0.01;    // Gyroscope update rate (100 Hz)
  alpha_noise = 0.01;
  bias_noise = 5;
  Sz = 20;
  
  A[0][0] = 1;
  A[0][1] = dT;
  A[1][0] = 0;
  A[1][1] = 1;

  At[0][0] = 1;
  At[0][1] = 0;
  At[1][0] = dT;
  At[1][1] = 1;

  C[0] = 1;
  C[1] = 0;

  YZ_P[0][0] = 1;
  YZ_P[0][1] = 1;
  YZ_P[1][0] = 1;
  YZ_P[1][1] = 1;
  
  YZ_x_k[0] = 1;
  YZ_x_k[1] = 1;

  XZ_P[0][0] = 1;
  XZ_P[0][1] = 1;
  XZ_P[1][0] = 1;
  XZ_P[1][1] = 1;   
  
  XZ_x_k[0] = 1;
  XZ_x_k[1] = 1;

  Sw[0][0] = alpha_noise*alpha_noise;
  Sw[0][1] = alpha_noise*bias_noise;
  Sw[1][0] = Sw[0][1];
  Sw[1][1] = bias_noise*bias_noise;
  
  
}

void KalmanFilter::KalmanFilterInit(int* accel_angles, float* gyro_rate){
	adxl345_angles_KF = accel_angles;
	itg3200_gyroRate_KF = gyro_rate;
}

void KalmanFilter::KalmanFilterUpdateAngles(int* KFAngles){
    matrixMath.MatrixCopy((float*)YZ_x_k,2,1,(float*)YZ_x_k_1);

    matrixMath.MatrixCopy((float*)XZ_x_k,2,1,(float*)XZ_x_k_1);

    YZ_x_k_1[1] = itg3200_gyroRate_KF[0];

    XZ_x_k_1[1] = itg3200_gyroRate_KF[1];    

    matrixMath.MatrixMult((float*)A,(float*)YZ_x_k_1,2,2,1,(float*)YZ_x_k);  

    matrixMath.MatrixMult((float*)A,(float*)XZ_x_k_1,2,2,1,(float*)XZ_x_k);  

    YZ_Inn = adxl345_angles_KF[0]-YZ_x_k[0];                                       

    XZ_Inn = adxl345_angles_KF[1]-XZ_x_k[0];  

    matrixMath.MatrixMult((float*)C,(float*)YZ_P,1,2,2,(float*)YZ_s_temp1);
    matrixMath.MatrixMult((float*)YZ_s_temp1,(float*)C,1,2,1,(float*)YZ_s_temp2);
    YZ_s = YZ_s_temp2[0] + Sz;

    matrixMath.MatrixMult((float*)C,(float*)XZ_P,1,2,2,(float*)XZ_s_temp1);
    matrixMath.MatrixMult((float*)XZ_s_temp1,(float*)C,1,2,1,(float*)XZ_s_temp2);
    XZ_s = XZ_s_temp2[0] + Sz;

    matrixMath.MatrixMult((float*)A,(float*)YZ_P,2,2,2,(float*)YZ_K_temp1);
    matrixMath.MatrixMult((float*)YZ_K_temp1,(float*)C, 2,2,1,(float*)YZ_K_temp2);
    YZ_K[0] = YZ_K_temp2[0]/YZ_s;
    YZ_K[1] = YZ_K_temp2[1]/YZ_s;
    YZ_K_Inn[0] = YZ_K[0]*YZ_Inn;
    YZ_K_Inn[1] = YZ_K[1]*YZ_Inn;
    
    matrixMath.MatrixMult((float*)A,(float*)XZ_P,2,2,2,(float*)XZ_K_temp1);
    matrixMath.MatrixMult((float*)XZ_K_temp1,(float*)C, 2,2,1,(float*)XZ_K_temp2);
    XZ_K[0] = XZ_K_temp2[0]/XZ_s;
    XZ_K[1] = XZ_K_temp2[1]/XZ_s;
    XZ_K_Inn[0] = XZ_K[0]*XZ_Inn;
    XZ_K_Inn[1] = XZ_K[1]*XZ_Inn;    
    
    matrixMath.MatrixAdd((float*)YZ_x_k,(float*)YZ_K_Inn, 2,1,(float*)YZ_x_k);
    YZ_plane_angle = (int)YZ_x_k[0];
    
    matrixMath.MatrixAdd((float*)XZ_x_k,(float*)XZ_K_Inn, 2,1,(float*)XZ_x_k);
    XZ_plane_angle = (int)XZ_x_k[0];

    matrixMath.MatrixMult((float*)YZ_K,(float*)C,2,1,2,(float*)YZ_K_C);
    matrixMath.MatrixMult((float*)YZ_K_C,(float*)YZ_P,2,2,2,(float*)YZ_K_C_P);
    matrixMath.MatrixMult((float*)YZ_K_C_P,(float*)At,2,2,2,(float*)YZ_K_C_P_At);
    matrixMath.MatrixMult((float*)A,(float*)YZ_P,2,2,2,(float*)YZ_A_P);
    matrixMath.MatrixMult((float*)YZ_A_P,(float*)At,2,2,2,(float*)YZ_A_P_At);
    matrixMath.MatrixSubtract((float*)YZ_A_P_At,(float*)YZ_K_C_P_At,2,2,(float*)YZ_A_P_At_K_C_P_At);
    matrixMath.MatrixAdd((float*)YZ_A_P_At_K_C_P_At,(float*)Sw,2,2,(float*)YZ_P);
    
    matrixMath.MatrixMult((float*)XZ_K,(float*)C,2,1,2,(float*)XZ_K_C);
    matrixMath.MatrixMult((float*)XZ_K_C,(float*)XZ_P,2,2,2,(float*)XZ_K_C_P);
    matrixMath.MatrixMult((float*)XZ_K_C_P,(float*)At,2,2,2,(float*)XZ_K_C_P_At);
    matrixMath.MatrixMult((float*)A,(float*)YZ_P,2,2,2,(float*)XZ_A_P);
    matrixMath.MatrixMult((float*)XZ_A_P,(float*)At,2,2,2,(float*)XZ_A_P_At);
    matrixMath.MatrixSubtract((float*)XZ_A_P_At,(float*)XZ_K_C_P_At,2,2,(float*)XZ_A_P_At_K_C_P_At);
    matrixMath.MatrixAdd((float*)XZ_A_P_At_K_C_P_At,(float*)Sw,2,2,(float*)XZ_P);
    
    KFAngles[0] = YZ_plane_angle;
  
    KFAngles[1] = XZ_plane_angle;
}


