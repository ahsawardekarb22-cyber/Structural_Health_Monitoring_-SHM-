#include "Wire.h"
 #include <ESP8266WiFi.h> // This library is retained for compatibility;
#include <WiFiClientSecure.h>

#define ON_Board_LED 2  // Defining the On Board LED, used for indicators when connecting to Wi-Fi

const char* ssid = "Abhi";  // Your Wi-Fi name (SSID)
const char* password = "ABHISHEK";  // Your Wi-Fi password

//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------

WiFiClientSecure client;  // Create a WiFiClientSecure object

String GAS_ID = "AKfycbx-WAggkdjQVPzHV-ZRubUDYGjpUFTlbX9ykZjenPkxpo3X8F_9U5pR1wLBcHVj56XlGQ";  // Spreadsheet script ID

// Wi-Fi functionality is not used in this version
 // MPU6050 I2C address
 const uint8_t MPU6050SlaveAddress = 0x68;
 // Select GPIO pins for I2C communication (for ESP8266, use GPIO numbers)
 const uint8_t scl = 12; // GPIO12 (D6)
 const uint8_t sda = 13; // GPIO13 (D7)
 // Sensitivity scale factors based on the datasheet
 const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;
 // MPU6050 register addresses
 const uint8_t MPU6050_REGISTER_SMPLRT_DIV = 0x19;
 const uint8_t MPU6050_REGISTER_PWR_MGMT_1 = 0x6B;
 const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H = 0x3B;
 int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;
 unsigned long previousMillis = 0;
 const long interval = 100; // Data update interval in milliseconds

 void setup() {
 Serial.begin(115200);
 Wire.begin(sda, scl); // Initialize I2C with the correct GPIO pins
 MPU6050_Init();
 // Display initial message
 Serial.println("MPU6050 data visualization on the Serial Plotter:");
 WiFi.begin(ssid, password);  // Connect to Wi-Fi
  Serial.println("");
  
  pinMode(ON_Board_LED, OUTPUT);  // On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH);  // Turn off LED initially
  
  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the Wi-Fi router
    digitalWrite(ON_Board_LED, LOW);
    delay(550);
    digitalWrite(ON_Board_LED, HIGH);
    delay(550);
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH);  // Turn off the On Board LED when connected to Wi-Fi
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  client.setInsecure();  // Allow insecure connections (no SSL certificate verification)
 }
 void loop() {
  double ax, ay, az, T, gx, gy, gz;
 // Read sensor values
 Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
// Convert raw values to human-readable units
ax = (double)AccelX / AccelScaleFactor;
 ay = (double)AccelY / AccelScaleFactor;
 az = (double)AccelZ / AccelScaleFactor;
 T = (double)Temperature / 340.00 + 36.53; // Temperature formula
 gx = (double)GyroX / GyroScaleFactor;
 gy = (double)GyroY / GyroScaleFactor;
 gz = (double)GyroZ / GyroScaleFactor;

 unsigned long currentMillis = millis();
 // Only update data every interval milliseconds
 if (currentMillis- previousMillis >= interval) {
 previousMillis = currentMillis; // Save the last time data was
 
 // Fetch and display MPU6050 data
 fetchAndDisplayData();

 // Send the data to Google Sheets
 sendData(ax, ay, az, gx, gy, gz);

  delay(2000);  // Delay before taking the next reading
 }

 }
 void fetchAndDisplayData() {
 double Ax, Ay, Az, T, Gx, Gy, Gz;
 // Read sensor values
 Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
// Convert raw values to human-readable units
 Ax = (double)AccelX / AccelScaleFactor;
 Ay = (double)AccelY / AccelScaleFactor;
 Az = (double)AccelZ / AccelScaleFactor;
 T = (double)Temperature / 340.00 + 36.53; // Temperature formula
 Gx = (double)GyroX / GyroScaleFactor;
 Gy = (double)GyroY / GyroScaleFactor;
 Gz = (double)GyroZ / GyroScaleFactor;
 // Serial Plotter output
 Serial.print(Ax); Serial.print(" ");
 Serial.print(Ay); Serial.print(" ");
 Serial.print(Az); Serial.print(" ");
 Serial.print(T); Serial.print(" ");
 Serial.print(Gx); Serial.print(" ");
 Serial.print(Gy); Serial.print(" ");
 Serial.println(Gz);
 }
 // Function to write to MPU6050 register
 void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data) {
 Wire.beginTransmission(deviceAddress);
 Wire.write(regAddress);
 Wire.write(data);
 Wire.endTransmission();
 }
 // Function to read all 14 bytes from the sensor
 void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress) {
 Wire.beginTransmission(deviceAddress);
 Wire.write(regAddress);
 Wire.endTransmission();
 Wire.requestFrom(deviceAddress, (uint8_t)14);
 AccelX = (((int16_t)Wire.read() << 8) | Wire.read());
 AccelY = (((int16_t)Wire.read() << 8) | Wire.read());
 AccelZ = (((int16_t)Wire.read() << 8) | Wire.read());
 Temperature = (((int16_t)Wire.read() << 8) | Wire.read());
 GyroX = (((int16_t)Wire.read() << 8) | Wire.read());
 GyroY = (((int16_t)Wire.read() << 8) | Wire.read());
GyroZ = (((int16_t)Wire.read() << 8) | Wire.read());
 }
 // Function to initialize the MPU6050
 void MPU6050_Init() {
 delay(150); // Delay for stabilization
 I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
 I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
 }

 // Subroutine for sending data to Google Sheets
void sendData(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz) {
  Serial.println("==========");
  Serial.print("Connecting to ");
  Serial.println(host);
  
  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }
  
  //----------------------------------------Get Current Date and Time

  
  //----------------------------------------Processing data and sending data
  String ax_str = String(ax);
  String ay_str = String(ay);
  String az_str = String(az);
  String gx_str = String(gx);
  String gy_str = String(gy);
  String gz_str = String(gz);
  String temperature = String(100);

  
  String url = "/macros/s/" + GAS_ID + "/exec?ax=" + ax_str + "&ay=" + ay_str + "&az=" + az_str + "&temperature=" + temperature +
               "&gx=" + gx_str + "&gy=" + gy_str + "&gz=" + gz_str + "&location=" + "Mumbai";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
  
  Serial.println("Request sent");
  
  //----------------------------------------Check if the data was sent successfully
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }
  
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("Data successfully sent to Google Sheets!");
  } else {
    Serial.println("Failed to send data to Google Sheets");
  }
  
  Serial.print("Reply: ");
  Serial.println(line);
  Serial.println("Closing connection");
  Serial.println("==========");
  Serial.println();
}