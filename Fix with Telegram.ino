#include <CTBot.h>
#include <CTBotDataStructures.h>
#include <CTBotDefines.h>
#include <CTBotInlineKeyboard.h>
#include <CTBotReplyKeyboard.h>
#include <CTBotSecureConnection.h>
#include <CTBotStatusPin.h>
#include <CTBotWifiSetup.h>
#include <Utilities.h>

#include <ArduinoJson.h>

#define triggerPin  D8 //D8
#define echoPin     D7 //D7
#define ena D0
#define in1 D1
#define in2 D2

CTBot myBot;
String ssid = "Galaxy";
String pass = "jessnolimit";
//String token = "1427551756:AAF_Akw83OfZ7TqK9RPOZ53_nRP0zdAvbbo";
//const int id = 648123250;

#define token  "1427551756:AAF_Akw83OfZ7TqK9RPOZ53_nRP0zdAvbbo"
#define id  648123250


long duration, jarak;

int pintu = 1;

/*
  variable pintu
  1 = tertutup
  0 = terbuka
*/

void setup() {

  Serial.setDebugOutput(true);

  Serial.begin (9600);

  Serial.println("Starting Pintu Air Otomatis Bot...");

  //Koneksi ke Wifi
  myBot.wifiConnect(ssid, pass);

  //Koneksi ke Telegram Bot
  myBot.setTelegramToken(token);

  //Cek Koneksi
  if (myBot.testConnection()) {
    Serial.println("Koneksi Bagus");
  } else {
    Serial.println("Koneksi Jelek");
  }

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
}

void bukaPintu() {

  // mengatur kecepatan motor
  analogWrite(ena, 255);

  // searah dengan jarum jam
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(90000); //lama putarnya 1.5 Menit

}

void tutupPintu() {

  // mengatur kecepatan motor
  analogWrite(ena, 255);

  // searah dengan jarum jam
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  delay(90000); //lama putarnya 1.5 Menit

}

void pintuSiaga() {

  // searah dengan jarum jam
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  delay(5000);
}

void loop() {
  TBMessage msg;

  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration / 2) / 29.1;

  Serial.println("Jarak : ");
  Serial.print(jarak);
  Serial.print("CM");

  if (jarak <= 5 && pintu == 1) {
    bukaPintu();
    Serial.print(" Pintu Buka\n");
    //myBot.sendMessage(msg.sender.id, "Pintu Buka");
    myBot.sendMessage(id, "Pintu Telah Terbuka Otomatis");
    pintuSiaga();
    pintu = 0;
  } else if (jarak >= 6 && jarak <= 31 && pintu == 1) {
    pintuSiaga();
    Serial.print(" Pintu Siaga\n");
    myBot.sendMessage(id, "Pintu Siaga");
  } else if (jarak >= 33 && pintu == 0) {
    tutupPintu();
    Serial.print(" Tutup Pintu\n");
    myBot.sendMessage(id, "Pintu Telah Tertutup Otomatis");
    pintuSiaga();
    pintu = 1;
  } else {
    pintuSiaga();
    Serial.print(" Perangkat Standby\n");
    //myBot.sendMessage(id, " Perangkat Standby");
  }

  if (myBot.getNewMessage(msg)) {
    if (msg.text.equalsIgnoreCase("Buka")) {
      bukaPintu();
      Serial.print("Perangkat Dibuka Manual");
      myBot.sendMessage(id, " Pintu Terbuka Secara Manual");
    }
    else if (msg.text.equalsIgnoreCase("Tutup")) {
      tutupPintu();
      Serial.print("Perangkat Ditutup Manual");
      myBot.sendMessage(id, "Pintu Telah Tertutup Manual");
    }
  }

}
