#include <FlexyStepper.h>
#include <Wire.h>

//
// pin assignments
//
const int ElMOTOR_STEP_PIN = 11;
const int ElMOTOR_DIRECTION_PIN = 12;

const int AzMOTOR_STEP_PIN = 9;
const int AzMOTOR_DIRECTION_PIN = 10;



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
float fEl;//elevation in degrees
float fAz;//az in degrees
float X;//Enable/Disable Steppers 0=disable
float J;//Jog El in steps
float K;//Jog Az in steps
float mvsteps=0;


char i2cRX[50];
char i2cTX[50];
int state = 0;

int INV_STEPPER_ENABLE=8; //Stepper Enable Pin

String sTemp;

void setup() {
  // put your setup code here, to run once:


  stepperAz.connectToPins(AzMOTOR_STEP_PIN, AzMOTOR_DIRECTION_PIN);
  stepperAz.setStepsPerRevolution(38400);
  stepperAz.setCurrentPositionInSteps(0);
  stepperAz.setSpeedInRevolutionsPerSecond(0.005);
  stepperAz.setAccelerationInStepsPerSecondPerSecond(100);

  stepperEl.connectToPins(ElMOTOR_STEP_PIN, ElMOTOR_DIRECTION_PIN);
  stepperEl.setStepsPerRevolution(38400);
  stepperEl.setCurrentPositionInSteps(0);
  stepperEl.setSpeedInRevolutionsPerSecond(0.005);
  stepperEl.setAccelerationInStepsPerSecondPerSecond(100);

  pinMode(INV_STEPPER_ENABLE,OUTPUT); //
  digitalWrite(INV_STEPPER_ENABLE,false); //false is enable stepper drivers

  // initialize i2c as slave
  Serial.begin(115200);
  Wire.begin(SLAVE_ADDRESS);
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Setup Done");

  }


float old;
float currentpos;
float Z=0;
int i;


void loop() {
  if (fAz>360){
    fAz=360;
  }
  if (fAz<-360){
    fAz=-360;
  }
  stepperAz.setTargetPositionInRevolutions(degToRot(fAz));

  if (fEl<0){ //Prevent pointing down.
    fEl=0;
  }
  if (fEl>80){ //Prevent pointing up.
    fEl=80;
  }
  stepperEl.setTargetPositionInRevolutions(degToRot(fEl));


//processMovement
  if (!stepperAz.motionComplete()){
      stepperAz.processMovement();
  }
  if (!stepperEl.motionComplete()){
      stepperEl.processMovement();
  }



  if (Z !=0){//Set stepper zero
    digitalWrite(INV_STEPPER_ENABLE,true); //true is disable stepper drivers

    for (int i = 0; i <= Z; i++) {
      delay(1000);
    }


    digitalWrite(INV_STEPPER_ENABLE,false); //false is disable stepper drivers
    stepperAz.setCurrentPositionInSteps(0);
    stepperEl.setCurrentPositionInSteps(0);

    Z=0;
  }


  if (J !=0){//Jog Elevation Steps

    for (int i = 0; i <= J; i++) {
      stepperEl.moveRelativeInSteps(J);
      J=0;
    }
  }

  if (K !=0){//Jog Az Steps

    for (int i = 0; i <= J; i++) {
      stepperEl.moveRelativeInSteps(J);
      J=0;
    }
  }



  if(X=1){
    digitalWrite(INV_STEPPER_ENABLE,false); //false is enable stepper drivers
  }

  if(X=0){
    digitalWrite(INV_STEPPER_ENABLE,true); //true is disable stepper drivers
  }




  delay(1); //delay 1ms
} // end loop


float degToRot(float deg)
{
  return deg/360;
}




void GetSerialData(){
  int val=0;
//Serial.println("GettingSerial");
while (Serial.available() == 0);
  val = Serial.parseInt(); //read int or parseFloat for ..float...
  rx=val;
}

// callback for received data
void receiveData(int byteCount) {
  int i = 0;
  int iTemp=0;
  String mystring;
  String sTemp;

    while (Wire.available()) {
      i2cRX[i] = Wire.read();
      i++;
    }

    if (i2cRX[0]=='E'){//Elevation Target
      sTemp=String(i2cRX);
      sTemp=sTemp.substring(2,sTemp.length());
      fEl=sTemp.toFloat();
    }

    if (i2cRX[0]=='A'){//Azimuth Target
      sTemp=String(i2cRX);
      sTemp=sTemp.substring(2,sTemp.length());
      fAz=sTemp.toFloat();
    }
    if (i2cRX[0]=='Z'){//Home Servos, parameter is seconds to wait.
      sTemp=String(i2cRX);
      sTemp=sTemp.substring(2,sTemp.length());
      Z=sTemp.toFloat();
    }

    if (i2cRX[0]=='X'){//Enable (1=ON, 0=OFF)
      sTemp=String(i2cRX);
      sTemp=sTemp.substring(2,sTemp.length());
      X=sTemp.toFloat();
    }

    if (i2cRX[0]=='J'){//Jog X Steps
      sTemp=String(i2cRX);
      sTemp=sTemp.substring(2,sTemp.length());
      J=sTemp.toFloat();
    }
    if (i2cRX[0]=='K'){//Jog X Steps
      sTemp=String(i2cRX);
      sTemp=sTemp.substring(2,sTemp.length());
      K=sTemp.toFloat();
    }

  memset(i2cRX, 0, sizeof(i2cRX));  //Clear i2cRX

}


// callback for sending data
void sendData() {
  Wire.write(i2cTX);
}

//End of the program
