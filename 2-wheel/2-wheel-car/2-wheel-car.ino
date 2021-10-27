

enum DIR {
  BACK = -1,
  STOP = 0,
  FORE = 1
};

typedef struct motor {
  int pin1;
  int pin2;
  int pin_speed;
  DIR dir;      // -1 = back, 0 = stop; 1 = fore
  int mspeed;  // 0 - 255
} motor;

#define MOTOR1_PIN1      2
#define MOTOR1_PIN2      3
#define MOTOR1_SPEED     9

#define MOTOR2_PIN1      4
#define MOTOR2_PIN2      5
#define MOTOR2_SPEED     10

#define MOTOR_BACK       0
#define MOTOR_FRONT      1

#define MIN_SPEED       100
#define MAX_SPEED       255     
#define ACCEL           50

#define FRONT_WALL_PIN  5

motor  all_motors[] = {
  {
    MOTOR1_PIN1,
    MOTOR1_PIN2,
    MOTOR1_SPEED,
    STOP,
    0
  },
  {
    MOTOR2_PIN1,
    MOTOR2_PIN2,
    MOTOR2_SPEED,
    STOP,
    0
  }
};


  

void setup() {
  // put your setup code here, to run once:
//  pinMode(motor1pin1, OUTPUT);
//  pinMode(motor1pin2, OUTPUT);
//  pinMode(motor2pin1, OUTPUT);
//  pinMode(motor2pin2, OUTPUT);
//  pinMode(9, OUTPUT); 
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  all_motor_init();
  pinMode(FRONT_WALL_PIN, INPUT);
}

void all_motor_init() {
  int i;
  for (i = 0; i < sizeof(all_motors) / sizeof(motor); i++){
    Serial.println("Init motor");
    all_motors[i].dir = STOP;
    all_motors[i].mspeed = 0;
    Serial.println("All outputs");
    pinMode(all_motors[i].pin1, OUTPUT);
    pinMode(all_motors[i].pin2, OUTPUT);
    pinMode(all_motors[i].pin_speed, OUTPUT);
  }
   
}

void motorCTRL(int m, DIR dir, int s)
{
  Serial.print(all_motors[m].pin1);
  Serial.print(all_motors[m].pin2);
  Serial.print(all_motors[m].pin_speed);
  if(dir == FORE){
    Serial.println("Motor FORE" + m);
    digitalWrite(all_motors[m].pin1, HIGH);
    digitalWrite(all_motors[m].pin2, LOW);
    analogWrite(all_motors[m].pin_speed, s);
  } else if(dir == BACK){
    Serial.println("Motor BACK");
    digitalWrite(all_motors[m].pin1, LOW);
    digitalWrite(all_motors[m].pin2, HIGH);
    analogWrite(all_motors[m].pin_speed, s);
    all_motors[m].mspeed = s;
  } else {
    Serial.println("Motor STOP");
    digitalWrite(all_motors[m].pin1, LOW);
    digitalWrite(all_motors[m].pin2, LOW);
    digitalWrite(all_motors[m].pin_speed, 0);
    all_motors[m].mspeed = 0;
  }
}



void loop() {
//  Serial.println("Hallo fore");
//  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
//  motorCTRL(0, FORE, 1000);
//  delay(1000);
//  Serial.println("Hallo stop");
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  motorCTRL(0, STOP, 0);
//  delay(1000);
//  Serial.println("Hallo back");
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  motorCTRL(0, BACK, 80);
//  delay(1000);
//  Serial.println("Hallo stop");
//  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
//  motorCTRL(0, STOP, 0);
//  delay(1000);

  Serial.println("Motor speedup");
  
 int val = digitalRead(FRONT_WALL_PIN); // read the input pin
 Serial.print("Value of a2 :");
 Serial.println(val);          // debug value
 for(int i = MIN_SPEED; i < MAX_SPEED; i += ACCEL){
    motorCTRL(0, FORE, i);
    delay(ACCEL * 10);
  }
  Serial.println("Motor speed down");
  for(int i = MAX_SPEED; i >= MIN_SPEED; i -= ACCEL){
    motorCTRL(0, FORE, i);
    delay(ACCEL * 10);
  }
  motorCTRL(0, STOP, 0);
  delay(1000);
  
 for(int i = MIN_SPEED; i < MAX_SPEED; i += ACCEL){
    motorCTRL(0, BACK, i);
    delay(ACCEL * 10);
  }
  Serial.println("Motor speed down");
  for(int i = MAX_SPEED; i >= MIN_SPEED; i -= ACCEL){
    motorCTRL(0, BACK, i);
    delay(ACCEL * 10);
  }
  motorCTRL(0, STOP, 0);
  delay(1000);
  

  
  // put your main code here, to run repeatedly:   
//  analogWrite(9, 150); //ENA pin
//  digitalWrite(motor1pin1, HIGH);
//  digitalWrite(motor1pin2, LOW);
//
//  digitalWrite(motor2pin1, HIGH);
//  digitalWrite(motor2pin2, LOW);
//  delay(1000);
//
//  digitalWrite(motor1pin1, LOW);
//  digitalWrite(motor1pin2, HIGH);
//
//  digitalWrite(motor2pin1, LOW);
//  digitalWrite(motor2pin2, HIGH);
//  delay(1000);
//
//  digitalWrite(motor1pin1, LOW);
//  digitalWrite(motor1pin2, LOW);
//  delay(1000);
//
}
