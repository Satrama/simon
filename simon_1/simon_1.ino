/* SIMON
 * Programme du jeu realise pour le cours "Perfectionnez-vous à la programmation Arduine
 * de Jean-Noel Rousseau sur Open CLasseroom
 * Termine le 14 octobre 2017
 * Par Satrama
 * Ce programme est libre et Open Source.
 */

const int pinBouton = A0; // pin pour les boutons en serie
const int pinTone = 8; // pin pour le buzzer
int pinLed[4] = {2,3,4,5}; // 4 pins pour les LED jeune, belue, rouge et verte

int sequence[100]; // tableau pour la séquence que jouera Simon
int frequences[4] = {528,660,792,950}; // Frequences des 4 notes
int longueurSequence = 1; // Varialble pour stocker la longueur de la sequence jouee par le joueur
int score = 0; // Stockage et initaitlisation du score

void setup() {
  // Initialisation des entrées/sorties
  for (int p = 0; p < 4; p++) pinMode(pinLed[p], OUTPUT); // LEDs
  pinMode(pinTone, OUTPUT); // Buzzer

  // Initialisation de la sortie série
  Serial.begin(9600);
  Serial.println("Port serie : ok !"); Serial.println();

  // Séquence de démarrage
  int sequenceDemarrage[4] = {1,3,2,4};
  for (int i = 0; i < 4; i++) {
    instr(sequenceDemarrage[i]);
  }
  delay(1500); // pause pour la preparation du joueur
  lancement(); // Appel de la fonction d'initialisation de la sequence
}

void loop() {
  simonJoue(); // Appel de la fonction que joue la sequence de simon
  joueurJoue(); // Appel de la fonction qui lit la reponse du joueur
}

void lancement() { // initialisation de la séquence de couleurs à jouer
  randomSeed(analogRead(5)); // Initialisation de la sequence aleatoire sur A5 (inutilisee)
  score = 0; // réinitialisation du score avant lancement
  Serial.println(); Serial.print("Score : "); Serial.print(score); // affichage du score
  for (int i = 0; i < 100; i++) { // Creation de la sequence, jusqu'a 100 notes
    sequence[i] = random(4)+1;
  }
}

void simonJoue() {  // joue la séquence de couleurs avec les sons associés
  delay(1000);
  for(int i = 0; i < longueurSequence; i++) {
    instr(sequence[i]); // Appel de la fonction "intrument"
    delay(10); // Delay pour bien distinguer les notes
  }
}

void joueurJoue() { // le joueur joue puis on teste si l'appui de chaque bouton correspond à la séquence jouée par le montage.
  bool resultat; // pour tester le résultat
  int sequenceReponse[longueurSequence]; // creation du tableau les réponses du joueur
  for (byte cpt = 0; cpt < longueurSequence; cpt++) {
    while(!analogRead(pinBouton)) {} // boucle d'attente de pression des boutons
    int etatBouton = analogRead(pinBouton);
    int note = 0; // pour stocker les notes
    if (etatBouton > 500) note = 1;
    if (etatBouton > 20 && etatBouton < 500) note = 2;
    if (etatBouton > 5 && etatBouton < 20) note = 3;
    if (etatBouton > 0 && etatBouton < 5) note = 4;
    sequenceReponse[cpt] = note; // stockage des note dans le tableau de reponses
    instr(note);
    // Test de validiter des réponses
    if (sequenceReponse[cpt] == sequence[cpt]) resultat = true;
    else {
      resultat = false;
      break; // on sort du test si une difference est detectee
    }
  }
  if (resultat) valide();
  else nonValide();
}

void valide() { // Fonction pour la victoire
  score++; // incrementation du score
  longueurSequence++; // ajout d'une note supplementaire
  delay(200);
  for(byte i = 0; i < 3; i++) { // Melodie de la victoire
    for(int l = 0; l < 4; l++) digitalWrite(pinLed[l], HIGH);
    for (int f = 100; f < 3000; f++) {
      tone(pinTone, f);
    }
    for(int l = 0; l < 4; l++) digitalWrite(pinLed[l], LOW);
    delay(50);
    noTone(pinTone); 
  }
  Serial.println(); Serial.print("Score : "); Serial.print(score); // Affichage du score
}

void nonValide() { // Fonction de la defaite
  delay(200);
  for(int l = 0; l < 4; l++) digitalWrite(pinLed[l], HIGH); // allumage de toutes les LEDs
  for (int f = 3000; f > 100; f--) { // son de la défaire et extinction une à une des LEDs
    tone(pinTone, f);
    delayMicroseconds(500);
    if(f < 2420 && f > 1840) digitalWrite(pinLed[3], LOW);
    if(f < 1840 && f > 1260) digitalWrite(pinLed[2], LOW);
    if(f < 1260 && f > 680) digitalWrite(pinLed[1], LOW);
    if(f < 680) digitalWrite(pinLed[0], LOW);
  }
  noTone(pinTone);
  lancement(); // on relance pour une nouvelle sequence
  longueurSequence = 1; // on ramene la longueur de la sequence à une note
}

void instr(int note) { // fonction "intrument" qui exxecute les notes (son et couleur)
  int vitesseNote = 350;
  switch (note) {
    case 1:
      tone(pinTone, frequences[0]); // note 528 (C4)
      break;
    case 2:
      tone(pinTone, frequences[1]); // note 660 (E4)
      break;
    case 3:
      tone(pinTone, frequences[2]); // note 792 (G4)
      break;
    case 4:
      tone(pinTone, frequences[3]); // note 950 (A#4)
      break;
    default:
      noTone(pinTone); // note "silence"
  }
  digitalWrite(note+1, HIGH); // alumage de la LED correspndant à la note jouee
  delay(vitesseNote);
  digitalWrite(note+1, LOW); // extrinction de la LED
  noTone(pinTone); // extinction du son
}

