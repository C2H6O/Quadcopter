int adxl345_accelerometer_data[3];
int itg3200_gyroscope_data[3];
unsigned int itg3200_gyroscope_temp[1];
long adxl345_accelerometer_calibration[3];
int itg3200_gyroscope_calibration[3];


void loop() {
  
  read_adxl345_data(adxl345_accelerometer_data);
  read_itg3200_data(itg3200_gyroscope_data);
  read_itg3200_temp(itg3200_gyroscope_temp);

  Serial.print(adxl345_accelerometer_calibration[0]);
  Serial.print(",");
  Serial.print(adxl345_accelerometer_calibration[1]); 
  Serial.print(",");
  Serial.print(adxl345_accelerometer_calibration[2]);  
  Serial.print(",");
  print_serial_adxl345(adxl345_accelerometer_data, 0);
  print_serial_itg3200(itg3200_gyroscope_data, itg3200_gyroscope_temp, 1);

}
