#define ADXL345_SMOOTHING_SAMPLES 1000

void setup() {
  Serial.begin(115200);
  Wire.begin();                                           // join i2c bus (address optional for master)

// ACCELEROMETER SETUP
  i2c_write(ADXL345_ADDRESS, ADXL345_POWER_CTL, 0x08);    // ????????
  i2c_write(ADXL345_ADDRESS, ADXL345_DATA_FORMAT, 0x00);  // ????????

// GYROSCOPE SETUP
  i2c_write(ITG3200_ADDRESS, ITG3200_SMPLRT_DIV, 0x00);  // ?????????????
  i2c_write(ITG3200_ADDRESS, ITG3200_DLPF, 0x1E);        // ????????????????
  i2c_write(ITG3200_ADDRESS, ITG3200_PWR_MGM, 0x01);     // ????????????
  
  adxl345_calibration(adxl345_accelerometer_calibration, ADXL345_SMOOTHING_SAMPLES);
  
  
}
