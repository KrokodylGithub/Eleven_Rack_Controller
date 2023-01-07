
#define MIDI_CHANNEL 1 /* set MIDI channel for controlled unit */

//***********************************************************
// Pins Definitions. 
// Define Teensy pins that you use for buttons and LED matrix
//***********************************************************  
#define LED_DATA_PIN 17
#define LED_CLK_PIN 15
#define LED_CS_PIN 16

#define DIST_BUTTON_PIN 41
#define FX1_BUTTON_PIN 42
#define FX2_BUTTON_PIN 43
#define DLY_BUTTON_PIN 44
#define REV_BUTTON_PIN 45
#define PRESET_1_BUTTON_PIN 9
#define PRESET_2_BUTTON_PIN 10
#define PRESET_3_BUTTON_PIN 11
#define PRESET_4_BUTTON_PIN 12
#define BANK_UP_BUTTON_PIN 22
#define BANK_DOWN_BUTTON_PIN 23
#define TUNER_BUTTON_PIN 21
#define TAP_BUTTON_PIN 20
// END Pins Definitions



//****************************************************
//Other Definitions 
//****************************************************
#define DIST_BUTTON_PRESSED 1  
#define FX1_BUTTON_PRESSED 2  
#define FX2_BUTTON_PRESSED 3  
#define DLY_BUTTON_PRESSED 4
#define REV_BUTTON_PRESSED 5
#define PRESET_1_BUTTON_PRESSED 6
#define PRESET_2_BUTTON_PRESSED 7
#define PRESET_3_BUTTON_PRESSED 8
#define PRESET_4_BUTTON_PRESSED 9
#define BANK_UP_BUTTON_PRESSED 10
#define BANK_UP_BUTTON_RELEASED 11
#define BANK_DOWN_BUTTON_PRESSED 12
#define BANK_DOWN_BUTTON_RELEASED 13
#define TUNER_BUTTON_PRESSED 14
#define TAP_BUTTON_PRESSED 15
#define TAP_BUTTON_RELEASED 16

#define LONG_PRESS_TIME 1500 /* Change time for long press (ms) */

#define FAST_BANK_CHANGE_UP 1
#define FAST_BANK_CHANGE_DOWN -1
#define FAST_BANK_CHANGE_STOP 0
#define FAST_BANK_CHANGE_DELAY 170 /*  Sets speed of fast banks change (smaller value - faster change) */


/* addresses 0-208 reserved for effects bytes */
#define BANK_BYTE_EPPROM_ADDRESS 209
#define PRESET_BYTE_EPPROM_ADDRESS 210


/* Bits of effects byte */
#define DIST_BIT 0
#define FX1_BIT 1
#define FX2_BIT 2
#define DLY_BIT 3
#define REV_BIT 4
#define TUNER_BIT 5

#define MIDI_TYPE_PC 192
#define MIDI_TYPE_CC 176
#define DIST_CC_MIDI_NUMBER 25
#define FX1_CC_MIDI_NUMBER 63
#define FX2_CC_MIDI_NUMBER 86
#define DLY_CC_MIDI_NUMBER 28
#define REV_CC_MIDI_NUMBER 36
#define TUNER_CC_MIDI_NUMBER 69
#define TAP_CC_MIDI_NUMBER 64 
#define MIDI_ON_VALUE 127
#define MIDI_OFF_VALUE 0

#define HIGHEST_BANK 25
#define LOWEST_BANK 0

#define DEBOUNCE_INTERVAL 20

//END Other Definitions



//******************************************************
// Matrix characters - BANK
// You can change font of characters here
//******************************************************
byte lettersMatrixArray[26][7]={
                    //A
                         {B00000000, 
                          B01111110,
                          B11111110,
                          B10010000,
                          B10010000,
                          B11111110,
                          B01111110},
                    //B
                         {B00000000, 
                          B01101100,
                          B11111110,
                          B10010010,
                          B10010010,
                          B11111110,
                          B11111110},
                    //C
                         {B00000000,
                          B01000100,
                          B11000110,
                          B10000010,
                          B10000010,
                          B11111110,
                          B01111100},
                    //D
                         {B00000000,
                          B01111100,
                          B11111110,
                          B10000010,
                          B10000010,
                          B11111110,
                          B11111110},
                    //E
                         {B00000000, 
                          B10000010,
                          B10000010,
                          B10010010,
                          B10010010,
                          B11111110,
                          B11111110},
                    //F
                         {B00000000, 
                          B10000000,
                          B10000000,
                          B10010000,
                          B10010000,
                          B11111110,
                          B11111110},
                     //G
                         {B00000000,
                          B01011100,
                          B11011110,
                          B10010010,
                          B10000010,
                          B11111110,
                          B01111100},
                     //H
                         {B00000000, 
                          B11111110,
                          B11111110,
                          B00010000,
                          B00010000,
                          B11111110,
                          B11111110},
                     //I
                         {B00000000, 
                          B00000000,
                          B00000000,
                          B11111110,
                          B11111110,
                          B00000000,
                          B00000000},
                     //J
                         {B00000000, 
                          B11111100,
                          B11111110,
                          B00000010,
                          B00000010,
                          B00000110,
                          B00000100},
                     //K
                         {B00000000, 
                          B10000010,
                          B11000110,
                          B01101100,
                          B00111000,
                          B11111110,
                          B11111110},
                     //L
                         {B00000000, 
                          B00000010,
                          B00000010,
                          B00000010,
                          B00000010,
                          B11111110,
                          B11111110},
                     //M
                         {B11111110, 
                          B11111110,
                          B01100000,
                          B00110000,
                          B01100000,
                          B11111110,
                          B11111110},
                      //N
                         {B00000000, 
                          B11111110,
                          B11111110,
                          B00110000,
                          B01100000,
                          B11111110,
                          B11111110},
                      //O
                         {B00000000,
                          B01111100,
                          B11111110,
                          B10000010,
                          B10000010,
                          B11111110,
                          B01111100},
                      //P
                         {B00000000, 
                          B01100000,
                          B11110000,
                          B10010000,
                          B10010000,
                          B11111110,
                          B11111110},
                      //Q
                         {B00000000,
                          B01111010,
                          B11111110,
                          B10000100,
                          B10000010,
                          B11111110,
                          B01111100},
                      //R
                         {B00000000, 
                          B01100000,
                          B11110110,
                          B10011100,
                          B10011000,
                          B11111110,
                          B11111110},
                      //S
                         {B00000000, 
                          B01001100,
                          B11011110,
                          B10010010,
                          B10010010,
                          B11110110,
                          B01100100},
                      //T
                         {B00000000, 
                          B10000000,
                          B10000000,
                          B11111110,
                          B11111110,
                          B10000000,
                          B10000000},
                      //U
                         {B00000000, 
                          B11111100,
                          B11111110,
                          B00000010,
                          B00000010,
                          B11111110,
                          B11111100},
                      //V
                         {B00000000, 
                          B11111000,
                          B11111100,
                          B00000110,
                          B00000110,
                          B11111100,
                          B11111000},
                      //W
                         {B11111100, 
                          B11111110,
                          B00000110,
                          B00011100,
                          B00000110,
                          B11111110,
                          B11111100},
                      //X
                         {B00000000, 
                          B11000110,
                          B11101110,
                          B00111000,
                          B00111000,
                          B11101110,
                          B11000110},
                      //Y
                         {B00000000, 
                          B11100000,
                          B11110000,
                          B00011110,
                          B00011110,
                          B11110000,
                          B11100000},
                      //Z
                         {B00000000, 
                          B11000010,
                          B11100010,
                          B10110010,
                          B10011010,
                          B10001110,
                          B10000110}
};
//END Matrix characters - BANK



//******************************************************
// Matrix characters - PRESET
// You can change font of characters here
//******************************************************
byte numbersMatrixArray[4][7]={
                     //1
                         {B00000000, 
                          B00000000,
                          B00000000,
                          B11111110,
                          B11111110,
                          B01000000,
                          B00000000},
                      //2
                         {B00000000,
                          B01100010,
                          B11110010,
                          B10010010,
                          B10010010,
                          B10011110,
                          B01001110},
                      //3
                         {B00000000,
                          B01101100,
                          B11111110,
                          B10010010,
                          B10010010,
                          B10010010,
                          B10010010},
                      //4
                         {B00000000, 
                          B11111110,
                          B11111110,
                          B00001000,
                          B00001000,
                          B11111000,
                          B11111000}
                   
};
//END Matrix characters - PRESET
