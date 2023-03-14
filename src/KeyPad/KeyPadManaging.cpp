/*
  Aquarium Project Pasquale
*/

#include "KeyPadManaging.h"

// Get kayboard value from analogic signal and return key
byte KeyPad::getKeyPadValue(){
  int val = 0;
  val = analogRead(keypadPin);
  int analog_voltage = (val * 3.3 / 4095) * 100;
  //Serial.println(val);
  //Serial.print("Volatge reading = ");
  //Serial.println(analog_voltage);
  if (analog_voltage >= 0 && analog_voltage < 280) {
    if (analog_voltage >= 0 && analog_voltage < 21) { //left
      return codeKeys[0];
    } else if (analog_voltage >= 21 && analog_voltage < 61) { //up
      return codeKeys[3];
    } else if (analog_voltage >= 61 && analog_voltage < 121) { //down
      return codeKeys[2];
    } else if (analog_voltage >= 121 && analog_voltage < 191) { //right
      return codeKeys[1];
    } else {//if(analog_voltage > 191 && analog_voltage < 280){ //ok
      return codeKeys[4];
    }
  } else { //no press
    return codeKeys[6];
  }
}

byte KeyPad::getResetKeyPadValue() {
  return codeKeys[6];
}

byte KeyPad::getNullKeyPadValue() {
  return codeKeys[5];
}

byte KeyPad::getLeftKeyPadValue() {
  return codeKeys[1];
}

byte KeyPad::getRightKeyPadValue() {
  return codeKeys[0];
}

byte KeyPad::getUpKeyPadValue() {
  return codeKeys[2];
}

byte KeyPad::getDownKeyPadValue() {
  return codeKeys[3];
}

byte KeyPad::getOkKeyPadValue() {
  return codeKeys[4];
}

/*if (val >= 0 && val < 4000) {
    if (val >=0 && val < 200) { //left
      return codeKeys[1];
    } else if (val > 350 && val < 550) { //up
      return codeKeys[2];
    } else if (val > 1050 && val < 1300) { //down
      return codeKeys[3];
    } else if (val > 1700 && val < 2000) { //right
      return codeKeys[0];
    } else {//if(val >756 && val < 880){ //ok
      return codeKeys[4];
    }
  } else { //no press
    return codeKeys[6];
  }*/