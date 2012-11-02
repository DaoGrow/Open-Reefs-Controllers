/*
 * Last Updated - November 2, 2012
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

// There are three parts to this code, the header, the cpp and the sketch. 
// You really should break them up instead of keeping them in one file like
// it is shown here. The only reason I kept it in one file is to deomonstrate all
// of the pieces in one place.

//--- This should go into a header file really...
#include <stdint.h>
#include <Arduino.h>

class Event
{
public:
  uint16_t timeout;
  virtual void act()=0;
};

class Queue
{
public:
  static const int NQUEUE = 16;
  Event * data[NQUEUE];
  int qstart,qend,qlength;
  Queue();
  void add(Event *e); 
  bool stale();
  Event *get();
};
 
extern Queue mainQueue;

///--- This should go into a cpp file linked to the above header

Queue::Queue()
{
  qstart=0;
  qend=0;
  qlength=0;
}

void Queue::add(Event *e)
{
  if (qlength < NQUEUE) {
    int at=qend, before = (before > 0) ? at - 1 : NQUEUE - 1;
    data[qend]=e;
    if (++qend == NQUEUE) qend=0;
    ++qlength;
    while (at != qstart && (int16_t)(data[before]->timeout-(e->timeout)) > 0) {
      data[at]=data[before];
      data[before]=e;
      at = before;
      before = (before > 0) ? before - 1 : NQUEUE - 1;
    }
  }
}

bool Queue::stale()
{
  return (qlength > 0) && ((int16_t)(data[qstart]->timeout-millis()) <= 0);
}

Event *Queue::get()
{
  Event *ans=0;
  if (qlength > 0) {
    ans=data[qstart];
    if (++qstart == NQUEUE) qstart=0;      
    --qlength;
  }
  return ans;
}
 
Queue mainQueue;

//--- You should include your header file here and only have this in the sketch. 

void setup(){
  //Add the kickoff event
  //mainQueue.add(&someEvent); 
}

void loop() {
  while (mainQueue.stale()) {
    Event *e = mainQueue.get();
    e->act();
  }
  // This is the event that will run when the system is idle.
  //idleEvent();
}
