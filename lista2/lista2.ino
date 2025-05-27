void setup() {
  Serial.begin(9600);

  // Piny do PWM (prędkość silników)
  pinMode(3, OUTPUT);  // EnA
  pinMode(9, OUTPUT);  // EnB

  // Piny do kierunku silników
  pinMode(4, OUTPUT); // In1 (silnik A)
  pinMode(5, OUTPUT); // In2 (silnik A)
  pinMode(7, OUTPUT); // In3 (silnik B)
  pinMode(8, OUTPUT); // In4 (silnik B)
}

// Funkcja jazdy do przodu
void forward() {
  analogWrite(3, 200);  // Prędkość silnika A
  analogWrite(9, 200);  // Prędkość silnika B

  digitalWrite(4, HIGH); // Silnik A w przód
  digitalWrite(5, LOW);
  digitalWrite(7, HIGH); // Silnik B w przód
  digitalWrite(8, LOW);

  Serial.println("Jazda do przodu");
}

// Funkcja jazdy do tyłu
void backward() {
  analogWrite(3, 200);  // Prędkość silnika A
  analogWrite(9, 200);  // Prędkość silnika B

  digitalWrite(4, LOW);  // Silnik A do tyłu
  digitalWrite(5, HIGH);
  digitalWrite(7, LOW);  // Silnik B do tyłu
  digitalWrite(8, HIGH);

  Serial.println("Jazda do tyłu");
}

// Funkcja zatrzymania silników
void stopCar() {
  analogWrite(3, 0);  // Zatrzymanie silnika A
  analogWrite(9, 0);  // Zatrzymanie silnika B

  Serial.println("STOP");
}

void loop() {
  forward();  // Jedź do przodu
  delay(2000); // 2 sekundy jazdy
  
  stopCar();  // Zatrzymaj się
  delay(1000);
  
  backward();  // Jedź do tyłu
  delay(2000);
  
  stopCar();  // Zatrzymaj się
  delay(1000);
}
