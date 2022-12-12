//код от артема)
#include <LiquidCrystal_I2C.h > //библиотеки
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);//инициализация дисплея
#define RST_PIN         9 //пины для арфид метки
#define SS_PIN          10 //пины для арфид метки
// конфиг
MFRC522 mfrc522(SS_PIN, RST_PIN);
bool flag = 0;
int s;
unsigned long id, last_time;
MFRC522::MIFARE_Key key; //ключ мифаера
void setup() { //основной сетап
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  SPI.begin();
  mfrc522.PCD_Init(); // инициализация карты
  for (byte i = 0; i < 6; i++) { //ключ
    key.keyByte[i] = 0xFF;    
  }
}
void loop() {  
  if(millis()-last_time>=800){ //миллис для задержки(wait)
    last_time=millis();
    s++;
    Serial.print(s);
    
  }
  if(s==3){
    lcd.clear();
    s=0;
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //если есть карта->выводить вэйт
    lcd.setCursor(0,0);
    lcd.print("wait");
    lcd.setCursor(0,1);
    lcd.print("12.12.2022");
    
    return;}
    else{
      lcd.clear();
    
    }
  
  if ( ! mfrc522.PICC_ReadCardSerial()) //читаем карту
    return;
  byte blockAddr      = 4;
  byte trailerBlock   = 7;
  byte buffer[18];
  byte size = sizeof(buffer);
  mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  
  int count; //тут мы записываем данные на переменные
  byte date[2];
  count = (buffer[7] << 8) + buffer[8]; //байт в инт
  id = (long (buffer[0]) << 24) + (long(buffer[1]) << 16) + (long (buffer[2]) << 8) + (long (buffer[3]) << 0); //байт в лонг
  Serial.println(id);
  Serial.println(count);
  lcd.setCursor(0, 0);
  lcd.print("id: ");
  lcd.setCursor(3, 0);
  lcd.print(id);
  lcd.setCursor(0, 1);
  lcd.print("drives:");
  lcd.setCursor(7, 1);
  lcd.print(count);
  

    
  }
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();  //конец действия
}
