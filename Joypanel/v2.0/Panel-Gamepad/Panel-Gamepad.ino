#include <Keypad.h>
#include <Joystick.h>

//=====================
//= Set Button keypad =
//=====================
const byte ROWS = 3;
const byte COLS = 6;
char keys[ROWS][COLS] = {
    {0, 1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10, 11},
    {12, 13, 14, 15, 16, 17}};
byte rowPins[ROWS] = {10, 16, 14};
byte colPins[COLS] = {4, 5, 6, 7, 8, 9};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

//=====================
//=== Set Joystick ===
//=====================

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   20, 0,                // Button Count, Hat Switch Count
                   true, true, false,    // X Y Z
                   false, false, false,  // RX RY RZ
                   false, false,         // Rudder throttle
                   false, false, false); // accelerator brake steering

int state;

void bootAnimation()
{
  int keys[] = {A3, A2, A1};
  int i;

  for (i = 0; i < 3; i++)
  {
    digitalWrite(keys[i], HIGH);
    delay(200);
  }

  delay(50);
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);

  for (i = 3; i > 0; i--)
  {
    digitalWrite(keys[i], HIGH);
    delay(200);
  }

  for (i = 0; i < 2; i++)
  {
    digitalWrite(A1, HIGH);
    digitalWrite(A2, HIGH);
    digitalWrite(A3, HIGH);

    delay(250);
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(A3, LOW);
    delay(250);
  }

  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
  digitalWrite(A3, HIGH);
}

void setup()
{
  Joystick.begin();
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  Serial.begin(9600);

  bootAnimation();
}

void loop()
{

  if (kpd.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++) // Scan the whole key list.
    {
      if (kpd.key[i].stateChanged) // Only find keys that have changed state.
      {
        switch (kpd.key[i].kstate)
        { // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
        case PRESSED:
          state = 1;
          break;
          ;
        case RELEASED:
          state = 0;
        }

        int u = kpd.key[i].kchar;
        Serial.print("Key: ");
        Serial.print(u);
        Serial.print(" State: ");
        Serial.println(state);
        Joystick.setButton(u, state);
      }
    }
  }
}
