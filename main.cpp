#include <Arduino.h>

#define S1_ON LOW
#define S1_OFF HIGH
#define S2_ON LOW
#define S2_OFF HIGH
#define S3_ON HIGH
#define S3_OFF LOW

int s1 = 17;
int s2 = 18;
int s3 = 8;
int pp = 15;
int sln = 4;

// Definicao dos estados
typedef enum {
  T_0,
  T_1,
  T_2,
  T_3,
  T_4,
  T_5,
  T_6,
  T_7,
  T_8,
  T_9,
  T_10,
  T_11,
  T_12,
  NUM_STATES
} State;
int slncurrentState = HIGH;   // Variable to store the current state of the input pin
int slnPreviousState = HIGH;  // Variable to store the previous state of the input pin

bool waitForFallingEdge(unsigned long timeout) {
  unsigned long startTime = millis();  // Record the start time

  while (millis() - startTime < timeout) {
    slncurrentState = digitalRead(sln);  // Read the current state of the input pin

    // Check for a 1 to 0 transition (falling edge)
    if (slnPreviousState == HIGH && slncurrentState == LOW) {
      slnPreviousState = slncurrentState;  // Update the previous state
      return true;                         // Falling edge detected
    }

    // Update the previous state
    slnPreviousState = slncurrentState;
  }

  return false;  // Timeout occurred without detecting a falling edge
}
// Function to wait for a falling edge without a timeout
bool waitForFallingEdgeWithoutTimeout() {
  while (true) {
    slncurrentState = digitalRead(sln);  // Read the current state of the input pin

    // Check for a 1 to 0 transition (falling edge)
    if (slnPreviousState == HIGH && slncurrentState == LOW) {
      slnPreviousState = slncurrentState;  // Update the previous state
      return true;                         // Falling edge detected
    }

    // Update the previous state
    slnPreviousState = slncurrentState;
    delay(170);
  }
}

// Prototipacao das funções de acao -> cada estado
void actionT_0();
void actionT_1();
void actionT_2();
void actionT_3();
void actionT_4();
void actionT_5();
void actionT_6();
void actionT_7();
void actionT_8();
void actionT_9();
void actionT_10();
void actionT_11();
void actionT_12();

// Prototipacao das funções de **        -> Transicao T2 -> T3
void transitionT_0toT_1();
void transitionT_1toT_2();
void transitionT_2toT_3();
void transitionT_3toT_4();
void transitionT_4toT_5();
void transitionT_5toT_6();
void transitionT_6toT_7();
void transitionT_7toT_8();
void transitionT_8toT_9();
void transitionT_9toT_10();
void transitionT_10toT_11();
void transitionT_11toT_12();
void transitionT_12toT_0();

// Array de ponteiros -> funções de acao
void (*stateActions[NUM_STATES])() = {
  actionT_0,
  actionT_1,
  actionT_2,
  actionT_3,
  actionT_4,
  actionT_5,
  actionT_6,
  actionT_7,
  actionT_8,
  actionT_9,
  actionT_10,
  actionT_11,
  actionT_12,
};

// Array de ponteiros -> funções de **        -> Transicao T2 -> T3
void (*transitions[NUM_STATES])() = {
  transitionT_0toT_1,
  transitionT_1toT_2,
  transitionT_2toT_3,
  transitionT_3toT_4,
  transitionT_4toT_5,
  transitionT_5toT_6,
  transitionT_6toT_7,
  transitionT_7toT_8,
  transitionT_8toT_9,
  transitionT_9toT_10,
  transitionT_10toT_11,
  transitionT_11toT_12,
  transitionT_12toT_0,
};

// Variável -> armazenar o estado atual
State currentState = T_0;

// Funções de acao
void actionT_0() {
  Serial.println();
  Serial.println("Acao T_0 ....Standby....");
  digitalWrite(s1, S1_ON);  // T_0
  digitalWrite(s2, S2_ON);
  digitalWrite(s3, S3_OFF);
}
void actionT_1() {
  Serial.println();
  Serial.println("Acao T1 -> DESTRAVAR PORTA, a aguardar utilizador");
  digitalWrite(s1, S1_ON);  // T_1
  digitalWrite(s2, S2_OFF);
  digitalWrite(s3, S3_OFF);
  delay(3000);
}
void actionT_2() {
  Serial.println();
  Serial.println("Acao T2 -> ABERTURA PORTA 1, pelo utilizador");
  digitalWrite(s1, S1_OFF);  // T_2
  digitalWrite(s2, S2_ON);
  digitalWrite(s3, S3_OFF);
  delay(1700);
}
void actionT_3() {
  Serial.println();
  Serial.println("Acao T3 -> ABERTURA PORTA 2, pelo utilizador");
  digitalWrite(s1, S1_OFF);  // T_3
  digitalWrite(s2, S2_OFF);
  digitalWrite(s3, S3_OFF);
  delay(140);
}
void actionT_4() {
  Serial.println();
  Serial.println("Acao T4 -> PORTA ABERTA, a aguardar conexao cabo");
  digitalWrite(s1, S1_OFF);  // T_4
  digitalWrite(s2, S2_ON);
  digitalWrite(s3, S3_ON);
  delay(2000);
  Serial.println("        -> Sinal PP --ON--");
  digitalWrite(pp, HIGH);
}
void actionT_5() {
  Serial.println();
  Serial.println("Acao T5 -> LIBERAR PORTA, cabo conectado");
  digitalWrite(s1, S1_OFF);  // T_5
  digitalWrite(s2, S2_OFF);
  digitalWrite(s3, S3_OFF);
}

void actionT_6() {
  Serial.println();
  Serial.println("Acao T6 -> TRAVAR PORTA, cabo conectado");
  digitalWrite(s1, S1_ON);  // t6
  delay(400);
  Serial.println("        -> VEICULO EM CARGA");
  digitalWrite(s2, S2_ON);
  digitalWrite(s3, S3_OFF);
}
void actionT_7() {
  Serial.println();
  Serial.println("Acao T7 -> LIBERAR PORTA, fim carga");
  digitalWrite(s1, S1_OFF);  // t7
  digitalWrite(s2, S2_OFF);
  digitalWrite(s3, S3_OFF);
}
void actionT_8() {
  Serial.println();
  Serial.println("Acao T8 -> ABERTURA PORTA 1, pelo utilizador, fim carga");
  digitalWrite(s1, S1_OFF);  // t8
  digitalWrite(s2, S2_ON);
  digitalWrite(s3, S3_OFF);
  delay(1000);
}
void actionT_9() {
  Serial.println();
  Serial.println("Acao T9 -> ABERTURA PORTA 2, pelo utilizador, fim carga");
  digitalWrite(s1, S1_OFF);  // t9
  digitalWrite(s2, S2_OFF);
  digitalWrite(s3, S3_OFF);
  Serial.println("        -> Sinal PP --OFF--, fim carga");
  digitalWrite(pp, LOW);
  delay(120);
}
void actionT_10() {
  Serial.println();
  Serial.println("Acao T10 -> RETIRAR CABO, fim carga");
  digitalWrite(s1, S1_OFF);  // t10
  digitalWrite(s2, S2_ON);
  digitalWrite(s3, S3_ON);
}
void actionT_11() {
  Serial.println();
  Serial.println("Acao T11 -> LIBERAR PORTA, fim carga");
  digitalWrite(s1, S1_OFF);  // t11
  digitalWrite(s2, S2_OFF);
  digitalWrite(s3, S3_OFF);
  delay(1000);
}
void actionT_12() {
  Serial.println();
  Serial.println("Acao T12 -> TRAVAR PORTA, fim carga");
  digitalWrite(s1, S1_ON);  // t12
  delay(400);
  digitalWrite(s2, S2_ON);
  digitalWrite(s3, S3_OFF);
}
// Funções de Transicao
void transitionT_0toT_1() {
  Serial.println("        -> Transicao T0 -> T1");
  waitForFallingEdgeWithoutTimeout();
}
void transitionT_1toT_2() {
  Serial.println("        -> Transicao T1 -> T2");
}
void transitionT_2toT_3() {
  Serial.println("        -> Transicao T2 -> T3");
}
void transitionT_3toT_4() {
  Serial.println("        -> Transicao T3 -> T4");
}
void transitionT_4toT_5() {
  Serial.println("        -> Transicao T4 -> T5");
  waitForFallingEdgeWithoutTimeout();
}
void transitionT_5toT_6() {
  Serial.println("        -> Transicao T5 -> T6");
  waitForFallingEdgeWithoutTimeout();
}
void transitionT_6toT_7() {
  Serial.println("        -> Transicao T6 -> T7");
  waitForFallingEdgeWithoutTimeout();
}
void transitionT_7toT_8() {
  Serial.println("        -> Transicao T7 -> T8");
  waitForFallingEdgeWithoutTimeout();
}
void transitionT_8toT_9() {
  Serial.println("        -> Transicao T8 -> T9");
}
void transitionT_9toT_10() {
  Serial.println("        -> Transicao T9 -> T10");
}
void transitionT_10toT_11() {
  Serial.println("        -> Transicao T10 -> T11");
  waitForFallingEdgeWithoutTimeout();
}
void transitionT_11toT_12() {
  Serial.println("        -> Transicao T11 -> T12");
  waitForFallingEdgeWithoutTimeout();
}
void transitionT_12toT_0() {
  Serial.println("        -> Transicao T12 -> T0");
}
// Funcao -> mudar de estado
void changeState(State newState) {
  // Executa a funcao de transicao para o novo estado
  transitions[currentState]();
  // Muda -> o novo estado
  currentState = newState;
}

void setup() {
  Serial.begin(115200);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(sln, INPUT);  // Configurando o pino como entrada com pull-up interno
  pinMode(pp, OUTPUT);
}

void loop() {
  //delay(500);
  //Serial.println("IN LOOP");
  // Executa a Acao do estado atual
  stateActions[currentState]();
  // Aguarda um segundo -> melhor visualizacao no monitor serial
  delay(100);

  // Muda -> o próximo estado (circular)
  State nextState = static_cast<State>((currentState + 1) % NUM_STATES);
  changeState(nextState);
}

