/*
   Author: Phiet Do
   Date: 4/12/2017
   Purpose: This code is used for the Master Arduino device to obtain strain and acceleration values.
   The code will obtain strain values by packets/buffers using I2C protocol from the slave Arduino.
   The MPU6050 is another slave device that will transfer acceleration values using I2C protocol as well.
   The HX711 is the ADC and amplifier used to measure the resistive strain gauge.
   The MPU6050 is the other sensor needed to obtain the acceleration which is further used to calculate G-Force.

   All HX711 library credit goes to bogde
   Library can be found at https://github.com/bogde/HX711

    All MPU6050 library credit goes to Jeff Rowberg <jeff@rowberg.net>
    Library can be found at https://github.com/jrowberg/i2cdevlib

    Libraries needed in this design:
    Wire and I2Cdev: Needed to perfrom I2C communication from Slave Arduino and MPU6050 to Master Arduino
    MPU6050: Needed to interface and obtain acceleration and other values from the MPU6050.
*/

#include <SPI.h>
#include <SD.h>
const int chipSelect = 4;

#include <Wire.h>
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro; // Create object for MPU6050
//MPU6050 accelgyro(0x69); // <-- use for AD0 high

int16_t ax, ay, az; // create variables that will hold the 3-axis acceleration values
float axg, ayg, azg; // variables that will hold the g-force of each acceleration value

#define OUTPUT_READABLE_ACCELGYRO // define MPU6050's acceleration functions used later
String totalData;
String delim = ",";
char datainfo[24] = {}; //empty array that will hold the values coming from the slave Arduino
char datainfoTwo[24] = {}; //empty array that will hold the values coming from the slave Arduino
bool switchCase = false;
String strain; // String that will hold the transferred strain from the slave
String strainTwo; // String that will hold the second transferred strain from the slave
String strainThree;
String strainFour;
String strainFive;
String strainSix;

float strainval; // Variable needed to change the string to a quantifiable measurement, float
float strainvalTwo; // Variable needed to change the string to a quantifiable measurement, float
float strainvalThree;
float strainvalFour;
float strainvalFive;
float strainvalSix;

// NOTE: y-axis is not needed for our design.

void setup() {
  // see if the card is present and can be initialized:

  Wire.begin();        // initialize the i2c bus
  Serial.begin(115200);  // start serial for output
  //Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
 // Serial.println("card initialized.");
  // initialize the MPU6050 device
  //Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  //Serial.println("Testing device connections...");
  //Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  
  delay(1000);
  int i=0;
  Wire.requestFrom(8, 23);    // request 7 bytes from slave arduino 8. The 7 bytes is the greatest size of the strain value.
  // While loop to go through each byte that has been sent through the I2C protocol and obtain the data to put in a char array.
  while (Wire.available()) {
    if(!switchCase){
      datainfo[i] = Wire.read(); // append every character or byte transferred into an empty char array
    }
    else{
      datainfoTwo[i] = Wire.read(); // append every character or byte transferred into an empty char array
    }
    
    //Serial.println(datainfo[i]);  
    i = i + 1;
  }
  
  // Similar to the above while loop but the transferred information is now being sent to String instead of a char array
  int j = 0;
  while (datainfo[j] != '\0') {
    //Serial.println(datainfo[j]);
    if (j < 7) {
      strain += datainfo[j];
    }
    else if (datainfo[j] == ',') { //function to skip the delimeter sent from slave device
      //j=j+1;
      // continue;
    }
    else if (j > 7 && j < 15) {
      strainTwo += datainfo[j];
    }
    else if (j > 15 && j < 24) {
      strainThree += datainfo[j];
    }

    j = j + 1;
  }
    //*****************************
  j = 0;
  while (datainfoTwo[j] != '\0') {
    if (j < 7) {
      strainFour += datainfoTwo[j];
    }
    else if (datainfo[j] == ',') { //function to skip the delimeter sent from slave device
      //j=j+1;
      // continue;
    }
    else if (j > 7 && j < 15) {
      strainFive += datainfoTwo[j];
    }
    else if (j > 15 && j < 24) {
      strainSix += datainfoTwo[j];
    }
    j = j + 1;
  } 

    
  //The if statement is needed because on occasion, no values are sent through I2C and we do not want to waste our time printing nothing
  
  if (strainFour.length() != 0 && strainFive.length() != 0 && strainSix.length() != 0) {
    /*
    Serial.println("------------------------------------------------------");
    Serial.print("original value\t"); //only needed if you want to check if original info ha been sent.
    Serial.print(datainfo);   //shows the data in the char array
    Serial.println(datainfoTwo);
    //Serial.print("strain value\t"); //prints out the strain in string form; not really a difference from char array
    //Serial.println(strain);
    */
    strainval = strain.toFloat(); // Convert string to float; This is why we needed the strain variable to be a String.
    strainvalTwo = strainTwo.toFloat(); // Convert string to float; This is why we needed the strain variable to be a String.
    strainvalThree = strainThree.toFloat(); // Convert string to float; This is why we needed the strain variable to be a String.
    strainvalFour = strainFour.toFloat(); // Convert string to float; This is why we needed the strain variable to be a String.
    strainvalFive = strainFive.toFloat(); // Convert string to float; This is why we needed the strain variable to be a String.
    strainvalSix = strainSix.toFloat(); // Convert string to float; This is why we needed the strain variable to be a String.
    /*
      Serial.print("strain  value\t"); // prints out the float strain value
      Serial.println(strainval);
    */
    accelgyro.getAcceleration(&ax, &ay, &az); // prints out the raw acceleration values from the MPU6050
    //accelgyro.getRotation(&gx, &gy, &gz); // prints out the raw gyro values from the MPU6050; You must declare these variables above.

    // display tab-separated acceleration x/y/z values
    /*
      Serial.print("acceleration:\t");
      Serial.print(ax); Serial.print("\t");
      Serial.print(ay); Serial.print("\t");
      Serial.print(az); Serial.println("\t");
    */
    //16384 for 2gs, 8192 for 4gs, 4096 for 8gs, 2048 for 16gs
    //Typecast each integer to floats and calculate the G-Force of each axis
    axg = (float)ax / 8192;
    //ayg = (float)ay / 8192;
    azg = (float)az / 8192;
    /*
      Serial.print("X,Y,Z G-Force: ");
      Serial.print(axg); Serial.print("\t");
      Serial.print(ayg); Serial.print("\t");

      Serial.println(azg); */
    //totalData = strainval + delim + strainvalTwo + delim + strainvalThree + delim + strainvalFour + delim + strainvalFive + delim + strainvalSix + delim + axg + delim + azg;
    totalData = strainval + delim + strainvalTwo + delim + strainvalThree + delim  + strainvalFour + delim + strainvalFive + delim + strainvalSix + delim + axg + delim + azg;
    Serial.println(totalData);
  }
  //After the values have been displayed, processed, or saved, then clear the values to reset for the next set.
  strain = "";
  strainTwo = "";
  strainThree = "";
  strainFour = "";
  strainFive = "";
  strainSix = "";
  //clear the array values for the next round of data.
  if(switchCase){
    memset(datainfo, 0, sizeof datainfo);
    memset(datainfoTwo, 0, sizeof datainfoTwo);  
  }
  //inverse the boolean to read data and store it in an alternate array.
  switchCase = !switchCase;
  

  //delay(100); // wait for a little to process another set of values.

  //start the datalogging and write it to a readable excel file
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(totalData);
    dataFile.close(); 
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.csv");
  }
}
