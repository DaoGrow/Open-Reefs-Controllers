/*
 * Last Updated - April 4, 2012
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
 
void setup() {
  Wire.begin();
}

/* Note: that the '39' is the address of the I2C breakout board.
 * I bridged the jumpers on this board to derive this configuration to be
 * addressable by '39' - Your configuration may be different! Either see 
 * the documentation for the breakout board or ask for help at:
 *             http://www.openreefs.com/forum 
 */
void set(byte b0,byte b1) {
  Wire.beginTransmission(39);
  Wire.write(b0);
  Wire.write(b1);
  Wire.endTransmission();
 
}

void loop(){
  /* 
   * This code just verifies that your all of your relays 
   * are working properly by rotating through each of them. 
   *
   * The channels are addressable in 7 bit binary.
   * To turn channel 1 on, you send a 1, to turn one and two on send 3
   * To turn just channel 2 on, send a 2. This code turns on all channels
   * together one by one and then at once turns the all off together. 
   *
   * One other thing to note is that each channel will require 73mA to switch.
   * It will also require 25mA to keep it closed.
   *  
   * This means if you were to switch ALL 7 channels at one time, it would draw
   * .511 amps through your lines and the power supply of the Arduino. Don't do
   * that regularly. Instead switch them on one at a time.
   * 
   * Also, do note that this will draw .175 amps to keep all 7 relays closed 
   * constantly. This means if you have your deviced power by a transformer you
   * will want to have a regulator on the Arduino that can handle the current, 
   * or more precisely, the watt dissipation that is converted to heat that 
   * occurs when you clamp the voltage down to 5v from whatever sized 
   * transformer you are using to power the Arduino.
   *
   */
  
  //All channels off
  set(0,0);
  delay(1000);
 
  //Channel 7 on first
  set(64,0);
  delay(200);

  //Channel 7 and 6 on together
  set(96,0);
  delay(200);
  
  //Channel 7, 6 and 5 on together
  set(112,0);
  delay(200);
 
  //Channel 7, 6, 5 and 4 on together
  set(120,0);
  delay(200);

  //Channel 7, 6, 5, 4 and 3 on together 
  set(124,0);
  delay(200);
 
  //Channel 7, 6, 5, 4, 3 and 2 on together
  set(126,0);
  delay(200);
  
  //All channels on together
  set(127,0);
  delay(1000);
}