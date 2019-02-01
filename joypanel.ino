#include <Keypad.h>
#include <Joystick.h>

//=====================
//= Set Button keypad =
//=====================
const byte ROWS = 3;
const byte COLS = 5;
char keys[ROWS][COLS] = {
  {'0','1','2','3','4'},
  {'5','6','7','8','9'},
  {'A','B','C','D','E'}
};
byte rowPins[ROWS] = {7, 8, 9};
byte colPins[COLS] = {2, 3, 4, 5, 6};

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//=====================
//=== Set Joystick ===
//=====================

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  18, 0,                 // Button Count, Hat Switch Count
  true, true, false,  // X Y Z
  false, false, false,  // RX RY RZ
  false, false,          // Rudder throttle
  false, false, false); // accelerator brake steering
  
int state;
void setup() {
    Joystick.begin();
    pinMode(14, INPUT_PULLUP);
    pinMode(15, INPUT_PULLUP);
    pinMode(16, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(14)) Joystick.setButton(15, 0);
  if (digitalRead(15)) Joystick.setButton(16, 0);
  if (digitalRead(16)) Joystick.setButton(17, 0);
  if (!digitalRead(14)) Joystick.setButton(15, 1);
  if (!digitalRead(15)) Joystick.setButton(16, 1);
  if (!digitalRead(16)) Joystick.setButton(17, 1);
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if (kpd.key[i].stateChanged)   // Only find keys that have changed state.
            {
                switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                    state = 1;
                break;;
                    case RELEASED:
                    state = 0;
                }
                int u = kpd.key[i].kchar;
                if (u > 64) u = u - 55;
                else u = u - 48;
                Joystick.setButton(u, state);
            }
        }
    }
}  // End loop
