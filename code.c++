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
enum Train {REVERS, COUP_DROIT, SMASH, TOUS, CHALLENGE};

// Variables globales pour l'état actuel du système
Mode currentMode = DROITIER; // Mode par défaut
Diff currentDifficulty = DEBUTANT; // Niveau par défaut
Train currentTrain = TOUS; // Type d'entraînement par défaut
bool isPaused = false; // Variable pour la gestion de la pause
bool isInSleepMode = false; // Variable pour la gestion de la mise en veille

// Variables pour suivre l'état des boutons (précédent et actuel)
bool btnUpPrevState = HIGH;
bool btnDownPrevState = HIGH;
bool btnChangeModePrevState = HIGH;
bool btnValidatePrevState = HIGH;
bool btnBackPrevState = HIGH;
bool btnPowerPrevState = HIGH;
bool btnPausePlayPrevState = HIGH;

// Création d'un objet LCD pour un écran I2C (adresse 0x27, taille 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Changez l'adresse si nécessaire

// Fonction pour lire les boutons avec gestion des rebonds
bool isButtonPressed(int pin, bool &prevState) {
  bool currentState = digitalRead(pin);
  if (currentState == LOW && prevState == HIGH) {  // Changement d'état du bouton (appui)
    delay(50);  // Anti-rebond logiciel
    prevState = currentState;
    return true;
  }
  prevState = currentState;
  return false;
}

// Initialisation de l'écran et des boutons
void setup() {
  Serial.begin(9600);  // Initialisation correcte du moniteur série
  Serial.println("setup"); // Ajout du point-virgule

  lcd.init();
  lcd.backlight();

  // Définition des broches comme entrée avec pull-up interne
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
  Serial.println("displayinfo");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode: ");
  lcd.print(currentMode == DROITIER ? "Droitier" : "Gaucher");

  lcd.setCursor(0, 1);
  lcd.print("Difficulte: ");
  switch (currentDifficulty) {
    case DEBUTANT: lcd.print("Debutant"); break;
    case FACILE: lcd.print("Facile"); break;
    case NORMAL: lcd.print("Normal"); break;
    case DIFFICILE: lcd.print("Difficile"); break;
    case EXTREME: lcd.print("Extreme"); break;
  }
}

// Fonction pour changer de mode (droitier / gaucher)
void toggleMode() {
  Serial.println("togglemode");
  currentMode = (currentMode == DROITIER) ? GAUCHER : DROITIER;
  displayInfo();
}

// Fonction pour changer le niveau de difficulté
void adjustDifficulty(int direction) {
  if (direction > 0) {
    currentDifficulty = (Diff)((currentDifficulty + 1) % 5);
  } else {
    currentDifficulty = (Diff)((currentDifficulty - 1 + 5) % 5);
  }
  displayInfo();
}

// Fonction pour gérer les entraînements
void startTraining() {
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (currentTrain) {
    case REVERS: 
      lcd.print("Entrainement Revers");
      break;
    case COUP_DROIT: 
      lcd.print("Entrainement Coup Droit");
      break;
    case SMASH: 
      lcd.print("Entrainement Smash");
      break;
    case TOUS: 
      lcd.print("Entrainement General");
      break;
    case CHALLENGE:
      lcd.print("Entrainement Challenge");
      break;
  }
  delay(2000); // Afficher l'écran pendant 2 secondes
}

// Fonction pour gérer la mise en veille
void goToSleep() {
  Serial.println("sleep");
  isInSleepMode = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode veille...");
  delay(3000); // Afficher "Mode veille" pendant 3 secondes
}

// Fonction pour reprendre l'entraînement après une pause
void resumeTraining() {
  Serial.println("resumetraining");
  isPaused = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reprise de l'entrainement");
  delay(2000); // Afficher pendant 2 secondes
}

// Fonction pour mettre l'entraînement en pause
void pauseTraining() {
  Serial.println("pause");
  isPaused = true;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Entrainement en pause");
  delay(2000); // Afficher pendant 2 secondes
}

// Boucle principale
void loop() {

  Serial.println("Main LOOP ");
  // Vérification des boutons avec anti-rebond
  if (isButtonPressed(BTN_UP, btnUpPrevState)) {
    adjustDifficulty(1); // Augmenter la difficulté
  }

  if (isButtonPressed(BTN_DOWN, btnDownPrevState)) {
    adjustDifficulty(-1); // Diminuer la difficulté
  }

  if (isButtonPressed(BTN_CHANGE_MODE, btnChangeModePrevState)) {
    toggleMode(); // Changer entre droitier et gaucher
  }

  if (isButtonPressed(BTN_VALIDATE, btnValidatePrevState)) {
    startTraining(); // Démarrer l'entraînement sélectionné
  }

  if (isButtonPressed(BTN_BACK, btnBackPrevState)) {
    // Code pour revenir en arrière dans le menu
    displayInfo(); // Retourner à l'écran d'information
  }

  if (isButtonPressed(BTN_POWER, btnPowerPrevState)) {
    goToSleep(); // Passer en mode veille
  }

  if (isButtonPressed(BTN_PAUSE_PLAY, btnPausePlayPrevState)) {
    if (isPaused) {
      resumeTraining(); // Reprendre l'entraînement
    } else {
      pauseTraining(); // Mettre l'entraînement en pause
    }
  }
}
