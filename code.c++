#include <LiquidCrystal_I2C.h> // Bibliothèque pour LCD I2C (changez en fonction de votre type d'écran)

// Définition des broches pour les boutons
#define BTN_UP 2
#define BTN_DOWN 3
#define BTN_POWER 4
#define BTN_PAUSE_PLAY 5
#define BTN_VALIDATE 6
#define BTN_BACK 7
#define BTN_CHANGE_MODE 8

// Définition des modes et des niveaux de difficulté
enum Mode {DROITIER, GAUCHER};
enum Difficulty {DEBUTANT, FACILE, NORMAL, DIFFICILE, EXTREME};
enum Entrainement {REVERS, COUP_DROIT, SMASH, CHALLENGE};

Mode currentMode = DROITIER; // Mode par défaut (droitier)
Difficulty currentDifficulty = DEBUTANT; // Niveau de difficulté par défaut (débutant)
Entrainement currentEntrainement = REVERS; // Entraînement par défaut (revers)

// Création d'un objet LCD pour un écran I2C (adresse 0x27, taille 16x2)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Changez l'adresse si nécessaire

// Initialisation de l'écran et des boutons
void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  
  // Définition des broches comme entrée
  pinMode(BTN_UP, INPUT);
  pinMode(BTN_DOWN, INPUT);
  pinMode(BTN_POWER, INPUT);
  pinMode(BTN_PAUSE_PLAY, INPUT);
  pinMode(BTN_VALIDATE, INPUT);
  pinMode(BTN_BACK, INPUT);
  pinMode(BTN_CHANGE_MODE, INPUT);

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
    currentDifficulty = (Difficulty)((currentDifficulty + 1) % 5);
  } else {
    currentDifficulty = (Difficulty)((currentDifficulty - 1 + 5) % 5);
  }
}

// Fonction pour gérer les entraînements particuliers
void startTraining() {
  switch (currentEntrainement) {
    case REVERS:
      // Logique pour l'entraînement revers
      break;
    case COUP_DROIT:
      // Logique pour l'entraînement coup droit
      break;
    case SMASH:
      // Logique pour l'entraînement smash
      break;
    case CHALLENGE:
      // Logique pour l'entraînement challenge
      break;
  }
}

// Fonction principale pour gérer les entrées et l'état du système
void loop() {
  // Lecture des boutons pour gérer la navigation dans les menus
  if (digitalRead(BTN_UP) == HIGH) {
    adjustDifficulty(1); // Augmenter la difficulté
    displayInfo();
    delay(300); // Anti-rebond
  }

  if (digitalRead(BTN_DOWN) == HIGH) {
    adjustDifficulty(-1); // Diminuer la difficulté
    displayInfo();
    delay(300); // Anti-rebond
  }

  if (digitalRead(BTN_CHANGE_MODE) == HIGH) {
    toggleMode(); // Changer entre droitier et gaucher
    displayInfo();
    delay(300); // Anti-rebond
  }

  if (digitalRead(BTN_VALIDATE) == HIGH) {
    startTraining(); // Démarrer l'entraînement sélectionné
    delay(300); // Anti-rebond
  }

  if (digitalRead(BTN_BACK) == HIGH) {
    // Code pour revenir en arrière dans le menu
    // Cela pourrait simplement afficher l'écran d'accueil ou revenir à une étape précédente
    delay(300); // Anti-rebond
  }

  if (digitalRead(BTN_POWER) == HIGH) {
    // Code pour éteindre ou mettre en veille le système
    // Cela peut inclure une gestion de l'alimentation ou des modes basse consommation
    delay(300); // Anti-rebond
  }

  if (digitalRead(BTN_PAUSE_PLAY) == HIGH) {
    // Code pour mettre en pause ou reprendre l'entraînement
    delay(300); // Anti-rebond
  }
}


