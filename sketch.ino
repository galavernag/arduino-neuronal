int statusBainha = 4;
int startBtn = 5;
int alertaPin = 6;
int bainhaPin = 7;
int latchPin = 8;
int dataPin = 9;
int clockPin = 10;

int delayDaTransmissao = 500;
int chanceInterrupcao = 10; // Chance de interrupção em porcentagem (10% por exemplo)

void setup() 
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(bainhaPin, INPUT);
  pinMode(alertaPin, OUTPUT);
  pinMode(statusBainha, OUTPUT);
  pinMode(startBtn, INPUT_PULLUP); // Configura o botão com pull-up interno
  
  Serial.begin(9600);
  randomSeed(analogRead(0)); // Inicializa o gerador de números aleatórios
}

void loop() {
  // Verifica o estado do botão startBtn
  if (digitalRead(startBtn) == LOW) {
    // Se o botão não estiver pressionado, não faz nada
    return;
  }

  // Verifica o estado da bainha de mielina uma vez no início
  int bainhaDeMielinaAtiva = digitalRead(bainhaPin);

  // Define o delay com base no estado da bainha de mielina
  if (bainhaDeMielinaAtiva == HIGH) {
    delayDaTransmissao = 60;
    digitalWrite(statusBainha, HIGH);
  } else {
    delayDaTransmissao = 1500;
    digitalWrite(statusBainha, LOW);
  }

  // Acende os LEDs em sequência
  for (int j = 0; j < 8; j++) {
    // Verifica se a sequência deve ser interrompida
    if (bainhaDeMielinaAtiva == 0 && random(0, 100) < chanceInterrupcao) {
      Serial.println("Sequência interrompida.");
      digitalWrite(alertaPin, HIGH);
      delay(2000);
      
      // Limpa o registrador
      digitalWrite(latchPin, LOW);
      for (int k = 0; k < 8; k++) {
        digitalWrite(dataPin, LOW);
        digitalWrite(clockPin, HIGH);    
        digitalWrite(clockPin, LOW);
      }
      digitalWrite(latchPin, HIGH);
      
      digitalWrite(alertaPin, LOW);
      
      // Retorna para o início do loop
      return; 
    }

    // Coloca e mantém o pino latch em low enquanto ocorre a transmissão
    digitalWrite(latchPin, LOW);

    // Ajusta o estado dos LEDs
    if (j == 0) {
      digitalWrite(dataPin, HIGH);
    } else {
      digitalWrite(dataPin, LOW);
    }

    digitalWrite(clockPin, HIGH);    
    digitalWrite(clockPin, LOW);

    digitalWrite(latchPin, HIGH);

    delay(delayDaTransmissao);
  }

  // Limpa o registrador ao final da sequência
  digitalWrite(latchPin, LOW);
  for (int k = 0; k < 8; k++) {
    digitalWrite(dataPin, LOW);
    digitalWrite(clockPin, HIGH);    
    digitalWrite(clockPin, LOW);
  }
  digitalWrite(latchPin, HIGH);
}
