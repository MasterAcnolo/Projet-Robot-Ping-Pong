#include <LiquidCrystal_I2C.h>

#define BTN_POWER 10
#define BTN_BACK 11
#define BTN_VALIDATE 12
#define POT_SELECT A1

// États principaux
enum Mode { DROITIER, GAUCHER };
enum Train { REVERS, COUPS_DROIT, SMASH, GENERAL };
enum Diff { DEBUTANT, FACILE, NORMAL, DIFFICILE, EXTREME };
enum State {
  CHOIX_MODE,
  CHOIX_ENTRAINEMENT,
  CHOIX_DIFFICULTE,
  CONFIRMATION,
  EN_TRAINEMENT,
  PAUSE,
  QUIT_CONFIRM
};

// Variables d'état
State state = CHOIX_MODE;
Mode currentMode = DROITIER;
Train currentTrain = REVERS;
Diff currentDiff = NORMAL;

// Sauvegarde des états pour éviter le clignotement
State lastDisplayedState = static_cast<State>(-1);
Mode lastMode = static_cast<Mode>(-1);
Train lastTrain = static_cast<Train>(-1);
Diff lastDiff = static_cast<Diff>(-1);

// Suivi des boutons
bool BTN_BACKPrevState = HIGH;
bool BTN_VALIDATEPrevState = HIGH;

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pinMode(BTN_BACK, INPUT_PULLUP);
  pinMode(BTN_VALIDATE, INPUT_PULLUP);
  displayState();
}

// Anti-rebond
bool isButtonPressed(int pin, bool &prevState) {
  bool currentState = digitalRead(pin);
  if (currentState == LOW && prevState == HIGH) {
    delay(50);
    prevState = currentState;
    return true;
  }
  prevState = currentState;
  return false;
}

void displayState() {
  lcd.clear();
  switch (state) {
    case CHOIX_MODE:
      lcd.setCursor(0, 0);
      lcd.print("Mode:");
      lcd.setCursor(0, 1);
      lcd.print(currentMode == DROITIER ? "Droitier" : "Gaucher");
      break;

    case CHOIX_ENTRAINEMENT:
      lcd.setCursor(0, 0);
      lcd.print("Entrainement:");
      lcd.setCursor(0, 1);
      switch (currentTrain) {
        case REVERS: lcd.print("Revers"); break;
        case COUPS_DROIT: lcd.print("Coup droit"); break;
        case SMASH: lcd.print("Smash"); break;
        case GENERAL: lcd.print("General"); break;
      }
      break;

    case CHOIX_DIFFICULTE:
      lcd.setCursor(0, 0);
      lcd.print("Difficulte:");
      lcd.setCursor(0, 1);
      switch (currentDiff) {
        case DEBUTANT: lcd.print("Debutant"); break;
        case FACILE: lcd.print("Facile"); break;
        case NORMAL: lcd.print("Normal"); break;
        case DIFFICILE: lcd.print("Difficile"); break;
        case EXTREME: lcd.print("Extreme"); break;
      }
      break;

    case CONFIRMATION:
      lcd.setCursor(0, 0);
      lcd.print("Commencer ?");
      lcd.setCursor(0, 1);
      lcd.print("Valider/Annuler");
      break;

    case EN_TRAINEMENT:
      lcd.setCursor(0, 0);
      lcd.print("Entrainement en");
      lcd.setCursor(0, 1);
      lcd.print("cours...");
      break;

    case PAUSE:
      lcd.setCursor(0, 0);
      lcd.print("Entrainement");
      lcd.setCursor(0, 1);
      lcd.print("en pause...");
      break;

    case QUIT_CONFIRM:
      lcd.setCursor(0, 0);
      lcd.print("Quitter ?");
      lcd.setCursor(0, 1);
      lcd.print("Valider/Annuler");
      break;
  }
}

void loop() {
  int potValue = analogRead(POT_SELECT);

  // Gestion potentiomètre
  switch (state) {
    case CHOIX_MODE:
      currentMode = (potValue < 512) ? DROITIER : GAUCHER;
      break;
    case CHOIX_ENTRAINEMENT:
      currentTrain = static_cast<Train>(map(potValue, 0, 1023, 0, 4));
      break;
    case CHOIX_DIFFICULTE:
      currentDiff = static_cast<Diff>(map(potValue, 0, 1023, 0, 5));
      break;
    default:
      break;
  }

  // Rafraîchissement intelligent
  bool needUpdate = false;
  if (state != lastDisplayedState) {
    needUpdate = true;
    lastDisplayedState = state;
  }
  if (state == CHOIX_MODE && currentMode != lastMode) {
    needUpdate = true;
    lastMode = currentMode;
  } else if (state == CHOIX_ENTRAINEMENT && currentTrain != lastTrain) {
    needUpdate = true;
    lastTrain = currentTrain;
  } else if (state == CHOIX_DIFFICULTE && currentDiff != lastDiff) {
    needUpdate = true;
    lastDiff = currentDiff;
  }

  if (needUpdate) displayState();

  // Bouton VALIDER
  if (isButtonPressed(BTN_VALIDATE, BTN_VALIDATEPrevState)) {
    switch (state) {
      case CHOIX_MODE:
        state = CHOIX_ENTRAINEMENT;
        break;
      case CHOIX_ENTRAINEMENT:
        state = CHOIX_DIFFICULTE;
        break;
      case CHOIX_DIFFICULTE:
        state = CONFIRMATION;
        break;
      case CONFIRMATION:
        state = EN_TRAINEMENT;
        break;
      case QUIT_CONFIRM:
        state = CHOIX_MODE; // Fin
        break;
      default:
        break;
    }
    displayState();
  }

  // Bouton ANNULER
  if (isButtonPressed(BTN_BACK, BTN_BACKPrevState)) {
    switch (state) {
      case CHOIX_ENTRAINEMENT:
        state = CHOIX_MODE;
        break;
      case CHOIX_DIFFICULTE:
        state = CHOIX_ENTRAINEMENT;
        break;
      case CONFIRMATION:
        state = CHOIX_DIFFICULTE;
        break;
      case EN_TRAINEMENT:
        state = PAUSE;
        break;
      case PAUSE:
        state = QUIT_CONFIRM;
        break;
      case QUIT_CONFIRM:
        state = EN_TRAINEMENT; // Reprise
        break;
      default:
        break;
    }
    displayState();
  }
}
