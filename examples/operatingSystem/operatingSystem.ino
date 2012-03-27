/*
 * Last Updated - March 27, 2012
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

#define NQUEUE 8
#include <stdint.h>
 
// This class handles the actual events we are going to perform.
// In the example we have a NTP request and a Clock time.
class Event
{
public:
  uint16_t timeout;
  virtual void act()=0;
};
 
// This class actually keeps track of all of the Events in a structured order
// processing them as quickly as possible.
// This is the key to the entire design, create lots of small very fast events
// and keep them rolling through the queue.
class Queue
{
public:
  Event * data[NQUEUE];
  int qstart,qend,qlength;
 
  Queue()
  {
    qstart=0;
    qend=0;
    qlength=0;
  }
 
  void add(Event *e)
  {
    if (qlength < NQUEUE) {
      data[qend]=e;
      if (++qend == NQUEUE) qend=0;
      ++qlength;
    }
  }
 
  bool stale()
  {
    return (qlength > 0) && ((int16_t)(data[qstart]->timeout-millis()) <= 0);
  }
 
  Event *get()
  {
    Event *ans=0;
    if (qlength > 0) {
      ans=data[qstart];
      if (++qstart == NQUEUE) qstart=0;      
      --qlength;
    }
    return ans;
  }
};
 
Queue queue;
 
class Idle : public Event
{
public:
  void act() {  };
};
 
Idle idle;
 
//Example of Thread
class NTPRequest : public Event
{
  void act() {};
};
 
NTPRequest ntp_request;
 
//Example of Thread
class Clock : public Event
{
public:
  int h,m,s;
  Clock()
  {
    h=m=s=0;
  }
 
  void act()
  {
    if (++s == 60) {
      s=0;
      if (++m == 60) {
        m=0;
        ++h;
      }
    }
    timeout=millis()+1000;
    queue.add(this);
 
    if (s == 0 && m == 0) {
      ntp_request.timeout=millis()+0;
      queue.add(&ntp_request);
    }
  }
};
 
 
Clock clock;
 
void setup()
{
  clock.timeout=millis()+1000;
  queue.add(&clock);
}
 
void loop()
{
  if (queue.stale()) {
    queue.get()->act();
  } else idle.act();
}
