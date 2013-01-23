#include <Wire.h>

int HMC5883_ADDRESS = 0x1E;  // COMPASS I2C ADDRESS
byte HMC5883_MODE = 0x02;
byte HMC5883_DATA = 0x03;

int ADXL345_ADDRESS = 0x53;  // ACCELEROMETER I2C ADDRESS
byte ADXL345_DEV_ID = 0x00;
byte ADXL345_POWER_CTL = 0x2D;
byte ADXL345_DATA_FORMAT = 0x31;
byte ADXL345_DATA = 0x32;
byte ADXL345_BW_RATE = 0x2C;

int ITG3200_ADDRESS = 0x68;  // GYROSCOPE I2C ADDRESS
byte ITG3200_TEMP = 0x1B;
byte ITG3200_DATA = 0x1D;
byte ITG3200_SMPLRT_DIV = 0x15;
byte ITG3200_DLPF = 0x16;
byte ITG3200_PWR_MGM = 0x3E;

byte adxl345_measurement_data[6];



byte hmc5883_measurement_data[6];
int hmc5883_magnetometer_data[3];


byte itg3200_measurement_data[6];
byte itg3200_measurement_temp[2];






void adxl345_calibration(long * adxl345_accelerometer_calibration, int smoothing_samples) {
  adxl345_accelerometer_calibration[0] = 0;
  adxl345_accelerometer_calibration[1] = 0;
  adxl345_accelerometer_calibration[2] = 0;
  for (int j=0; j < smoothing_samples; j++) {
    read_adxl345_data(adxl345_accelerometer_data);
    for (int i=0; i < 3;i++) {
      adxl345_accelerometer_calibration[i] += adxl345_accelerometer_data[i];
    }
  }
    adxl345_accelerometer_calibration[0] /= smoothing_samples;
    adxl345_accelerometer_calibration[1] /= smoothing_samples;
    adxl345_accelerometer_calibration[2] /= smoothing_samples;  
}


void read_itg3200_data(int * itg3200_gyroscope_data) {
  i2c_read(ITG3200_ADDRESS, ITG3200_DATA, 6, itg3200_measurement_data);
  for (int i=0;i<3;i++) {
     itg3200_gyroscope_data[i] = ((int)itg3200_measurement_data[2*i] << 8) + (((int)itg3200_measurement_data[2*i + 1]));
  }
}


void read_itg3200_temp(unsigned int * itg3200_gyroscope_temp) {
  i2c_read(ITG3200_ADDRESS, ITG3200_TEMP, 2, itg3200_measurement_temp);
  itg3200_gyroscope_temp[0] = ((unsigned int)itg3200_measurement_temp[1])+((unsigned int)itg3200_measurement_temp[0]<<8);
}

void read_adxl345_data(int * adxl345_accelerometer_data) {
  i2c_read(ADXL345_ADDRESS, ADXL345_DATA, 6, adxl345_measurement_data);
  for (int i=0;i<3;i++) {
    adxl345_accelerometer_data[i] = ((int)adxl345_measurement_data[2*i]) + (((int)adxl345_measurement_data[2*i + 1])<<8);
  }
}

void print_serial_itg3200(int * itg3200_gyroscope_data, unsigned int * itg_gyroscope_temp, int last_printed) {
  Serial.print(itg3200_gyroscope_data[0], DEC);
  Serial.print(",");
  Serial.print(itg3200_gyroscope_data[1], DEC);
  Serial.print(",");
  Serial.print(itg3200_gyroscope_data[2], DEC);
  if (itg_gyroscope_temp != 0) {
    Serial.print(",");
    Serial.print(itg3200_gyroscope_temp[0], DEC);
  }
  if (last_printed) {
    Serial.println();
  }
  else {
    Serial.print(",");
  }
}

void print_serial_adxl345(int * adxl345_accelerometer_data, int last_printed) {
  Serial.print(adxl345_accelerometer_data[0], DEC);
  Serial.print(",");
  Serial.print(adxl345_accelerometer_data[1], DEC);
  Serial.print(",");
  Serial.print(adxl345_accelerometer_data[2], DEC);
  if (last_printed) {
    Serial.println();
  }
  else {
    Serial.print(",");
  }
}




/* 
  i2c_write(HMC5883_ADDRESS, HMC5883_MODE, 0x00);
  i2c_read(HMC5883_ADDRESS, HMC5883_DATA, 6, hmc5883_measurement_data);
  for (int i=0;i<3;++i) {
     hmc5883_magnetometer_data[i] = (int)hmc5883_measurement_data[2*i] + (((int)hmc5883_measurement_data[2*i + 1]) << 8);
   }
  Serial.print(hmc5883_magnetometer_data[0], DEC);
  Serial.print(",");
  Serial.print(hmc5883_magnetometer_data[1], DEC);
  Serial.print(",");
  Serial.print(hmc5883_magnetometer_data[2], DEC);
  //Serial.println();
  Serial.print(",");
  */
