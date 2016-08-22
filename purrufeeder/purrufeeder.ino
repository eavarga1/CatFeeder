//CAT FEEDER 
//test
//test2
//2.10ESP8266 wifi uploading data to PVCloud
//2.9 pin 3 and 2 changed by 0 and 1 for using it for ESP8266
//2.8 whater implementation 
//2.7 lost due to overwrite
//2.6 better sequense to dispense avoiding jams
//2.5 Cat is scare to vibration need to load delayed after cat left.
//2.0 implemented ultrasonic sensor due to errors with infrared due to light changes.


//motor A connected between A01 and A02
//motor B connected between B01 and B02

//pins declarations 
int STBY = 10; //standby
const int whater = 0; // pin activation whater

//Motor A
const int PWMA = 1; //Speed control 
const int AIN1 = 9; //Direction
const int AIN2 = 8; //Direction

//Motor B vibrador
const int PWMB = 5; //Speed control
const int BIN1 = 11; //Direction
const int BIN2 = 12; //Direction

const int trig = 6;   //ultrasonic sensor pins
const int echo = 7;
const int led = 13;

long duration;     //holds pulse width proporsional to distance
int cm = 0;        // holds distance in centimeters

void setup(){
 // Serial.begin(9600);
  pinMode(STBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT);
  pinMode(led, OUTPUT);
  pinMode(whater, OUTPUT);
  digitalWrite (whater, HIGH);
  
}
void loop(){


measure();       //subrutine to measure

//only one mearure cause false trigers /           
 if (cm < 17){
          int dist_count = 0;
          for (int i=0; i <= 10; i++){
              measure();
                if (cm < 17) {
                    dist_count++;
                }

          }

      if (dist_count > 9) {  

    //cat is detected but no dispense until it left (10 minutes)  
    digitalWrite (led, HIGH);  // to show cat is been detected while waiting 10 minutes   
    //for (int variable2 = 10; variable2 <=1; variable2--) { 
    delay(1000); //600000 delay of ten minutes before sequence to not scare the cat, 
    //}  
     digitalWrite (led, LOW);

// secuence to dispense cat food, if only one direction no vibrations cause jams.
  move(2, 255, 1); //motor 2 vibrator, full speed, left
  delay(300);
  move(2, 0, 1); //motor 2 vibrador, 0 speed, left
  delay(150);
  move(2, 255, 0); //motor 2 vibrador, full speed, right
  delay(300);
  move(2, 0, 1); //motor 2 vibrador, 0 speed, left 
  stop(); 
        
        
        
        for (int i=0; i <= 10; i++){ 
        move(1, 255, 1); //motor 1, full speed, left
        delay(85); //ms
        move(1, 0, 1); //motor 1, 0 speed, left

                 move(2, 255, 0); //motor 2 vibrador, full speed, right
                 delay(50); 
                 move(2, 0, 0);
               // stop(); //stop
        
        //delay(30); //hold for 250ms until move again
        move(1, 255, 0); //motor 1, full speed, right
        delay(60);
        move(1, 0, 0); //motor 1, 0 speed, right

                 move(2, 255, 1); //motor 2 vibrador, full speed, left
                 delay(50); 
                 move(2, 0, 1);
                 stop();
    
        }
        

  move(2, 255, 1); //motor 2 vibrador, full speed, left
  delay(600);
  move(2, 0, 1); //motor 2 vibrador, 0 speed, left
  stop();

  delay(30);  
  
  move(2, 255, 0); //motor 2 vibrador, full speed, left
  delay(2000);
  move(2, 0, 1); //motor 2 vibrador, 0 speed, left
  stop();
  
  delay(60000);
  digitalWrite(whater, LOW);
  delay(420000);   //water pump on 420000
  digitalWrite(whater, HIGH);
  delay(7200000);       // 7200000  2 hours delay to not overfeed cat can come during this time but not feed again
 //    for (int variable2 = 120; variable2 <=1; variable2--) { 
 // delay(60000); //delay of one minute
   //  }
           }

           }
           }


// subrutineto move dc motors

void move(int motor, int speed, int direction){
//Move specific motor at speed and direction
//motor: 0 for B 1 for A
//speed: 0 is off, and 255 is full speed
//direction: 0 clockwise, 1 counter-clockwise
  digitalWrite(STBY, HIGH); //disable standby
  boolean inPin1 = LOW;
  boolean inPin2 = HIGH;
  if(direction == 1){
    inPin1 = HIGH;
    inPin2 = LOW;
  }
  if(motor == 1){
    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    analogWrite(PWMA, speed);
  }else{
    digitalWrite(BIN1, inPin1);
    digitalWrite(BIN2, inPin2);
    analogWrite(PWMB, speed);
  }
}


//subrutine to stop dc motors , both at the same time only one standby pin for both
void stop(){
//enable standby  
  digitalWrite(STBY, LOW);  
}

//subrutine to measure distance with ultrasonic sensor
  int measure() {
      digitalWrite(trig, LOW);
      delay(10);
      digitalWrite(trig, HIGH);
      delay(10);
      digitalWrite(trig, LOW); 
      duration = pulseIn(echo, HIGH);
      cm = microsecondsToCentimeters(duration); 
          // Serial.print(cm);
          // Serial.print("cm");
          // Serial.println();
      }


//subrutine to calculate distance
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
  }



  
