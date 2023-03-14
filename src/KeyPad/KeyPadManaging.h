#ifndef KeyPadManaging_h
#define KeyPadManaging_h

#include <Arduino.h>

class KeyPad {

  private: 
    byte keypadPin;
    byte *codeKeys;

  public:
    KeyPad(byte pin, byte *codes){ 
      setPin(pin);
      setKeyCode(codes);  
    }

    ~KeyPad(){}

    byte getPin(){ return keypadPin;}
    
    byte* getKeyCode(){ return codeKeys;}

    void setKeyCode(byte *code){ this->codeKeys = code;}
    
    void setPin(byte pin){ this->keypadPin = pin;}

    byte getKeyPadValue();

    byte getNullKeyPadValue();

    byte getResetKeyPadValue();

    byte getLeftKeyPadValue();
 
    byte getRightKeyPadValue();
 
    byte getUpKeyPadValue();

    byte getDownKeyPadValue();
    
    byte getOkKeyPadValue();
};

#endif