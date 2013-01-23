#include <Wire.h>
byte dev_id[1];
int dev_id_int;


byte datax0, datax1, datay0, datay1, dataz0, dataz1;

int HMC5883_ADDRESS = 0x1E;
byte HMC5883_MODE = 0x02;
byte HMC5883_DATA = 0x03;

int ADXL345_ADDRESS = 0x53;
byte ADXL345_DEV_ID = 0x00;
byte ADXL345_POWER_CTL = 0x2D;
byte ADXL345_DATA_FORMAT = 0x31;
byte ADXL345_DATA = 0x32;
byte ADXL345_BW_RATE = 0x2C;

int ITG3200_ADDRESS = 0x68;
byte ITG3200_TEMP = 0x1B;
byte ITG3200_DATA = 0x1D;
byte ITG3200_SMPLRT_DIV = 0x15;
byte ITG3200_DLPF = 0x16;
byte ITG3200_PWR_MGM = 0x3E;

byte adxl345_measurement_data[6];
byte hmc5883_measurement_data[6];
byte itg3200_measurement_data[6];
byte itg3200_measurement_temp[2];
int hmc5883_magnetometer_data[3];
int adxl345_accelerometer_data[3];
int itg3200_gyroscope_data[3];
int itg3200_gyroscope_temp[1];
int itg3200_gyroscope_x_offset = 25;
int itg3200_gyroscope_y_offset = -35;
int itg3200_gyroscope_z_offset = 1;


byte TEST;

int number_of_bytes=0;

void i2c_write(int address, byte reg, byte data) {
  // Send output register address
  Wire.beginTransmission(address);
  Wire.write(reg);
  // Connect to device and send byte
  Wire.write(data); // low byte
  Wire.endTransmission();
}

void i2c_read(int address, byte reg, int count, byte* data) {
  int i = 0;
  char c;
 // Send input register address
 Wire.beginTransmission(address);
 Wire.write(reg);
 Wire.endTransmission();
 // Connect to device and request bytes
 Wire.beginTransmission(address);
 Wire.requestFrom(address,count);
 while(Wire.available()) {// slave may send less than requested
   c = Wire.read(); // receive a byte as character
   data[i] = c;
   i++;
 }
 Wire.endTransmission();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(); // join i2c bus (address optional for master)
  i2c_write(ADXL345_ADDRESS, ADXL345_POWER_CTL, 0x08);
  i2c_write(ADXL345_ADDRESS, ADXL345_DATA_FORMAT, 0x00);
  memset(adxl345_measurement_data, 0, 6);
  i2c_write(ITG3200_ADDRESS, ITG3200_SMPLRT_DIV, 0x00);  // Set sampling rate to 8000 Hz
  i2c_write(ITG3200_ADDRESS, ITG3200_DLPF, 0x1E);        // 42 Hz LPF, FS_SEL selected for proper operation
  i2c_write(ITG3200_ADDRESS, ITG3200_PWR_MGM, 0x01);
}

void loop() {

//  delay(200);
//  i2c_read(ADXL345_ADDRESS, ADXL345_DEV_ID, 1, dev_id);
//  dev_id_int = (int)dev_id[0];
//  Serial.println(dev_id_int, BIN);


  i2c_read(ADXL345_ADDRESS, ADXL345_DATA, 6, adxl345_measurement_data);
   for (int i=0;i<3;++i) {
     adxl345_accelerometer_data[i] = ((int)adxl345_measurement_data[2*i]) + (((int)adxl345_measurement_data[2*i + 1])<<8);
   }
  Serial.print(adxl345_accelerometer_data[0], DEC);
  Serial.print(",");
  Serial.print(adxl345_accelerometer_data[1], DEC);
  Serial.print(",");
  Serial.print(adxl345_accelerometer_data[2], DEC);
  //Serial.println();
  Serial.print(",");
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
  i2c_read(ITG3200_ADDRESS, ITG3200_DATA, 6, itg3200_measurement_data);
  for (int i=0;i<3;++i) {
     itg3200_gyroscope_data[i] = ((int)itg3200_measurement_data[2*i] << 8) + (((int)itg3200_measurement_data[2*i + 1]));
   }
   itg3200_gyroscope_data[0] += itg3200_gyroscope_x_offset;
   itg3200_gyroscope_data[1] += itg3200_gyroscope_y_offset;
   itg3200_gyroscope_data[2] += itg3200_gyroscope_z_offset;
   
  i2c_read(ITG3200_ADDRESS, ITG3200_TEMP, 2, itg3200_measurement_temp);
  itg3200_gyroscope_temp[0] = ((int)itg3200_measurement_temp[1])+((int)itg3200_measurement_temp[0]<<8);
  Serial.print(itg3200_gyroscope_temp[0], DEC);
  Serial.print(",");
  Serial.print(itg3200_gyroscope_data[0], DEC);
  Serial.print(",");
  Serial.print(itg3200_gyroscope_data[1], DEC);
  Serial.print(",");
  Serial.print(itg3200_gyroscope_data[2], DEC);
  //Serial.print(",");
  Serial.println();

  
}
