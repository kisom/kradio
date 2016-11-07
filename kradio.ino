// kradio is a Red Board-based RF TXR operating on the 2m, 1.25m, and 70cm
// bands.
//
// The LCD setup code is taken from the LCD shield wiki:
// http://linksprite.com/wiki/index.php5?title=16_X_2_LCD_Keypad_Shield_for_Arduino_V2.
#define Rbase ((unsigned long)47)
#define Rup ((unsigned long)75)
#define Rdown  ((unsigned long)62)
#define Rleft  ((unsigned long)47)
#define Rright ((unsigned long)39)
#define Rselect  ((unsigned long)28)
#define PWM_PIN 3
#define RESET_PIN A3
#define SWITCH_PIN 2

#include <HamShield.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// initialise the radio
HamShield radio;

int sensorValue ;
int KeyTable[31];
int freq[6] = {4, 4, 6, 0, 0, 0};

uint32_t radioFreq() {
  uint32_t  f = 0;
  for (int i = 0; i < 6; i++) {
    f *= 10;
    f += freq[i];
  }
  return f;
}

#define VERSION "KRadio v1.0.0"

void generate_key_table(int vcc, int* array)
{
  float resistor;

  //////////////1key//////////////////////
  resistor = ((float)Rup) / (Rbase + Rup);
  *array++ = resistor * vcc;

  resistor = ((float)Rdown) / (Rbase + Rdown);
  *array++ = resistor * vcc;

  resistor = ((float)Rleft) / (Rbase + Rleft);
  *array++ = resistor * vcc;

  resistor = ((float)Rright) / (Rbase + Rright);
  *array++ = resistor * vcc;

  resistor = ((float)Rselect) / (Rbase + Rselect);
  *array++ = resistor * vcc;

  //////////////2 key/////////////////////////
  resistor = ((float)Rup) * Rdown / (Rup + Rdown);
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rup) * Rright / (Rup + Rright);
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rup) * Rleft / (Rup + Rleft);
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rup) * Rselect / (Rup + Rselect);
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rdown) * Rleft / (Rdown + Rleft);
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rdown) * Rright / (Rdown + Rright);
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rdown) * Rselect / (Rdown + Rselect);
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rright) * Rleft / (Rright + Rleft);
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rright) * Rselect / (Rright + Rselect);
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rleft) * Rselect / (Rleft + Rselect);
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  ///////////////3 key//////////////////////
  resistor = ((float)Rup * Rdown * Rright / (Rup * Rright + Rdown * Rright + Rup * Rdown));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rup * Rdown * Rleft / (Rup * Rleft + Rdown * Rleft + Rup * Rdown));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rup * Rdown * Rselect / (Rup * Rselect + Rdown * Rselect + Rup * Rdown));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rleft * Rdown * Rright / (Rleft * Rright + Rdown * Rright + Rleft * Rdown));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rleft * Rdown * Rselect / (Rleft * Rselect + Rdown * Rselect + Rleft * Rdown));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rleft * Rup * Rright / (Rleft * Rright + Rup * Rright + Rleft * Rup));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rleft * Rup * Rselect / (Rleft * Rselect + Rup * Rselect + Rleft * Rup));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rup * Rright * Rselect / (Rup * Rright + Rright * Rselect + Rup * Rselect));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rdown * Rright * Rselect / (Rdown * Rright + Rright * Rselect + Rdown * Rselect));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rleft * Rright * Rselect / (Rleft * Rright + Rright * Rselect + Rleft * Rselect));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  ////////////////4 key///////////////////////////
  resistor = ((float)Rup * Rdown * Rleft * Rright / (Rdown * Rleft * Rright + Rup * Rleft * Rright + Rup * Rdown * Rright + Rup * Rdown * Rleft));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rup * Rdown * Rleft * Rselect / (Rdown * Rleft * Rselect + Rup * Rleft * Rselect + Rup * Rdown * Rselect + Rup * Rdown * Rleft));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rup * Rselect * Rleft * Rright / (Rselect * Rleft * Rright + Rup * Rleft * Rright + Rup * Rselect * Rright + Rup * Rselect * Rleft));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rselect * Rdown * Rleft * Rright / (Rdown * Rleft * Rright + Rselect * Rleft * Rright + Rselect * Rdown * Rright + Rselect * Rdown * Rleft));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  resistor = ((float)Rselect * Rdown * Rup * Rright / (Rdown * Rup * Rright + Rselect * Rup * Rright + Rselect * Rdown * Rright + Rselect * Rdown * Rup));
  resistor = resistor / (resistor + Rbase);
  *array++ = resistor * vcc;

  /////////////////5 key//////////////////////////
  resistor = ((float)Rup * Rdown * Rleft * Rright * Rselect / (Rdown * Rleft * Rright * Rselect + Rup * Rleft * Rright * Rselect + Rup * Rdown * Rright * Rselect + Rup * Rdown * Rleft * Rselect + Rup * Rdown * Rleft * Rright));
  resistor = resistor / (resistor + Rbase);
  *array = resistor * vcc;
}

unsigned char GetKey(int value)
{
  char tmpChar;
  unsigned int Rst;

  tmpChar = 0;
  do {
    if (value > KeyTable[tmpChar]) Rst = value - KeyTable[tmpChar];
    else  Rst = KeyTable[tmpChar] - value;
    tmpChar ++;
  } while (Rst > 1);

  return tmpChar--;

}

void show_frequency() {
  lcd.clear();
  lcd.print("Tuner:");
  lcd.setCursor(0, 1);
  for (int i = 0; i < 6; i++) {
    lcd.print(freq[i]);
  }
}

int wrapFreq(int n) {
  if (n < 0) {
    n = 9;
  } else if (n > 9) {
    n = 0;
  }

  return n;
}

void set_frequency() {
  int col = 0;
  uint8_t cont = 1;

  sensorValue = analogRead(A0);

  Serial.println("setting frequency");

  lcd.clear();
  lcd.print("Frequency:");
  lcd.setCursor(0, 1);
  for (int i = 0; i < 6; i++) {
    lcd.print(freq[i]);
  }

  Serial.print("analog value: ");
  Serial.println(sensorValue);

  while (cont) {
    if (sensorValue != analogRead(A0)) {
      sensorValue = analogRead(A0);
      Serial.println("button pressed");
      Serial.print("analog value: ");
      Serial.println(sensorValue);      
      unsigned char key = GetKey(sensorValue);
      switch (key) {
        case Rdown:
          Serial.println("down");
          freq[col] = wrapFreq(freq[col] - 1);
          lcd.setCursor(col, 1);
          lcd.print(freq[col]);
          break;
        case Rup:
          Serial.println("up");
          freq[col] = wrapFreq(freq[col] - 1);
          lcd.setCursor(col, 1);
          lcd.print(freq[col]);
          break;
        case Rleft:
          Serial.println("left");
          col--;
          if (col < 0) {
            col = 5;
          } else if (col > 5) {
            col = 0;
          }
          break;
        case Rright:
          Serial.println("right");
          col++;
          if (col < 0) {
            col = 5;
          } else if (col > 5) {
            col = 0;
          }
          break;
        case Rselect:
          Serial.println("select");
          cont = 0;
          radio.frequency(radioFreq());
          break;
        default:
          Serial.println("interface error: unexpected key");
          exit(1);
      }
    }
  }
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print(VERSION);
  sensorValue  = 1023;
  Serial.begin(9600);

  delay(2);

  // NOTE: if not using PWM out, it should be held low to avoid tx noise
  pinMode(PWM_PIN, OUTPUT);
  digitalWrite(PWM_PIN, LOW);

  // prep the switch
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  // set up the reset control pin
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);

  // Tell the HamShield to start up
  radio.initialize();
  radio.setRfPower(0);
  radio.frequency(radioFreq());

  lcd.clear();
  lcd.print("Radio ready.");
  delay(1);

  show_frequency();
  // set_frequency();
}

void loop() {
}
