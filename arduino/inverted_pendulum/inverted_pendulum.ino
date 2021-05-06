

int hall_sensor_H1 = A0;   
int hall_sensor_H2 = A1;   
int ledPin = 13;
int pwmPin = 11;      
double sensorValue_H1 = 0;  
double sensorValue_H2 = 0;  
unsigned long READ_PERIOD = 800; //us
unsigned long lastRead;
const int moving_avarage_window = 100;  // Has to be an even number
int tick= 0;


double H1_buffer = 0;
double H2_buffer = 0;
double H1_send = 0;
double H2_send = 0;

/**
 *    TODO: 
 *    Kommunikation
 */

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);



}

void loop() {
  //sensorValue = analogRead(hall_sensor_H1)*0.0049;  // 10 bitar => 1024 => 5/1024=4.9 mV
  //Serial.println(sensorValue);
  //analogWrite(pwmPin, sensorValue);

  //  Read and handle data
  H1_buffer = read_hall_sensor(hall_sensor_H1, H1_buffer);
  H2_buffer = read_hall_sensor(hall_sensor_H2, H2_buffer);
  //  Send data
 // Serial.print("H1 sending: ");
 // Serial.println(H1_send);
  tick++;   
}

//  Read analog voltage
double read_hall_sensor(int H, double H_buffer){
 
  /*Serial.print("Added: ");
  Serial.print(analogRead(H)*0.0049);
  Serial.print(" at tick: ");
  Serial.println(tick);
  //Serial.print(".");*/
  H_buffer+=analogRead(H)*0.0049;                     //  10 bits => 1024 => 5 V / 1024 = 4.9 mV
  if(!(tick%moving_avarage_window)){
    digitalLowPass(H, H_buffer);
    return 0;
  }
  return H_buffer;
}

//  Digital Low Pass filtration
void digitalLowPass(int H, double H_send){
      if(H==hall_sensor_H1){
      H1_send=H_send/moving_avarage_window;
      Serial.print("H1 sending: ");
      Serial.println(H1_send);  
    } else{
      H2_send=H_send/moving_avarage_window;
      Serial.print("H2 sending: ");
      Serial.println(H2_send); 
            Serial.print("tick= ");
      Serial.println(tick); 
      tick = 0;
    }
}

//
