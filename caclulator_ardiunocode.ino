/*
  File: my_code.ino
  Author: Bhathiya Induwara
  Description: Arduino Calculator using I2C LCD and 4x4 Keypad
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;

// Keypad layout
// A = +
// B = -
// C = *
// D = /
// # = =
// * = Clear

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Keypad pin connections
byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {A3, A2, A1, A0};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// Variables
String firstNumber = "";
String secondNumber = "";
char operation = 0;
bool enteringSecond = false;

// Splash Screen
void showSplashScreen() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Bhathiya's");

  lcd.setCursor(1, 1);
  lcd.print("Calculator");

  delay(3000);

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Start Calc");

  lcd.setCursor(0, 1);
}

// Calculate result
double calculate(double a, double b, char op) {

  switch(op) {

    case 'A':
      return a + b;

    case 'B':
      return a - b;

    case 'C':
      return a * b;

    case 'D':

      if(b != 0)
        return a / b;

      else
        return 0;
  }

  return 0;
}

// Reset calculator
void resetCalculator() {

  firstNumber = "";
  secondNumber = "";
  operation = 0;
  enteringSecond = false;

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Start Calc");

  lcd.setCursor(0, 1);
}

// Setup
void setup() {

  Serial.begin(9600);

  lcd.init();
  lcd.backlight();

  showSplashScreen();
}

// Main Loop
void loop() {

  char key = keypad.getKey();

  if(key) {

    // CLEAR button (*)
    if(key == '*') {

      resetCalculator();
      return;
    }

    // Number buttons
    if(key >= '0' && key <= '9') {

      lcd.print(key);

      if(!enteringSecond) {
        firstNumber += key;
      }
      else {
        secondNumber += key;
      }
    }

    // Operation buttons
    else if(key == 'A' || key == 'B' || key == 'C' || key == 'D') {

      operation = key;
      enteringSecond = true;

      lcd.setCursor(0, 1);

      switch(key) {

        case 'A':
          lcd.print("+");
          break;

        case 'B':
          lcd.print("-");
          break;

        case 'C':
          lcd.print("*");
          break;

        case 'D':
          lcd.print("/");
          break;
      }
    }

    // Equals button (#)
    else if(key == '#') {

      double num1 = firstNumber.toDouble();
      double num2 = secondNumber.toDouble();

      double result = calculate(num1, num2, operation);

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Result:");

      lcd.setCursor(0, 1);
      lcd.print(result);

      // Send result to Serial Monitor / Java GUI
      Serial.println(result);

      delay(3000);

      // Auto reset
      resetCalculator();
    }
  }
}