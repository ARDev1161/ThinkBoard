#ifndef __TRACKPOINT_H__
#define __TRACKPOINT_H__

#include <Mouse.h>
#include "Arduino.h"
#include "keys.h"

#define TRACKPOINT_RESET 20 // reset trackpoint
#define TRACKPOINT_CLK 17 // yellow
#define TRACKPOINT_DATA 16     // orange
#define CLOCK_INT 4

#define LMB_BIT 19
#define WMB_BIT 13
#define RMB_BIT 9

class TrackPoint
{
  public:
  struct DataReport {
    uint8_t state;
    int8_t x;
    int8_t y;
  };

  void write(uint8_t data);
  uint8_t read(void);
  uint8_t readFromRamLocation(uint8_t);
  void writeToRamLocation(uint8_t, uint8_t);
  void setSensitivityFactor(uint8_t);
  uint8_t sensitivityFactor();
  void setRemoteMode();
  void reset();
  DataReport readData();
  
  TrackPoint(uint8_t, uint8_t, uint8_t, uint8_t);
  ~TrackPoint();
    
  void getDataBit(void);
  uint8_t reportAvailable(void);
  void setStreamMode(void);
  DataReport getStreamReport(void);
  
  void gohi(uint8_t pin);
  void golo(uint8_t pin);
  
  protected:
  
  private:
  TrackPoint( const TrackPoint &c );
  TrackPoint& operator=( const TrackPoint &c );
  
  //TrackPoint pins
  uint8_t _clkPin;
  uint8_t _dataPin;
  uint8_t _resetPin;
  
  //PS2 data
  DataReport data;
  
  //getDataBit() variables
  volatile uint8_t bitcount;
  volatile uint8_t n;
  volatile uint8_t val;
  volatile uint8_t incoming;
  volatile uint8_t counter;
  volatile uint8_t dataAvailable;
  uint8_t usingSeparateResetPin;

}; //TrackPoint


// default constructor
TrackPoint::TrackPoint(uint8_t clockPin, uint8_t dataPin, uint8_t resetPin, uint8_t streamMode) : _clkPin(clockPin), _dataPin(dataPin), _resetPin(resetPin)
{  
  pinMode(clockPin, INPUT_PULLUP);  
  pinMode(dataPin, INPUT_PULLUP);
  
  if(streamMode) {
    bitcount = 0;
    n = 0;
    val = 0;
    incoming = 0;
    counter = 0;
    dataAvailable = 0;
  }
  
  gohi(_clkPin);
  gohi(_dataPin);
} //TrackPoint

// default destructor
TrackPoint::~TrackPoint()
{
} //~TrackPoint

uint8_t TrackPoint::reportAvailable(void) {
  return dataAvailable;
}

TrackPoint::DataReport TrackPoint::getStreamReport(void) {
  return data;
}

void TrackPoint::getDataBit(void) {
  dataAvailable = 0;

  val = (digitalRead(_dataPin) ? 1 : 0);
  n = bitcount-1;
  if(n <= 7) {
    incoming |= (val << n);
  }
  bitcount++;

  if(bitcount == 11) {
    switch(counter) {
      case 0:
      data.state = incoming;
      counter++;
      break;

      case 1:
      data.x = incoming;
      counter++;
      break;

      case 2:
      data.y = incoming;
      counter = 0;
      dataAvailable = 1;
      break;
    }
    bitcount = 0;
    incoming = 0;
  }
}

// errors are ignored
void TrackPoint::writeToRamLocation(uint8_t location, uint8_t value) {
  write(0xe2);
  read(); // ACK
  write(0x81);
  read(); // ACK
  write(location);
  read(); // ACK
  write(value);
  read(); // ACK
}

// undefined in case of error
uint8_t TrackPoint::readFromRamLocation(uint8_t location) {
  write(0xe2);
  read(); // ACK
  write(0x80);
  read(); // ACK
  write(location);
  read(); // ACK
  return read();
}

void TrackPoint::setSensitivityFactor(uint8_t sensitivityFactor) {
  writeToRamLocation(0x4a, sensitivityFactor);
}

uint8_t TrackPoint::sensitivityFactor() {
  return readFromRamLocation(0x4a);
}

void TrackPoint::setRemoteMode() {
  write(0xf0);
  read();
}

void TrackPoint::setStreamMode() {
  write(0xea);
  read();
  write(0xf4); //enable report
  read();
  
  //put mouse into idle mode, ready to send
  gohi(_clkPin);
  gohi(_dataPin);
}

void TrackPoint::reset() {
  if(_resetPin != 0) { 
    //Arduino doesn't have a pin 0, user is using a separate pin for the RESET line
    pinMode(_resetPin, OUTPUT);
    digitalWrite(_resetPin, HIGH);
    delay(2000); // empirical value
    digitalWrite(_resetPin, LOW); 
  } else {
    /*
      an inverter RC circuit is being used, pull CLOCK low for
      5ms to charge RESET line, then leave it there for 2s,
      then pull CLOCK high for 5ms to discharge the RC circuit
      to pull RESET line LOW.
    */
    golo(_clkPin);
    delay(5);
    delay(2000);
    gohi(_clkPin);
    delay(10);
    
    gohi(_clkPin);
  }
}

TrackPoint::DataReport TrackPoint::readData() {
  DataReport d;

  write(0xeb);
  read(); // ACK
  d.state = read();
  d.x = read();
  d.y = read();

  return d;
}

// Comments and code below mostly unchanged from `ps2.h`.

/*
* according to some code I saw, these functions will
* correctly set the clock and data pins for
* various conditions.  It's done this way so you don't need
* pullup resistors.
*/
void TrackPoint::gohi(uint8_t pin) {
  pinMode(pin, INPUT_PULLUP);
  digitalWrite(pin, HIGH);
}

void TrackPoint::golo(uint8_t pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

/* write a uint8_t to the PS2 device */
void TrackPoint::write(uint8_t data) {
  uint8_t i;
  uint8_t parity = 1;

  gohi(_dataPin);
  gohi(_clkPin);
  delayMicroseconds(300);
  golo(_clkPin);
  delayMicroseconds(300);
  golo(_dataPin);
  delayMicroseconds(10);
  gohi(_clkPin);  // start bit
  // wait for device to take control of clock
  while (digitalRead(_clkPin) == HIGH); // this loop intentionally left blank
  // clear to send data
  for (i=0; i < 8; i++){
    if (data & 0x01)
      gohi(_dataPin);
    else
      golo(_dataPin);

    // wait for clock
    while (digitalRead(_clkPin) == LOW);
    while (digitalRead(_clkPin) == HIGH);
    parity = parity ^ (data & 0x01);
    data = data >> 1;
  }
  // parity bit
  if (parity)
    gohi(_dataPin);
  else
    golo(_dataPin);
   
  // clock cycle - like ack.
  while (digitalRead(_clkPin) == LOW);
  while (digitalRead(_clkPin) == HIGH);
 
  // stop bit
  gohi(_dataPin);
  delayMicroseconds(50);
  while (digitalRead(_clkPin) == HIGH);
 
  // mode switch
  while ((digitalRead(_clkPin) == LOW) || (digitalRead(_dataPin) == LOW));
  // hold up incoming data
  golo(_clkPin);
}


// read a uint8_t of data from the ps2 device.  Ignores parity.
uint8_t TrackPoint::read(void) {
  uint8_t data = 0x00;
  uint8_t i;
  uint8_t bit = 0x01;

  // start clock
  gohi(_clkPin);
  gohi(_dataPin);
  delayMicroseconds(50);
  while (digitalRead(_clkPin) == HIGH);
  delayMicroseconds(5); // not sure why.
  while (digitalRead(_clkPin) == LOW);  // eat start bit
  for (i=0; i < 8; i++){
    while (digitalRead(_clkPin) == HIGH);
    if (digitalRead(_dataPin) == HIGH){
      data = data | bit;
    }
    while (digitalRead(_clkPin) == LOW);
    bit = bit << 1;
  }
  // eat parity bit, ignore it.
  while (digitalRead(_clkPin) == HIGH);
  while (digitalRead(_clkPin) == LOW);
  // eat stop bit
  while (digitalRead(_clkPin) == HIGH);
  while (digitalRead(_clkPin) == LOW);
  golo(_clkPin);  // hold incoming data

  return data;
}


TrackPoint trackpoint(TRACKPOINT_CLK, TRACKPOINT_DATA, TRACKPOINT_RESET, true);

inline void mouseButtonProccess(uint8_t buttonBit, uint8_t mouseButton)
{  
  if(keyboard.isPressed(buttonBit)){
    if(!Mouse.isPressed(mouseButton))
      Mouse.press(mouseButton);      
  }
  else
    if(Mouse.isPressed(mouseButton))
      Mouse.release(mouseButton);  
}

  String mouse;
  int x,y;
  int trackpointDriftThreshold = 2;
  int decreaseCoeff = 2;
  TrackPoint::DataReport dataTrackpoint;
  
inline void mouseProccessing(){
  dataTrackpoint = trackpoint.getStreamReport();
  
  x = abs(dataTrackpoint.x) > trackpointDriftThreshold ? 
          dataTrackpoint.x:0; // 2 is threshold, because trackpoint have drift
          
  y = abs(dataTrackpoint.y) > trackpointDriftThreshold ?
          dataTrackpoint.y:0; // 2 is threshold, because trackpoint have drift
  
  if(Mouse.isPressed(MOUSE_MIDDLE))
    Mouse.move(0, 0, x/16);
  else
    Mouse.move( -y, -x);

  if(keyboard.bank() == 0)
  {
    mouseButtonProccess(LMB_BIT, MOUSE_LEFT);
    mouseButtonProccess(WMB_BIT, MOUSE_MIDDLE);
    mouseButtonProccess(RMB_BIT, MOUSE_RIGHT);
  }
}

#endif //__TRACKPOINT_H__
