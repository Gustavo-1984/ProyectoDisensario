
int pinA = 35;
int pinB = 34;
int Pos = 0;
int Aux;

unsigned long times;
unsigned long t;
bool C = true;
bool D = true;

void setup() {
  Serial.begin(9600);
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  Serial.print("Pulso");
  Serial.println(Pos);
}

void loop() {



  times = millis();

  if(digitalRead(pinA) == LOW){
    t = times;
    if(C == true){
      Pos++;
      Serial.print ("Pulso");
      Serial.println(Pos);
      C = false;
      D = false;
    }
  }

  if(digitalRead(pinB) == LOW){
    t = times;

    if(D == true){
      Pos--;
      Serial.print("Pulso");
      Serial.println(Pos);
      D= false;
      C= false;
    }
  }

  if (times-t>5){
    C = true;
    D = true;
  }
}
