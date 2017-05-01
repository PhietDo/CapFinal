/*
 * Author: Phiet Do
 * Updated Date: 4/12/2017
 * Purpose: This code is used for the Slave Arduino device to calculate the strain using the HX711 module.
 * The code will obtain strain values and send packets/buffers using I2C protocol to the master Arduino device. 
 * (Not used in this code) The MPU6050 is another slave device that will transfer acceleration values using I2C protocol as well.
 * The HX711 is the ADC and amplifier used to measure the resistive strain gauge.
 * (Not used in this code) The MPU6050 is the other sensor needed to obtain the acceleration which is further used to calculate G-Force.
 * 
 * All HX711 library credit goes to bogde
 * Library can be found at https://github.com/bogde/HX711
 * 
 * All MPU6050 library credit goes to Jeff Rowberg <jeff@rowberg.net>
 * Library can be found at https://github.com/jrowberg/i2cdevlib
 *  
 * Libraries needed in this design:
 * Wire and I2Cdev: Needed to perfrom I2C communication from Slave Arduino and MPU6050 to Master Arduino
 * HX711: needed to interface with the HX711 values. Does the conversion from bits to grams but can easily obtain readings from 0 to 2^24 bits.
*/

#include <Wire.h>
#include "HX711.h"
#include <string.h>

char strain[7]; //empty array that will hold values to send to master Arduino
char strainTwo[7]; //empty array that will hold values to send to master Arduino
char strainThree[7]; //empty array that will hold values to send to master Arduino
char strainFour[7]; //empty array that will hold values to send to master Arduino
char strainFive[7]; //empty array that will hold values to send to master Arduino
char strainSix[7]; //empty array that will hold values to send to master Arduino
bool switchCase = false;
char fullStrain[24];
char fullStrainTwo[24];

HX711 scale; // create an HX711 object
float fvalue; // used to obtain the one strain/gram reading
float value; // used to obtain the average strain/gram reading


HX711 scaleTwo; // create an HX711 object
float fvalueTwo; // used to obtain the one strain/gram reading
float valueTwo; // used to obtain the average strain/gram reading

HX711 scaleThree;
float fvalueThree; // used to obtain the one strain/gram reading
float valueThree; // used to obtain the average strain/gram reading

HX711 scaleFour;
float fvalueFour; // used to obtain the one strain/gram reading
float valueFour; // used to obtain the average strain/gram reading

HX711 scaleFive;
float fvalueFive; // used to obtain the one strain/gram reading
float valueFive; // used to obtain the average strain/gram reading

HX711 scaleSix;
float fvalueSix; // used to obtain the one strain/gram reading
float valueSix; // used to obtain the average strain/gram reading

void setup() {
  Wire.begin(8);                // Slave id #8
  Wire.onRequest(requestEvent); // Function to run when asking for data
  Serial.begin(115200);  // Initialize serial monitor

  Serial.println("Initializing HX711...");
  // parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT  - pin #A1
  // HX711.PD_SCK - pin #A0
  scale.begin(A1, A0);
  scale.set_scale(2500.f);     // this value is obtained by calibrating the scale with known weights;
  scale.tare();                // reset the scale to 0
  Serial.print("1.");
  
  // HX711.DOUT  - pin #A3
  // HX711.PD_SCK - pin #A2
  scaleTwo.begin(A3, A2);
  scaleTwo.set_scale(2500.f);     // this value is obtained by calibrating the scale with known weights;
  scaleTwo.tare();                // reset the scale to 0
  Serial.print("2.");
  
  scaleThree.begin(A5, A4);
  scaleThree.set_scale(2500.f);     // this value is obtained by calibrating the scale with known weights;
  scaleThree.tare();                // reset the scale to 0
  Serial.print("3.");
  
  scaleFour.begin(A7, A6);
  scaleFour.set_scale(2500.f);     // this value is obtained by calibrating the scale with known weights;
  scaleFour.tare();                // reset the scale to 0
  Serial.print("4.");
  
  scaleFive.begin(A9, A8);
  scaleFive.set_scale(2500.f);     // this value is obtained by calibrating the scale with known weights;
  scaleFive.tare();                // reset the scale to 0
  Serial.print("5.");
  
  scaleSix.begin(A11, A10);
  scaleSix.set_scale(2500.f);     // this value is obtained by calibrating the scale with known weights;
  scaleSix.tare();                // reset the scale to 0
  Serial.println("6.");

}

void loop() {
  //fvalue = scale.get_units(),1; // obtain a strain/gram reading
  value = scale.get_units(),1; // obtain an average strain/gram reading based on 10 readings - change 1 to a bigger value for an average
  dtostrf(value, 7, 2, strain); //converts the float or integer to a string. (floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array);
  /*
  Serial.print("Bending reading One:\t");
  Serial.print(fvalue);
  Serial.print("\t| average:\t");
  Serial.println(value);
  */
  
  //fvalueTwo = scaleTwo.get_units(),1; // obtain a strain/gram reading
  valueTwo = scaleTwo.get_units(),1; // obtain an average strain/gram reading based on 10 readings - change 1 to a bigger value for an average
  dtostrf(valueTwo, 7, 2, strainTwo); //converts the float or integer to a string. (floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array);
  /*
  Serial.print("Axial reading Two:\t");
  Serial.print(fvalueTwo);
  Serial.print("\t| average:\t");
  Serial.println(valueTwo);
  */
  
  //fvalueThree = scaleThree.get_units(),1; // obtain a strain/gram reading
  valueThree = scaleThree.get_units(),1; // obtain an average strain/gram reading based on 10 readings - change 1 to a bigger value for an average
  dtostrf(valueThree, 7, 2, strainThree); //converts the float or integer to a string. (floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array);
  /*
  Serial.print("Axial reading Three:\t");
  Serial.print(fvalueThree);
  Serial.print("\t| average:\t");
  Serial.println(valueThree);
  */
  
  //fvalueFour = scale.get_units(),1; // obtain a strain/gram reading
  valueFour = scaleFour.get_units(),1; // obtain an average strain/gram reading based on 10 readings - change 1 to a bigger value for an average
  dtostrf(valueFour, 7, 2, strainFour); //converts the float or integer to a string. (floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array);
  /*
  Serial.print("Axial reading Four:\t");
  Serial.print(fvalueFour);
  Serial.print("\t| average:\t");
  Serial.println(valueFour);
  */
  
  //fvalueFive = scaleFive.get_units(),1; // obtain a strain/gram reading
  valueFive = scaleFive.get_units(),1; // obtain an average strain/gram reading based on 10 readings - change 1 to a bigger value for an average
  dtostrf(valueFive, 7, 2, strainFive); //converts the float or integer to a string. (floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array);
  /*
  Serial.print("Axial reading Five:\t");
  Serial.print(fvalueFive);
  Serial.print("\t| average:\t");
  Serial.println(valueFive);
  */

  //fvalueSix = scaleSix.get_units(),1; // obtain a strain/gram reading
  valueSix = scaleSix.get_units(),1; // obtain an average strain/gram reading based on 10 readings - change 1 to a bigger value for an average
  dtostrf(valueSix, 7, 2, strainSix); //converts the float or integer to a string. (floatVar, minStringWidthIncDecimalPoint, numVarsAfterDecimal, empty array);
  /*
  Serial.print("Axial reading Six:\t");
  Serial.print(fvalueSix);
  Serial.print("\t| average:\t");
  Serial.println(valueSix);
  */

  /*
   * A series of while loops to place the strain values into one array to transfer over.
   * The comma acts as a delimiter to distinguish different strain values.
   * Also note that there are two full strain arrays because we want to send a collection
   * of 48 bytes but I2C can only send 32 bytes at a time. Therefore we send information
   * in increments of 24.
   */
  int i = 0;
  int j = 0;
  while(strain[i] != '\0'){
    fullStrain[i] = strain[i];
    i=i+1;
  }
  fullStrain[i] = ',';
  i=i+ 1;
  while (strainTwo[j] != '\0'){
    fullStrain[i] = strainTwo[j];
    i=i+1;
    j=j+1;  
  }
  fullStrain[i] = ',';
  i=i+ 1;
  j = 0;
  while (strainThree[j] != '\0'){
    fullStrain[i] = strainThree[j];
    i=i+1;
    j=j+1;  
  }
  fullStrain[i] = '\0';
  
  i=0;
  j = 0;
  while (strainFour[j] != '\0'){
    fullStrainTwo[i] = strainFour[j];
    i=i+1;
    j=j+1;  
  }
  fullStrainTwo[i] = ',';
  i=i+ 1;
  j = 0;
  while (strainFive[j] != '\0'){
    fullStrainTwo[i] = strainFive[j];
    i=i+1;
    j=j+1;  
  }
  fullStrainTwo[i] = ',';
  i=i+ 1;
  j = 0;
  while (strainSix[j] != '\0'){
    fullStrainTwo[i] = strainSix[j];
    i=i+1;
    j=j+1;  
  }

  fullStrainTwo[i] = '\0';

  //Prints values for display **UNCOMMENT IF YOU WOULD LIKE TO SEE THE VALUES
  /*
  Serial.println("****");
  Serial.println(fullStrain);
  Serial.println(fullStrainTwo);
  */
  // Resets all arrays back to 0 for reinitialzation of values
  memset(strain, 0, sizeof strain);
  memset(strainTwo, 0, sizeof strainTwo);
  memset(strainThree, 0, sizeof strainThree);
  memset(strainFour, 0, sizeof strainFour);
  memset(strainFive, 0, sizeof strainFive);
  memset(strainSix, 0, sizeof strainSix);
  //memset(strain, 0, sizeof fullStrain);
  //memset(strain, 0, sizeof fullStrainTwo);
  //memset(fullStrain, 0, sizeof fullStrain);
  delay(1000);
  
  //*********************************************************

}

// function: to send data using I2C protocol
// The two full strain arrays are sent in an alternate wave where one array is sent at a time.
void requestEvent() {
  if(!switchCase){
    Wire.write(fullStrain);
    switchCase = !switchCase;
  }
  else{
    Wire.write(fullStrainTwo);
    switchCase = !switchCase;
  }
  
  
}

