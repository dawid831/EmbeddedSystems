#define LED 13

volatile char serialInput;
volatile int delayTime;
int dotTime = 300;
int dashTime = dotTime * 3;


void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  while(Serial.available())
  {
    serialInput = Serial.read();
    switch(serialInput)
    {
      case 'a': Serial.println("a .-");
                blink(dotTime);
                blink(dashTime);
                break;
      case 'b': Serial.println("b -...");
                blink(dashTime);  
                blink(dotTime);
                blink(dotTime);
                blink(dotTime);
                break;
      case 'c': Serial.println("c -.-.");
                blink(dashTime);
                blink(dotTime);
                blink(dashTime);
                blink(dotTime);
                break;
      case 'd': Serial.println("d -..");
                blink(dashTime);
                blink(dotTime);
                blink(dotTime);
                break;
      case 'e': Serial.println("e .");
                blink(dotTime);
                break;
      case 'f': Serial.println("f ..-.");
                blink(dotTime);
                blink(dotTime);
                blink(dashTime);
                blink(dotTime);
                break;
      case 'g': Serial.println("g --.");
                blink(dashTime);
                blink(dashTime);
                blink(dotTime);
                break;
      case 'h': Serial.println("h ....");
                blink(dotTime);
                blink(dotTime);
                blink(dotTime);
                blink(dotTime);
                break;
      case 'i': Serial.println("i ..");
                blink(dotTime);
                blink(dotTime);
                break;
      case 'j': Serial.println("j .---");
                blink(dotTime);
                blink(dashTime);
                blink(dashTime);
                blink(dashTime);
                break;
      case 'k': Serial.println("k -.-");
                blink(dashTime);
                blink(dotTime);
                blink(dashTime);
                break;
      case 'l': Serial.println("l .-..");
                blink(dotTime);
                blink(dashTime);
                blink(dotTime);
                blink(dotTime);
                break;
      case 'm': Serial.println("m --");
                blink(dashTime);
                blink(dashTime);
                break;
      case 'n': Serial.println("n -.");
                blink(dashTime);
                blink(dotTime);
                break;
      case 'o': Serial.println("o ---");
                blink(dashTime);
                blink(dashTime);
                blink(dashTime);
                break;
      case 'p': Serial.println("p .--.");
                blink(dotTime);
                blink(dashTime);
                blink(dashTime);
                blink(dotTime);
                break;
      case 'q': Serial.println("q --.-");
                blink(dashTime);
                blink(dashTime);
                blink(dotTime);
                blink(dashTime);
                break;
      case 'r': Serial.println("r .-.");
                blink(dotTime);
                blink(dashTime);
                blink(dotTime);
                break;
      case 's': Serial.println("s ...");
                blink(dotTime);
                blink(dotTime);
                blink(dotTime);
                break;
      case 't': Serial.println("t -");
                blink(dashTime);
                break;
      case 'u': Serial.println("u ..-");
                blink(dotTime);
                blink(dotTime);
                blink(dashTime);
                break;
      case 'v': Serial.println("v ...-");
                blink(dotTime);
                blink(dotTime);
                blink(dotTime);
                blink(dashTime);
                break;
      case 'w': Serial.println("w .--");
                blink(dotTime);
                blink(dashTime);
                blink(dashTime);
                break;
      case 'x': Serial.println("x -..-");
                blink(dashTime);
                blink(dotTime);
                blink(dotTime);
                blink(dashTime);
                break;
      case 'y': Serial.println("y -.--");
                blink(dashTime);
                blink(dotTime);
                blink(dashTime);
                blink(dashTime);
                break;
      case 'z': Serial.println("z --..");
                blink(dashTime);
                blink(dashTime);
                blink(dotTime);
                blink(dotTime);
                break;
      default:
                Serial.println("Nieprawidłowy znak! Wpisz literę a-z.");
                return;
    }
  }

}


void blink(int time) {
  digitalWrite(LED, HIGH);
  delay(time);
  digitalWrite(LED, LOW);    
  delay(time); 
}