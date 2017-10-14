const int pinBouton = A0;
const int pinTone = 8;
int pinLed[4] = {2,3,4,5};

int sequence[100]; // tableau pour la séquence
int frequences[4] = {528,660,792,950}; // Frequences des 4 notes
int longueurSequence = 1; // Varialble pour stocker la longueur de la sequence lue
int score = 0;

void setup() {
  // Initialisation des entrées/sorties
  for (int p = 0; p < 4; p++) pinMode(pinLed[p], OUTPUT);
  pinMode(pinTone, OUTPUT);

  // Initialisation de la sortie série
  Serial.begin(9600);
  Serial.println("Port serie : ok !"); Serial.println();

  // Séquence de démarrage
  int sequenceDemarrage[4] = {1,3,2,4};
  for (int i = 0; i < 4; i++) {
    instr(sequenceDemarrage[i]);
  }
  delay(1000);
  lancement(); // Appel de la fonction d'initialisation de la sequence
}

void loop() {
  simonJoue(); // Appel de la fonction que joue la sequence de simon
  joueurJoue(); // Appel de la fonction qui lit la reponse du joueur
}

void lancement() { // initialisation de la séquence de couleurs à jouer
  randomSeed(analogRead(5)); // Initialisation de la sequence aleatoire sur A5 (inutilisee)
  for (int i = 0; i < 100; i++) { // Creation de la sequence, jusqu'a 100 notes
    sequence[i] = random(4)+1;
  }
  for (int i = 0; i < 100; i++) { // Affichage de la sequance sur le monituer deris
    Serial.print(sequence[i]);
  }
}

void simonJoue() {  // joue le montage joue la séquence de couleurs avec les sons associés
  delay(1000);
  for(int i = 0; i < longueurSequence; i++) {
    instr(sequence[i]); // fonction instrument qui joue les notes et les couleurs
  }
}

void joueurJoue() { // le joueur joue puis on teste si l'appui de chaque bouton correspond à la séquence jouée par le montage.
  bool resultat; // pour tester le résultat
  int sequenceReponse[longueurSequence]; // creation du tableau des réponse du joueur
  for (byte cpt = 0; cpt < longueurSequence; cpt++) {
    while(!analogRead(pinBouton)) {} // boucle d'attente de pression des boutons
    int etatBouton = analogRead(pinBouton);
    int note = 0;
    if (etatBouton > 500) note = 1;
    if (etatBouton > 20 && etatBouton < 500) note = 2;
    if (etatBouton > 5 && etatBouton < 20) note = 3;
    if (etatBouton > 0 && etatBouton < 5) note = 4;
    sequenceReponse[cpt] = note;
    instr(note);
  }
  for (int i = 0; i < longueurSequence; i++) { // boucle de test du resultat
    if (sequenceReponse[i] == sequence[i]) resultat = true;
    else {
      resultat = false;
      break; // on sort du test si une difference est detectee
    }
  }
  if (resultat) valide();
  else nonValide();
}

void valide() {
  score++;
  longueurSequence++;
  for(int i = 0; i < 3; i++) {
    for (int f = 100; f < 3000; f++) {
      tone(pinTone, f);
    }
    noTone(pinTone); 
  }
}

void nonValide() {
  for(int i = 0; i < 3; i++) {
    for (int f = 3000; f > 100; f--) {
      tone(pinTone, f);
    }
    noTone(pinTone); 
  }
  lancement();
  longueurSequence = 1;
}

void instr(int note) {
  int vitesseNote = 300;
  switch (note) {
    case 1:
      delay(vitesseNote/2);
      tone(pinTone, frequences[0]);
      break;
    case 2:
      delay(vitesseNote/2);
      tone(pinTone, frequences[1]);
      break;
    case 3:
      delay(vitesseNote/2);
      tone(pinTone, frequences[2]);
      break;
    case 4:
      delay(vitesseNote/2);
      tone(pinTone, frequences[3]);
      break;
    default:
      delay(vitesseNote/2);
      noTone(pinTone);
  }
  digitalWrite(note+1, HIGH);
  delay(vitesseNote);
  digitalWrite(note+1, LOW);
  noTone(pinTone);
}

