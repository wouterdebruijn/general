#include <Keypad.h>
#include <Keyboard.h>

//=====================
//= Set Button keypad =
//=====================
const byte ROWS = 3;
const byte COLS = 6;

// Set an value for each key.
char keys[ROWS][COLS] = {
    {0, 1, 2, 3, 4, 5},
    {6, 7, 8, 9, 10, 11},
    {12, 13, 14, 15, 16, 17}};
byte rowPins[ROWS] = {10, 16, 14};
byte colPins[COLS] = {4, 5, 6, 7, 8, 9};

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

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
  Keyboard.begin();
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);

  bootAnimation();
}

// Press the keyboard key on an button press. First two rows use F13-F24, last row adds the Left-Ctrl key.
// Full list is available at: https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardmodifiers/
void loop()
{
  if (kpd.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++)
    { // Go through all keys
      if (kpd.key[i].stateChanged)
      { // If the key has a new state
        int u = kpd.key[i].kchar;
        // Press a key
        if (kpd.key[i].kstate == PRESSED)
        {
          if (u < 12)
          {
            /// First two rows will press F13-F24
            Keyboard.press(u + 240);
          }
          else
          {
            // Last row adds the Ctrl key
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press((u - 12) + 240); // Remove 12 so we are between 0 and 5, add 240 so we are in the F13-F24 range.
          }
        }
        // Release all keys on button release
        if (kpd.key[i].kstate == RELEASED)
        {
          Keyboard.releaseAll();
        }
      }
    }
  }
}
