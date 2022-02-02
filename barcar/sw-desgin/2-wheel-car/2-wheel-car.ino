
/*
 * pUBLIC FUNCTIONS
 */
void all_motor_init();


enum CAR_DIR {
  STOP = 0,
  BACK = 2,
  FORE = 1
};

#define MIN_SPEED       80
#define MAX_SPEED       90     
// #define MAX_SPEED       255
#define ACCEL_DELAY     1000
#define ACCEL           (MAX_SPEED - MIN_SPEED) / 2

#define FRONT_WALL_PIN  25
#define BACK_WALL_PIN   26
#define LED_BUILTIN     25
#define ADC             36     

typedef struct motor_t {
  int pin1;
  int pin2;
  int pin_speed;
  int pwm_ch, pwm_res, pwm_freq;
  CAR_DIR dir;      // -1 = back, 0 = stop; 1 = fore
  int cur_speed;  // current speed 0 - 255
} motor_t;

motor_t  motor[] = {
  {
    /* pin1, pin2, pwm */
       18  , 19 , 16,
    /* 
     *  PWM
     *  ch, res, freq */
        0 , 8  , 500,
    STOP, // Current direction
    0     // Current speed
  }
//  ,
//  {
//    /* pin1, pin2, pwm */
//       18  , 19 , 16,
//    /* 
//     *  PWM
//     *  ch, res, freq */
//        1 , 8  , 500,
//    STOP, // Current direction
//    0     // Current speed
//  }
};

/*
 * ADC table
 * 2.796V = 3210
 * 2.771V = 3160
 * 2.646V = 3045
 */

#ifdef IM_A_SLAVE
void slave_setup() {
#else
void setup() {
#endif

#ifndef IM_A_SLAVE
  Serial.begin(115200);
#endif
  pinMode(LED_BUILTIN, OUTPUT);
  all_motor_init();
  pinMode(FRONT_WALL_PIN, INPUT);
  pinMode(BACK_WALL_PIN, INPUT);
}

void all_motor_init() {
  int i;
  for (i = 0; i < 1; i++){ // sizeof(motor) / sizeof(motor); i++){
    Serial.println("Init motor");
    motor[i].dir = STOP;
    motor[i].cur_speed = 0;
    Serial.println("All outputs");
    pinMode(motor[i].pin1, OUTPUT);
    pinMode(motor[i].pin2, OUTPUT);
    pinMode(motor[i].pin_speed, OUTPUT);
    digitalWrite(motor[i].pin_speed, HIGH);
    ledcSetup(motor[i].pwm_ch, motor[i].pwm_freq, motor[i].pwm_res);
    ledcAttachPin(motor[i].pin_speed, motor[i].pwm_ch);   
  }
   
}

void motorCTRL(int m, CAR_DIR dir, int s)
{
  // Serial.printf("Motor ctrl pins : %d, %d, %d\n", motor[m].pin1, motor[m].pin2, motor[m].pin_speed);
  // Serial.printf("Motor pin val : 1(%d), 2(%d) ; Speed(%d)\n", motor[m].dir & 1, (motor[m].dir >> 1 & 1), s);
  ledcWrite(motor[m].pwm_ch, s);
  motor[m].dir = dir;
  motor[m].cur_speed = s;
  digitalWrite(motor[m].pin1, (int)motor[m].dir & 1);
  digitalWrite(motor[m].pin2, (int)(motor[m].dir >> 1 & 1));
  return;
  
  if(dir == FORE){
    Serial.println("Motor FORE" + m);
    digitalWrite(motor[m].pin1, HIGH);
    digitalWrite(motor[m].pin2, LOW);
  } else if(dir == BACK){
    Serial.println("Motor BACK");
    digitalWrite(motor[m].pin1, LOW);
    digitalWrite(motor[m].pin2, HIGH);
  } else {
    Serial.println("Motor STOP");
    digitalWrite(motor[m].pin1, LOW);
    digitalWrite(motor[m].pin2, LOW);
  }
}


void printSensors()
{
  Serial.printf("Sensor front - back : %d - %d , ADC %d %2.2f V \n", digitalRead(FRONT_WALL_PIN), digitalRead(BACK_WALL_PIN) , analogRead(ADC), (float)analogRead(ADC) * 0.000871028037383);
}

void check(){
  printSensors();
  while(digitalRead(FRONT_WALL_PIN) == 0 && digitalRead(BACK_WALL_PIN) == 0){
    Serial.printf("Emergency STOP!!!\n");
    printSensors();
    motorCTRL(0, STOP, 0);
    delay(500);
  }
}

void driveCar(CAR_DIR dir, int sensor){
#ifdef IM_A_SLAVE
  server.handleClient();
  if(barCarIsRunning == 0){
    motorCTRL(0, STOP, 0);
    return;
  }
#endif
  Serial.println("Motor speedup");
  motorCTRL(0, STOP, 0);
  for(int i = MIN_SPEED; i < MAX_SPEED; i += ACCEL){
    check();
    if (digitalRead(sensor) != 1)
      break;
    motorCTRL(0, dir, i);
    delay(ACCEL_DELAY);
  }
  while(digitalRead(sensor) == 1){
    printSensors();
#ifdef IM_A_SLAVE
    server.handleClient();
    if(barCarIsRunning == 0){
      motorCTRL(0, STOP, 0);
      return;
   }
#endif
    delay(200);
  }
  Serial.println("Motor speed down");
  for(int i = MAX_SPEED; i >= MIN_SPEED && 0; i -= ACCEL){
    check();
     motorCTRL(0, dir, i);
    delay(ACCEL_DELAY);
  }
  motorCTRL(0, STOP, 0);
}
#ifdef IM_A_SLAVE
void slave_loop() {
 #else
void loop() {
 #endif

  
  printSensors();
  motorCTRL(0, STOP, 0);
  if (digitalRead(FRONT_WALL_PIN) == HIGH){
    Serial.println("Go forward!\n");
    driveCar(FORE, FRONT_WALL_PIN);
    Serial.println("Go forward! ends\n");
    delay(1000);
  } 
  if (digitalRead(BACK_WALL_PIN) == HIGH){
    Serial.println("Go forward!\n");
    driveCar(BACK, BACK_WALL_PIN);
    delay(1000);
  }
  return;
}
