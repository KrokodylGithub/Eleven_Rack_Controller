#include "LedControl.h"
#include "MIDIControlerDefinitions.h"
#include <MIDI.h>
#include <Bounce2.h>
#include <EEPROM.h>


bool distState,fx1State, fx2State, dlyState, revState, tunerState = false;
int bank, preset =0;

bool bankUpButtonPressed_FLAG = false;
bool bankDownButtonPressed_FLAG = false;
bool bankUpButtonReleased_FLAG = false;
bool bankDownButtonReleased_FLAG = false;
bool tapButtonPressed_FLAG = false;

unsigned long bankUpButtonPressedTime = 0;
unsigned long bankDownButtonPressedTime = 0;
unsigned long tapButtonPresseTime = 0;
unsigned long fastBankChangeTimeMark = 0;

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);


/********************************************************************
*  Display and LEDs are made with 8x8 LED Matrix modules (MAX7219) 
*  Order of modules chain is PRESET > BANK > LEDs
********************************************************************/
LedControl lc = LedControl(LED_DATA_PIN, LED_CLK_PIN, LED_CS_PIN, 3);
Bounce debouncedDistButton,
       debouncedFx1Button,
       debouncedFx2Button,
       debouncedDlyButton,
       debouncedRevButton,
       debouncedPrst1Button,
       debouncedPrst2Button,
       debouncedPrst3Button,
       debouncedPrst4Button,
       debouncedBankUpButton,
       debouncedBankDownButton,
       debouncedTunerButton,
       debouncedTapButton = Bounce(); // Instantiate a Bounce objects


//********
// SETUP
//********     
void setup() {
  Serial.begin(57600);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  
  // buttons pins setup
  debouncedPrst1Button.attach(PRESET_1_BUTTON_PIN,INPUT_PULLUP);
  debouncedPrst2Button.attach(PRESET_2_BUTTON_PIN,INPUT_PULLUP);
  debouncedPrst3Button.attach(PRESET_3_BUTTON_PIN,INPUT_PULLUP);
  debouncedPrst4Button.attach(PRESET_4_BUTTON_PIN,INPUT_PULLUP);
  debouncedDistButton.attach(DIST_BUTTON_PIN,INPUT_PULLUP);
  debouncedFx1Button.attach(FX1_BUTTON_PIN,INPUT_PULLUP);
  debouncedFx2Button.attach(FX2_BUTTON_PIN,INPUT_PULLUP);
  debouncedDlyButton.attach(DLY_BUTTON_PIN,INPUT_PULLUP);
  debouncedRevButton.attach(REV_BUTTON_PIN,INPUT_PULLUP);
  debouncedBankUpButton.attach(BANK_UP_BUTTON_PIN,INPUT_PULLUP);
  debouncedBankDownButton.attach(BANK_DOWN_BUTTON_PIN,INPUT_PULLUP);
  debouncedTunerButton.attach(TUNER_BUTTON_PIN,INPUT_PULLUP);
  debouncedTapButton.attach(TAP_BUTTON_PIN,INPUT_PULLUP);

  /* Set Display and LEDs parameters */
  lc.setIntensity(0,4); /* PRESET character brightness */
  lc.setIntensity(1,4); /* BANK character brightness  */
  lc.setIntensity(2,1); /* LEDs brightness */
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.clearDisplay(2);

   //Wake up MAX72XX
  lc.shutdown(0, false);
  lc.shutdown(1, false);
  lc.shutdown(2, false);

  loadLastRig();
  loadEffectsStates(); 

  updateDisplay();
  updateLED();
} // END setup()



//***************************************************************************
//loadLastRig() loads rig (bank and preset)used before turning off controller
//***************************************************************************
void loadLastRig(){
  /* read EEPROM */
  bank = (int)EEPROM.read(BANK_BYTE_EPPROM_ADDRESS);
  preset = (int)EEPROM.read(PRESET_BYTE_EPPROM_ADDRESS);
} //END loadLastRig()



//***************************************************************
// loadEffectsStates() loads effects states saved for current rig
//***************************************************************
void loadEffectsStates(){
  /*  Effects states are stored in EEPROM, one byte for one rig effects (adresses 0-208) */
  int effectsByteAddress = 4*bank+preset;
  byte effectsByte = EEPROM.read(effectsByteAddress);
  
  distState = (bool)bitRead(effectsByte,DIST_BIT);
  fx1State = (bool)bitRead(effectsByte,FX1_BIT);
  fx2State = (bool)bitRead(effectsByte,FX2_BIT);
  dlyState = (bool)bitRead(effectsByte,DLY_BIT);
  revState = (bool)bitRead(effectsByte,REV_BIT);
}// END loadEffectsStates()



//*************************************************
// saveEffectsStates() saves current effects states
//*************************************************
void saveEffectsStates(){
  /*  Effects states are stored in EEPROM, one byte for one rig effects (adresses 0-208) */
   byte effectsByte = 0;
   bitWrite(effectsByte, DIST_BIT, (int)distState);
   bitWrite(effectsByte, FX1_BIT, (int)fx1State);
   bitWrite(effectsByte, FX2_BIT, (int)fx2State);
   bitWrite(effectsByte, DLY_BIT, (int)dlyState);
   bitWrite(effectsByte, REV_BIT, (int)revState);

   int effectsByteAddress = 4*bank+preset;
   EEPROM.write(effectsByteAddress, effectsByte);
}//END saveEffectsStates()



//*********************************
// saveRigBytes() saves current rig
//*********************************
void saveRigBytes(bool bankUpdate, bool presetUpdate){
  /* Bank and Preset are stored in EEPROM */
  if(bankUpdate){EEPROM.write(BANK_BYTE_EPPROM_ADDRESS, bank);}
  if(presetUpdate){EEPROM.write(PRESET_BYTE_EPPROM_ADDRESS, preset);}
} // END saveRigBytes()


//************************************************************************
// updateDisplay() sets bank character and preset character on LED modules
//************************************************************************
void updateDisplay() {
  for (int i = 0; i < 7; i++) {
      lc.setRow(0, i, numbersMatrixArray[preset][i]);
      lc.setRow(1, i, lettersMatrixArray[bank][i]);
  }
} //END updateDisplay()



//******************************************************************
// readButtons() checks if any button is pressed and then returns 1
//****************************************************************** 
int readButtons(){
   debouncedPrst1Button.update();
   debouncedPrst2Button.update();
   debouncedPrst3Button.update();
   debouncedPrst4Button.update();
   debouncedDistButton.update();
   debouncedFx1Button.update();
   debouncedFx2Button.update();
   debouncedDlyButton.update();
   debouncedRevButton.update();
   debouncedBankUpButton.update();
   debouncedBankDownButton.update();
   debouncedTunerButton.update();
   debouncedTapButton.update();

   if (debouncedPrst1Button.changed() ){
      if (debouncedPrst1Button.read() == LOW){return PRESET_1_BUTTON_PRESSED;}
   }
   if (debouncedPrst2Button.changed() ){
      if (debouncedPrst2Button.read() == LOW){return PRESET_2_BUTTON_PRESSED;}
   }
   if (debouncedPrst3Button.changed() ){
      if (debouncedPrst3Button.read() == LOW){return PRESET_3_BUTTON_PRESSED;}
   }
   if (debouncedPrst4Button.changed() ){
      if (debouncedPrst4Button.read() == LOW){return PRESET_4_BUTTON_PRESSED;}
   }
   if (debouncedDistButton.changed() ){
      if (debouncedDistButton.read() == LOW){return DIST_BUTTON_PRESSED;}
   }
   if (debouncedFx1Button.changed() ){
      if (debouncedFx1Button.read() == LOW){ return FX1_BUTTON_PRESSED;}
   }
   if (debouncedFx2Button.changed() ){
      if (debouncedFx2Button.read() == LOW){return FX2_BUTTON_PRESSED;}
   }
   if (debouncedDlyButton.changed() ){
      if (debouncedDlyButton.read() == LOW){return DLY_BUTTON_PRESSED;}
   }
   if (debouncedRevButton.changed() ){
      if (debouncedRevButton.read() == LOW){return REV_BUTTON_PRESSED;}
   }
   if (debouncedBankUpButton.changed() ){
      if (debouncedBankUpButton.read() == LOW){return BANK_UP_BUTTON_PRESSED;}
      if (debouncedBankUpButton.read() == HIGH){return BANK_UP_BUTTON_RELEASED;}
   }
   if (debouncedBankDownButton.changed() ){
      if (debouncedBankDownButton.read() == LOW){return BANK_DOWN_BUTTON_PRESSED;}
      if (debouncedBankDownButton.read() == HIGH){return BANK_DOWN_BUTTON_RELEASED;}
   }
   if (debouncedTunerButton.changed() ){
      if (debouncedTunerButton.read() == LOW){return TUNER_BUTTON_PRESSED;}
   }
   if (debouncedTapButton.changed() ){
      if (debouncedTapButton.read() == LOW){return TAP_BUTTON_PRESSED;}
      if (debouncedTapButton.read() == HIGH){return TAP_BUTTON_RELEASED;}
   }
   return 0;
}// END readButtons()



//****************************************************
// reactForButton() triggers action for pressed button
//****************************************************
void reactForButton(int buttonChanged){
  int CCval;
  
  switch (buttonChanged){
    case 0:
    break;

    case PRESET_1_BUTTON_PRESSED:
    preset = 0;
    sendMIDImessage();
    loadEffectsStates();
    saveRigBytes(false,true);
    updateDisplay();
    updateLED();
    break;

    case PRESET_2_BUTTON_PRESSED:
    preset = 1;
    sendMIDImessage();
    loadEffectsStates();
    saveRigBytes(false,true); 
    updateDisplay();
    updateLED();
    break;

    case PRESET_3_BUTTON_PRESSED:
    preset = 2;
    sendMIDImessage();
    loadEffectsStates();
    saveRigBytes(false,true); 
    updateDisplay();
    updateLED();
    break;

    case PRESET_4_BUTTON_PRESSED:
    preset = 3;
    sendMIDImessage();
    loadEffectsStates();
    saveRigBytes(false,true); 
    updateDisplay();
    updateLED();
    break;
    
    case DIST_BUTTON_PRESSED:
    distState = !distState;
    if(distState){CCval = MIDI_ON_VALUE;}else{CCval = MIDI_OFF_VALUE;}
    sendMIDImessage(DIST_CC_MIDI_NUMBER, CCval);
    updateLED();
    break;

    case FX1_BUTTON_PRESSED:
    fx1State = !fx1State;
    if(fx1State){CCval = MIDI_ON_VALUE;}else{CCval = MIDI_OFF_VALUE;}
    sendMIDImessage(FX1_CC_MIDI_NUMBER, CCval);
    updateLED();
    break;

    case FX2_BUTTON_PRESSED:
    fx2State = !fx2State;
    if(fx2State){CCval = MIDI_ON_VALUE;}else{CCval = MIDI_OFF_VALUE;}
    sendMIDImessage(FX2_CC_MIDI_NUMBER, CCval);
    updateLED();
    break;
    
    case DLY_BUTTON_PRESSED:
    dlyState = !dlyState;
    if(dlyState){CCval = MIDI_ON_VALUE;}else{CCval = MIDI_OFF_VALUE;}
    sendMIDImessage(DLY_CC_MIDI_NUMBER, CCval);
    updateLED();
    break;

    case REV_BUTTON_PRESSED:
    revState = !revState;
    if(revState){CCval = MIDI_ON_VALUE;}else{CCval = MIDI_OFF_VALUE;}
    sendMIDImessage(REV_CC_MIDI_NUMBER, CCval);
    updateLED();
    break;

    case BANK_UP_BUTTON_PRESSED:
    bank++;
    if(bank>HIGHEST_BANK)bank=LOWEST_BANK;
    sendMIDImessage();
    loadEffectsStates();
    saveRigBytes(true,false);
    updateDisplay();
    updateLED();
    bankUpButtonPressed_FLAG = true;
    bankUpButtonPressedTime = millis();
    break;

    case BANK_UP_BUTTON_RELEASED:
    bankUpButtonReleased_FLAG = true;
    break;

    case BANK_DOWN_BUTTON_PRESSED:
    bank--;
    if(bank<LOWEST_BANK)bank=HIGHEST_BANK;
    sendMIDImessage();
    loadEffectsStates();
    saveRigBytes(true,false);
    updateDisplay();
    updateLED();
    bankDownButtonPressed_FLAG = true;
    bankDownButtonPressedTime = millis();
    break;

    case BANK_DOWN_BUTTON_RELEASED:
    bankDownButtonReleased_FLAG = true;
    break;

    case TUNER_BUTTON_PRESSED:
    tunerState = !tunerState;
    if(tunerState){CCval = MIDI_ON_VALUE;}else{CCval = MIDI_OFF_VALUE;}
    sendMIDImessage(TUNER_CC_MIDI_NUMBER, CCval);
    updateLED();
    break;

    case TAP_BUTTON_PRESSED:
    sendMIDImessage(TAP_CC_MIDI_NUMBER,MIDI_OFF_VALUE);
    tapButtonPressed_FLAG = true;
    tapButtonPresseTime = millis();
    break;

    case TAP_BUTTON_RELEASED:
    tapButtonPressed_FLAG = false;
    break;
  }
}// END reactForButton()



//******************************* 
//updateLED() controls LED states 
//*******************************
void updateLED(){
  /* LEDs states are stored in two bytes array
     first byte: Dist LED, Fx1 LED, Fx2 LED, Dly LED, Rev LED
     second Byte Prst 1 LED, Prst 2 LED, Prst 3 LED, Prst 4 LED 
  */
  byte LEDsArray [2] = {B00000000, B00000000};  

  /*write first byte*/
  bitWrite(LEDsArray[0], DIST_BIT, (int)distState);
  bitWrite(LEDsArray[0], FX1_BIT, (int)fx1State);
  bitWrite(LEDsArray[0], FX2_BIT, (int)fx2State);
  bitWrite(LEDsArray[0], DLY_BIT, (int)dlyState);
  bitWrite(LEDsArray[0], REV_BIT, (int)revState);
  bitWrite(LEDsArray[0], TUNER_BIT, (int)tunerState);

  /* write second byte */
  for(int i=0; i<4; i++){    
    bitWrite(LEDsArray[1], i, (int)(i==preset)); /* (i==preset) sets 1 for current preset bit */
  }
  
  /* set LEDs on or off */
  for(int i=0;i<2;i++){
    lc.setRow(2, i, LEDsArray[i]);
  }
}// END updateLED()



//**************************************
// animateFlash() makes display flashing 
//**************************************
void animateFlash(){
  int dly = 30;
  
    for(int i=0; i<3; i++){
      for(int i=0; i<8; i++){
      lc.setRow(0, i, B11111111);
      lc.setRow(1, i, B00000000);
      }
    delay(dly);
    for(int i=0; i<8; i++){
      lc.setRow(0, i, B00000000);
      lc.setRow(1, i, B11111111);
    }
    delay(dly);
  } 
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  
  updateDisplay();
  updateLED();
}// END animateFlash()



//*******************************************************
// checkLongPress() triggers action for long press button
//*******************************************************
void checkLongPress(){
  unsigned long timeCheck = millis();
  bool longPressDone = false;

  /* for TAP button */
  if(tapButtonPressed_FLAG){
    if(timeCheck - tapButtonPresseTime > LONG_PRESS_TIME){
    tapButtonPressed_FLAG = false;
    saveState();
    }
  }

  /* for BANK UP button */
  if(bankUpButtonPressed_FLAG){
    if(timeCheck - bankUpButtonPressedTime > LONG_PRESS_TIME){
      longPressDone = true;
      fastBankChange(FAST_BANK_CHANGE_UP);
    }
    if(bankUpButtonReleased_FLAG){
      if(longPressDone){
        fastBankChange(FAST_BANK_CHANGE_STOP);
      }
       bankUpButtonPressed_FLAG = false;
       bankUpButtonReleased_FLAG = false;
    }
   }

   /* for BANK DOWN button */
   if(bankDownButtonPressed_FLAG){
    if(timeCheck - bankDownButtonPressedTime > LONG_PRESS_TIME){
      longPressDone = true;
      fastBankChange(FAST_BANK_CHANGE_DOWN);
    }
    if(bankDownButtonReleased_FLAG){
      if(longPressDone){
        fastBankChange(FAST_BANK_CHANGE_STOP);
      }
       bankDownButtonPressed_FLAG = false;
       bankDownButtonReleased_FLAG = false;
    }
   }
}//END checkLongPress()



//********************************************
// fastBankChange() scrolling banks up or down
//********************************************
void fastBankChange(int changeDirection){
  unsigned long dly = FAST_BANK_CHANGE_DELAY;

  switch (changeDirection){
    case FAST_BANK_CHANGE_UP:
      if(millis() - fastBankChangeTimeMark >= dly){
       fastBankChangeTimeMark = millis();
       bank++;
       if(bank>HIGHEST_BANK)bank=LOWEST_BANK;
       updateDisplay();
    }
    break;
    
    case FAST_BANK_CHANGE_DOWN:
    if(millis() - fastBankChangeTimeMark >= dly){
       fastBankChangeTimeMark = millis();
       bank--;
       if(bank<LOWEST_BANK)bank=HIGHEST_BANK;
       updateDisplay();
    }
    break;
    
    case FAST_BANK_CHANGE_STOP:
    sendMIDImessage();
    loadEffectsStates();
    updateDisplay();
    updateLED();
    break;
    
  }
}// END fastBankChange()


//*************************************************************
// saveState() triggers actions during saving of effects states
//*************************************************************
void saveState(){
  saveEffectsStates();
  animateFlash();
}// END saveState()



//**********************************************************
// sendMIDImessage() sends MIDI messages to controlled unit
//**********************************************************
void sendMIDImessage(int ccMidiNumber, int ccMidiValue){
  MIDI. sendControlChange(ccMidiNumber, ccMidiValue, MIDI_CHANNEL);
}
void sendMIDImessage(){
  int rig = 4*bank+preset;
  MIDI. sendProgramChange(rig, MIDI_CHANNEL);
}// END sendMIDImessage()



//********************************************************************************
// copyDeviceParams() reads controlled unit parameters and copy them to controller
//********************************************************************************
void copyDeviceParams(){
  byte type = MIDI.getType();
  int data1 = MIDI.getData1();
  int data2 = MIDI.getData2();

/* get Bank and Preset when Program Change message is sent */
  if (type == MIDI_TYPE_PC){
    bank = (int) data1 / 4; /* decodes Bank from sent message */
    preset = (int) data1 % 4; /* decodes Preset from sent message */
    saveRigBytes(true,true); 
    loadEffectsStates();
    updateDisplay();
    updateLED();
  }
  /* get effect change when Control Chane is sent */
  if (type == MIDI_TYPE_CC){
    switch (data1){
      case DIST_CC_MIDI_NUMBER:
      if(data2 < 63){distState = 0;}else{distState = 1;}
      updateLED();
      break;

      case FX1_CC_MIDI_NUMBER:
      if(data2 < 63){fx1State = 0;}else{fx1State = 1;}
      updateLED();
      break;

      case FX2_CC_MIDI_NUMBER:
      if(data2 < 63){fx2State = 0;}else{fx2State = 1;}
      updateLED();
      break;

      case DLY_CC_MIDI_NUMBER:
      if(data2 < 63){dlyState = 0;}else{dlyState = 1;}
      updateLED();
      break;

      case REV_CC_MIDI_NUMBER:
      if(data2 < 63){revState = 0;}else{revState = 1;}
      updateLED();
      break;
    }
  }
}// END sendMIDImessage()



// Main loop
void loop() {
  if (MIDI.read()) copyDeviceParams();
  reactForButton(readButtons());
  if(tapButtonPressed_FLAG | bankUpButtonPressed_FLAG | bankDownButtonPressed_FLAG){
    checkLongPress();
  }
}// END Main loop
