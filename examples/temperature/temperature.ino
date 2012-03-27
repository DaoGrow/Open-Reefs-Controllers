/*
 * Last Updated - March 22, 2012
 * Open Ocean Reef Controller by Brandon Bearden 
 *
 * This work is licensed under the Creative Commons 
 * Attribution-NonCommercial-ShareAlike 3.0 Unported License. 
 * To view a copy of this license, visit 
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 * or send a letter to Creative Commons, 444 Castro Street, 
 * Suite 900, Mountain View, California, 94041, USA.
 * 
 * IN NO EVENT SHALL THE LICENSE HOLDER BE LIABLE TO ANY 
 * PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, 
 * OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OF THIS 
 * SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE COPYRIGHT 
 * HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE LICENSE HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. 
 * THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, 
 * AND THE LICENCE HOLDER HAS NO OBLIGATION TO PROVIDE 
 * MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, 
 * OR MODIFICATIONS.
 *
 */
 
#include <Wire.h>
#include <LibTemperature.h>

// Initialize Temperature Sensor Library
LibTemperature temp = LibTemperature(0);

void setup() {
  // Begin Serial Communication
  Serial.begin(9600);
}

float getTemp(int sensor, char conversion){
  double tempS = 0;
  // Choose the sensor to poll
  switch(sensor) {
    case 2: sensor = 0b0011100;
    break;
    case 3: sensor = 0b0011110;
    break;
    default: sensor = 0x2A;
  }
  // Switch to celsius or fahrenheit
  if(conversion=='c')
    tempS = temp.GetTemperature(sensor);
  else
    tempS = (temp.GetTemperature(sensor)* 9 / 5) + 32;;
  
  // Get and output the temperature
  double integral;
  double fractional;
  // This line breaks the temperature up into parts so 
  // that it can be displayed properly.
  // The 100 gets 2 decimal places.
  fractional = 100*(modf(tempS, &integral));
  Serial.print(int(integral));
  Serial.print(".");
  Serial.print(int(fractional));
  
  return tempS;
}

void sensorAverage(float avg){
  double integral;
  double fractional;
  // This line breaks the temperature up into parts so 
  // that it can be displayed properly.
  // The 1000 gets 3 decimal places.
  fractional = 1000*(modf(avg, &integral));
  Serial.print(int(integral));
  Serial.print(".");
  Serial.print(int(fractional));
}

void temperature() {
  //Display temperatures for sensors 1-3
  int sensorCount = 0;
  float avg = 0;
  double temp;
  //sensor 1
  Wire.requestFrom(0x2A, 1);
  if(Wire.available()) {
    Serial.print("Sensor 1: ");
    temp = getTemp(1,'f');
    Serial.println("");
    avg += temp;
    sensorCount++;
  }
  else {
    Serial.print("Sensor1 Disconnected");
  }
  //sensor 2
  Wire.requestFrom(0b0011100, 1);
  if(Wire.available()) {
    Serial.print("Sensor 2: ");
    temp = getTemp(2,'f');
    Serial.println("");
    avg += temp;
    sensorCount++;
  }
  else {
    Serial.print("Sensor2 Disconnected");
  }
  
  //sensor 3
  Wire.requestFrom(0b0011110, 1);
  if(Wire.available()) {
    Serial.print("Sensor 3: ");
    temp = getTemp(3,'f');
    Serial.println("");
    avg += temp;
    sensorCount++;
  }
  else {
    Serial.print("Sensor3 Disconnected");
  }
  
  //average temp
  if(sensorCount != 0){
    avg /= sensorCount*1.0;
    Serial.print("Average Temp: ");
    sensorAverage(avg);
  }
  else{
    Serial.print("No Sensors Connected");
  }
}

void loop() {
  temperature();
  Serial.println("");
  Serial.println("");
  delay(3000);
}
