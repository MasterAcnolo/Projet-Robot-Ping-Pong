#include <LiquidCrystal_I2C.h> // Bibliothèque pour LCD I2C

// Définition des broches pour les boutons
#define BTN_UP 8
#define BTN_DOWN 9
#define BTN_POWER 7
#define BTN_PAUSE_PLAY 10
#define BTN_VALIDATE 11
#define BTN_BACK 12
#define BTN_CHANGE_MODE 13

// Définition des modes et des niveaux de difficulté
enum Mode {DROITIER, GAUCHER};
enum Diff {DEBUTANT, FACILE, NORMAL, DIFFICILE, EXTREME};
enum Train {REVERS, COUP_DROIT, SMASH, TOUS};

Mode currentMode = DROITIER; // Mode par défaut (droitier)
Diff currentDifficulty = DEBUTANT; // Niveau de difficulté par défaut (débutant)
Train currenttrain = TOUS; // Entraînement par défaut (tous)

// Création d'un objet LCD pour un écran I2C (adresse 0x27, taille 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Changez l'adresse si nécessaire

// Initialisation de l'écran et des boutons
void setup() {
  lcd.init();
  lcd.backlight();

  // Définition des broches comme entrée avec résistance de tirage interne (INPUT_PULLUP)
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_POWER, INPUT_PULLUP);
  pinMode(BTN_PAUSE_PLAY, INPUT_PULLUP);
  pinMode(BTN_VALIDATE, INPUT_PULLUP);
  pinMode(BTN_BACK, INPUT_PULLUP);
  pinMode(BTN_CHANGE_MODE, INPUT_PULLUP);

  displayInfo(); // Afficher les informations initiales
}

// Fonction pour afficher les informations sur l'écran LCD
void displayInfo() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode: ");
  lcd.print(currentMode == DROITIER ? "Droitier" : "Gaucher");

  lcd.setCursor(0, 1);
  lcd.print("Difficulte: ");
  
  switch (currentDifficulty) {
    case DEBUTANT:
      lcd.print("Debutant");
      break;
    case FACILE:
      lcd.print("Facile");
      break;
    case NORMAL:
      lcd.print("Normal");
      break;
    case DIFFICILE:
      lcd.print("Difficile");
      break;
    case EXTREME:
      lcd.print("Extreme");
      break;
  }
}

// Fonction pour changer de mode (droitier / gaucher)
void toggleMode() {
  if (currentMode == DROITIER) {
    currentMode = GAUCHER;
  } else {
    currentMode = DROITIER;
  }
}

// Fonction pour changer le niveau de difficulté
void adjustDifficulty(int direction) {
  // Si direction > 0, on augmente la difficulté, sinon on la diminue
  if (direction > 0) {
    currentDifficulty = (Diff)((currentDifficulty + 1) % 5);
  } else {
    currentDifficulty = (Diff)((currentDifficulty - 1 + 5) % 5);
  }
}

// Fonction pour gérer les entraînements particuliers
void startTraining() {
  switch (currenttrain) {
    case REVERS:
      // Logique pour l'entraînement revers
      break;
    case COUP_DROIT:
      // Logique pour l'entraînement coup droit
      break;
    case SMASH:
      // Logique pour l'entraînement smash
      break;
    case TOUS:
      // Logique pour l'entraînement général
      break;
  }
}

// Fonction principale pour gérer les entrées et l'état du système
void loop() {
  // Lecture des boutons pour gérer la navigation dans les menus
  if (digitalRead(BTN_UP) == LOW) { // Changer pour LOW (poussé)
    adjustDifficulty(1); // Augmenter la difficulté
    displayInfo();
    delay(500); // Anti-rebond
  }

  if (digitalRead(BTN_DOWN) == LOW) { // Changer pour LOW (poussé)
    adjustDifficulty(-1); // Diminuer la difficulté
    displayInfo();
    delay(500); // Anti-rebond
  }

  if (digitalRead(BTN_CHANGE_MODE) == LOW) { // Changer pour LOW (poussé)
    toggleMode(); // Changer entre droitier et gaucher
    displayInfo();
    delay(500); // Anti-rebond
  }

  if (digitalRead(BTN_VALIDATE) == LOW) { // Changer pour LOW (poussé)
    startTraining(); // Démarrer l'entraînement sélectionné
    delay(500); // Anti-rebond
  }

  if (digitalRead(BTN_BACK) == LOW) { // Changer pour LOW (poussé)
    // Code pour revenir en arrière dans le menu
    delay(500); // Anti-rebond
  }

  if (digitalRead(BTN_POWER) == LOW) { // Changer pour LOW (poussé)
    // Code pour éteindre ou mettre en veille le système
    delay(500); // Anti-rebond
  }

  if (digitalRead(BTN_PAUSE_PLAY) == LOW) { // Changer pour LOW (poussé)
    // Code pour mettre en pause ou reprendre l'entraînement
    delay(500); // Anti-rebond
  }
}
