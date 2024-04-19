//Receiver signal pins
double ch1_pin = 3;  //PWM pin
double ch2_pin = 5;  //PWM pin

int R_PWM_right = 6;  //PWM pin
int L_PWM_right = 9;  //PWM pin

int R_PWM_left = 10;  //PWM pin
int L_PWM_left = 11;  //PWM pin

//Rx threshold values - Update based on your TX values
//Forward Start and forward end
int Ch1Ch2_start_Fwd = 1530;
int Ch1Ch2_End_Fwd = 1980;
//backward Start and backward end
int Ch1Ch2_start_Bac = 1460;
int Ch1Ch2_End_Bac = 980;

int forward_speed_RightLeft;
int backward_speed_RightLeft;


void setup() {
  Serial.begin(9600);
  pinMode(ch1_pin, INPUT);
  pinMode(ch2_pin, INPUT);
  pinMode(R_PWM_right, OUTPUT);
  pinMode(L_PWM_right, OUTPUT);
  pinMode(R_PWM_left, OUTPUT);
  pinMode(L_PWM_left, OUTPUT);
}

void loop() {
  //Receiver signal pins data
  double ch1 = pulseIn(3, HIGH);
  double ch2 = pulseIn(5, HIGH);

  Serial.print("ch1: ");
  Serial.print(ch1);
  Serial.println("    ");
  //Serial.print("ch2: ");
  // Serial.print(ch2);
  // Serial.println("    ");

  //Speed mapping for right motor driver
  forward_speed_RightLeft = map(ch1, Ch1Ch2_start_Fwd, Ch1Ch2_End_Fwd, 0, 255);
  backward_speed_RightLeft = map(ch1, Ch1Ch2_start_Bac, Ch1Ch2_End_Bac, 0, 255);

  //pwm limit setup
  if (forward_speed_RightLeft > 255) {
    forward_speed_RightLeft = 255;
  } else if (forward_speed_RightLeft < 0) {
    forward_speed_RightLeft = 0;
  }
  if (backward_speed_RightLeft < 0) {
    backward_speed_RightLeft = 0;
  } else if (backward_speed_RightLeft > 255) {
    backward_speed_RightLeft = 255;
  }

  //stop
  if ((ch1 == 0) && (ch2 == 0)) stop();
  //FWD
  else if ((ch1 > Ch1Ch2_start_Fwd) && (ch2 > Ch1Ch2_start_Fwd)) straight();
  //RIGHT
  else if ((ch1 > Ch1Ch2_start_Fwd) && (ch2 < Ch1Ch2_start_Bac)) right();
  //LEFT
  else if ((ch1 < Ch1Ch2_start_Bac) && (ch2 > Ch1Ch2_start_Fwd)) left();
  //BACK
  else if ((ch1 < Ch1Ch2_start_Bac) && (ch2 < Ch1Ch2_start_Bac)) backward();
  //stop
  else stop();
}
//moto run function
void stop() {
  analogWrite(R_PWM_right, 0);
  analogWrite(L_PWM_right, 0);
  analogWrite(R_PWM_left, 0);
  analogWrite(L_PWM_left, 0);
}
void straight() {
  analogWrite(R_PWM_right, forward_speed_RightLeft);
  analogWrite(L_PWM_right, 0);
  analogWrite(R_PWM_left, 0);
  analogWrite(L_PWM_left, forward_speed_RightLeft);
}
void right() {
  analogWrite(R_PWM_right, 0);
  analogWrite(L_PWM_right, forward_speed_RightLeft);
  analogWrite(R_PWM_left, 0);
  analogWrite(L_PWM_left, forward_speed_RightLeft);
}
void left() {
  analogWrite(R_PWM_right, backward_speed_RightLeft);
  analogWrite(L_PWM_right, 0);
  analogWrite(R_PWM_left, backward_speed_RightLeft);
  analogWrite(L_PWM_left, 0);
}
void backward() {
  analogWrite(R_PWM_right, 0);
  analogWrite(L_PWM_right, backward_speed_RightLeft);
  analogWrite(R_PWM_left, backward_speed_RightLeft);
  analogWrite(L_PWM_left, 0);
}
