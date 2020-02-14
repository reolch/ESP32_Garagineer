#include <BluetoothSerial.h>;


BluetoothSerial bt;

const char* bt_name = "ESP32_RUNBA";

/*
******************************************
       　      Define Area
******************************************
*/
#define OP_START     128
#define OP_SAFE_MODE 131
#define OP_POWER     133
#define OP_CLEAN     135
#define OP_DRIVE_PWM 146
#define OP_DIGIT_LED 164
#define rxPin 16
#define txPin 17
#define ddPin 4
#define LED_BUILTIN 5

/*
******************************************
       　      ProtoType宣言
******************************************
*/
void BT_Command();
void wakeUp();
void clean();
void startSafe();
void powerOff();
void opDrivePWM(signed short rightPWM, signed short leftPWM);
void opDigitLEDsASCII(signed short digit3, signed short digit2, signed short digit1, signed short digit0);


/*
******************************************
       　    グローバル変数
******************************************
*/
String a;
int cnt = 0; //ステップカウント用
String label = "Tick";
char buffer[33];

HardwareSerial Roomba(2);

void setup() {
  bt.begin(bt_name);

  Roomba.begin(115200);
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ddPin, OUTPUT);

  //Roomba側の準備が出来るまでのWait設定
  delay(3000);

  Serial.println("Roomba Start");

  wakeUp();    //Roomba電源ON
  startSafe(); //セーフモードで起動

  Serial.println("Setup Completed");
}

void loop() {
  if (bt.available()) {
    
    BT_Command();
  }
}
/*
******************************************
       　     Fuction Area
******************************************
*/
/*
******************************************
       　ASC_Codeで命令を受け取る
******************************************
*/

void BT_Command() {
  a = (String)bt.read();
  bt.println(a);
  // Play 0
  if (a == "48") {
    Serial.println("Roomba WakeUP");
    wakeUp();
  }
  // Start 1
  if (a == "49") {
    Serial.println("Roomba Clean");
    clean();

    

  }
  // Pause 2
  if (a == "50") {
    Serial.println("Roomba Stop");
    opDrivePWM(0, 0);
  }
  if (a == "51") {
    Serial.println("Roomba Right");
    opDrivePWM(70, 0);
  }
  if (a == "52") {
    Serial.println("Roomba Left");
    opDrivePWM(0, 70);
  }
  if (a == "53") {
    Serial.println("Roomba SafeMode");
    wakeUp();
    startSafe();
  }
  if (a == "54") {
    Serial.println("Roomba Forward");
    opDrivePWM(100, 100);
  }
  if (a == "55") {
    Serial.println("Roomba Back");
    opDrivePWM(-100, -100);
  }
  // random 8
  if (a == "56") {

  }
  // 何もしない 9
  if (a == "57") {

  }
}

void wakeUp () {
  digitalWrite(ddPin, HIGH);
  delay(100);
  digitalWrite(ddPin, LOW);
  delay(500);
  digitalWrite(ddPin, HIGH);
  delay(2000);
}

/*--------------------------------------------------------------------------*/
void startSafe() {
  Roomba.write(OP_START);     //Start
  Roomba.write(OP_SAFE_MODE); //Safe mode
  delay(1000);
  Serial.println("safemode");
}

/*--------------------------------------------------------------------------*/
void powerOff() {
  Roomba.write(OP_POWER);
}

/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
void clean() {
  Roomba.write(OP_CLEAN);
}
void opDrivePWM(signed short rightPWM, signed short leftPWM) {

  Roomba.write(OP_DRIVE_PWM);
  Roomba.write(rightPWM >> 8);
  Roomba.write(rightPWM);
  Roomba.write(leftPWM >> 8);
  Roomba.write(leftPWM);

}

/*--------------------------------------------------------------------------*/
void opDigitLEDsASCII(signed short digit3, signed short digit2, signed short digit1, signed short digit0) {

  Roomba.write(OP_DIGIT_LED);
  Roomba.write(digit3);
  Roomba.write(digit2);
  Roomba.write(digit1);
  Roomba.write(digit0);

}
