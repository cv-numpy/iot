#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;


#define thumb = 0

#define thumb1 = 1

#define thumb2 = 2

#define thumb3 = 3

#define thumb4 = 4


#define indexFinger = 0

#define indexFinger1 = 1

#define indexFinger2 = 2

#define indexFinger3 = 3

#define indexFinger4 = 4


#define middleFinger = 0

#define middleFinger1 = 1

#define middleFinger2 = 2

#define middleFinger3 = 3

#define middleFinger4 = 4


#define ringFinger = 0

#define ringFinger1 = 1

#define ringFinger2 = 2

#define ringFinger3 = 3

#define ringFinger4 = 4


#define littleFinger = 0

#define littleFinger1 = 1 

#define littleFinger2 = 2

#define littleFinger3 = 3

#define littleFinger4 = 4



static const int servo_index_thumb[] =         { 0, 1};
static const int servo_index_indexFinger[] =   { 2, 3};
static const int servo_index_middleFinger[] =  { 4, 5};
static const int servo_index_ringFinger[] =    { 6, 7};
static const int servo_index_littleFinger[] =  { 8, 9};


void move_a_finger(int servo_index[], int set_position){
  int position1; int position2;
  position1 = 150+set_position;
  position2 = 600-set_position;
  pwm.setPWM(servo_index[0], 0, position1);
  pwm.setPWM(servo_index[1], 0, position2);
  }

void init_position()
{
  int position0 = 0;
  move_a_finger(servo_index_thumb, position0);
  move_a_finger(servo_index_indexFinger, position0);
  move_a_finger(servo_index_middleFinger, position0);
  move_a_finger(servo_index_ringFinger, position0);
  move_a_finger(servo_index_littleFinger, position0);

}



void setup() {
  Serial.begin(9600);
  Serial.println("8 channel Servo test!");

  pwm.begin();

  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  delay(10);

  
  init_position();
  Serial.println("Positioning.");  
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}


void loop() {
  // Drive each servo one at a time using setPWM()
  Serial.println(servonum);
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
  }

  delay(500);
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
  }

  delay(500);

  servonum++;
  if (servonum > 9) servonum = 0; // Testing the first 8 servo channels
}
