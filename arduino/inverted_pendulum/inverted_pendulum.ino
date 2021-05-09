
#include <SoftwareSerial.h>
#include <math.h>
SoftwareSerial mySerial(2, 3); // RX, TX

int count = 0;

int hall_sensor_H1 = A0;   
int hall_sensor_H2 = A1;   
int ledPin = 13;
int pwmPin = 11;      
double sensorValue_H1 = 0;  
double sensorValue_H2 = 0;  
unsigned long READ_PERIOD = 800; //us
unsigned long lastRead;
const int moving_avarage_window = 40;  // Has to be an even number
int tick= 0;

uint8_t data_multiplier = 100;
int voltageShift = 250;

uint8_t h1_test_send; // För att skicka via uart
uint8_t h2_test_send; // För att skicka via uart

double H1_buffer = 0;
double H2_buffer = 0;
double H1_send = 0;
double H2_send = 0;
uint16_t dTime = 0;
/**
 *    TODO: 
 *    Kommunikation
 */

void setup() {
  dTime = micros();
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);


  mySerial.begin(9600 ); // Baud rate?


cli();//stop interrupts


  TCCR0A = 0;
  TCCR0B = 0;
  TCNT0  = 0;

  OCR0A = 239;// = (16*10^6) / (65*1024) - 1, 65 Hz
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // prescaler 1024
  TCCR0B |= (1 << CS12) | (1 << CS10);   
  TIMSK0 |= (1 << OCIE0A);
  sei();
  
    //  TA BORT

}

void loop() {
  //sensorValue = analogRead(hall_sensor_H1)*0.0049;  // 10 bitar => 1024 => 5/1024=4.9 mV
  //Serial.println(sensorValue);
  //analogWrite(pwmPin, sensorValue);



  //  Read and handle data
  H1_buffer = read_hall_sensor(hall_sensor_H1, H1_buffer);
  H2_buffer = read_hall_sensor(hall_sensor_H2, H2_buffer);
  //  Send data
  

  
      

  tick++;   
}

void send_data(int H){
  if(H==hall_sensor_H1){
    mySerial.write(h1_test_send);
    Serial.print("H1 sending : ");
    Serial.println(h1_test_send);  
  }else{
    h2_test_send+= 128;
    mySerial.write(h2_test_send);
    Serial.print("H2 sending : ");
    Serial.println(h2_test_send);
  }
  


}

//  Read analog voltage
double read_hall_sensor(int H, double H_buffer){
 
  /*Serial.print("Added: ");
  Serial.print(analogRead(H)*0.0049);
  Serial.print(" at tick: ");
  Serial.println(tick);
  Serial.print(".");*/

  /*
  Serial.print("Reading: ");
  Serial.println(analogRead(H)*0.0049);
  */

  
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
        /*
        Serial.print("H1 avg: ");
        Serial.println(H1_send);
        */
       //Test för att snabba på kommunikation
       H1_send*=data_multiplier;
       /*
       Serial.print("H1 avg: ");
        Serial.println(H1_send);
       */
       
       H1_send-=voltageShift;
       H1_send = round(H1_send);
       h1_test_send = H1_send;
/*
       Serial.print("H1 sending: ");
       Serial.println(h1_test_send); 
  */     
       //send_data(hall_sensor_H1);
       
       
    } else{
      H2_send=H_send/moving_avarage_window;
      /*
      Serial.print("H2 avg: ");
      Serial.println(H2_send);
      */
      H2_send*=data_multiplier;
      H2_send-=voltageShift;
      h2_test_send = round(H2_send);

     /* 
      Serial.print("H2 sending: ");
      Serial.println(h2_test_send);
      */
     //       Serial.print("tick= ");
     // Serial.println(tick); 
      tick = 0;
      //send_data(hall_sensor_H2);
    }
  
}


ISR(TIMER0_COMPA_vect){
   Serial.print("delta Time: ");
   Serial.println(dTime-micros());
   dTime = micros();
   count++;
   if(count==2){
    send_data(hall_sensor_H1); 
    send_data(hall_sensor_H2);
    count=0;
   }
 
   
}
