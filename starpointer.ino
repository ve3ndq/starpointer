#include <FlexyStepper.h>
#include <Wire.h>

//
// pin assignments
//
const int ElMOTOR_STEP_PIN = 9;
const int ElMOTOR_DIRECTION_PIN = 10;

const int AzMOTOR_STEP_PIN = 11;
const int AzMOTOR_DIRECTION_PIN = 12;



/*
  I2C Pinouts
  SDA -> A4
  SCL -> A5
*/

//i2c address
#define SLAVE_ADDRESS 0x04
FlexyStepper stepperEl;
FlexyStepper stepperAz;

int rx =0;
float AzTarget =0;
float ElTarget =0;
float fEl;
float fAz;

float mvsteps=0;


char i2cRX[50];
char i2cTX[50];
int state = 0;

String sTemp;

void setup() {
  // put your setup code here, to run once:

 
  stepperAz.connectToPins(AzMOTOR_STEP_PIN, AzMOTOR_DIRECTION_PIN);
  stepperAz.setStepsPerRevolution(16*200);
  stepperAz.setCurrentPositionInSteps(0);
  stepperAz.setSpeedInRevolutionsPerSecond(1);
  stepperAz.setAccelerationInStepsPerSecondPerSecond(3000);

  stepperEl.connectToPins(ElMOTOR_STEP_PIN, ElMOTOR_DIRECTION_PIN);
  stepperEl.setStepsPerRevolution(16*200);
  stepperEl.setCurrentPositionInSteps(0);
  stepperEl.setSpeedInRevolutionsPerSecond(1);
  stepperEl.setAccelerationInStepsPerSecondPerSecond(3000);


  
  // initialize i2c as slave
  Serial.begin(38400);
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Setup Done");

  }


float old;
void loop() {
  
  stepperAz.setTargetPositionInRevolutions(degToRot(fEl));
  stepperEl.setTargetPositionInRevolutions(degToRot(fEl));
  if (!stepperAz.motionComplete()){
      stepperAz.processMovement();
  }
  if (!stepperEl.motionComplete()){
      stepperEl.processMovement();
  }

//  stepperAz.moveToPositionInRevolutions(degToRot(fEl));
//  stepperEl.moveToPositionInRevolutions(degToRot(fEl));
/*
  while((!stepperAz.motionComplete()) || (!stepperEl.motionComplete()))
    {

      stepperAz.processMovement();
      stepperEl.processMovement();

    }
*/
  delay(1);
} // end loop


float degToRot(float deg)
{
  return deg/360; 
}




void GetSerialData(){
  int val=0;
Serial.println("GettingSerial");
while (Serial.available() == 0);
  val = Serial.parseInt(); //read int or parseFloat for ..float...

rx=val;
Serial.println(val);

}




// callback for received data
void receiveData(int byteCount) {
  int i = 0;
  int iTemp=0;
  String mystring;
  String sTemp;
  
  //Serial.print("Avail: ");
  //Serial.print(Wire.available());
  //Serial.print("\n");
  Serial.print("i2cRX>");
    while (Wire.available()) {
      i2cRX[i] = Wire.read();
/*    Serial.print("|");
    Serial.print(int(i2cRX[i]));
    Serial.print(i2cRX[i]);
    Serial.print("|");
    
*/
    Serial.print(i2cRX[i]);
    i++;
    }
 
    Serial.println("<i2cRX");     i++;
    Serial.print("Complete i2cRX:");
    Serial.println(i2cRX);

    if (i2cRX[0]=='E'){
      sTemp=String(i2cRX);
      sTemp=sTemp.substring(2,sTemp.length());
      Serial.println("sTemp:" + sTemp);
      fEl=sTemp.toFloat();
      Serial.print("fEl:");
      Serial.println(fEl);

    if (i2cRX[0]=='A'){
      sTemp=String(i2cRX);
      sTemp=sTemp.substring(2,sTemp.length());
      Serial.println("sTemp:" + sTemp);
      fAz=sTemp.toFloat();
      Serial.print("fAz:");
      Serial.println(fAz);
    }
    }
    
  


  memset(i2cRX, 0, sizeof(i2cRX));  //Clear i2cRX
  }  // end while
// callback for sending data



void sendData() {
  Wire.write(i2cTX);
}

//End of the program
