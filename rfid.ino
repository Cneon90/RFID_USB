#include <SPI.h>
#include <MFRC522.h>
#include "Keyboard.h"

#define RU        1 //Если по умолчанию русский то 1 если ENG то 0
#define ALT       1 //Если смена shift+alt то 1, если shift+ctrl то 0

void exit_windows()//Выход из windows (Блокировка)
{
    Keyboard.press(KEY_LEFT_GUI);//После кнопку win
    delay(100);
    Keyboard.press('l');
    Keyboard.releaseAll();
}


//----------------Вход в винду-----------------
void  enter()
{
    //Нажимаем Enter что бы экран перевел на поле ввода пароля
    Keyboard.press(KEY_LEFT_CTRL);
    delay(100);
    Keyboard.releaseAll();
    delay(1500);//ждем пока экран переведется из заставки на ввод пароля
    
           if (RU) //Если по умолчанию русский то перед вводом надо сменить пароль 
           {
              Keyboard.press(KEY_LEFT_SHIFT);
                 if(ALT) //Если компинация клавиш для смены языка shift+alt
                        Keyboard.press(KEY_LEFT_ALT);
                 else //Если компинация клавиш для смены языка shift+ctrl
                        Keyboard.press(KEY_LEFT_CTRL);
               delay(100);
               Keyboard.releaseAll();
            }
    Keyboard.print("ZKDMx1990LiriK1990"); //Непосредственный ввод пароля
    delay(100);
    Keyboard.press(KEY_RETURN);//После ввода пароля нажимаем ентер для входа в windows
    Keyboard.releaseAll();
   //delay(10000);
   //exit_windows();
}

MFRC522 rfid(10, 9); //10 порт - RST, 9 порт - SDA

//A0 E8 3F 28
byte idCard[] = {
  0xA0,
  0xE8,
  0x3F,
  0x28
};

void setup() {
  SPI.begin(); // Инициализируем SPI
  rfid.PCD_Init(); // Инициализируем RFID
  Serial.begin(9600); // Инициализируем Serial
  Keyboard.begin();// Инициализируем Клавиатуры
}


void loop() {
  // Если ключ поднесен к считывателю
boolean acess=1;//Определяем получит ли доступ ключ, изначально 1, если хоть одно не совпадение, то переведем в 0
    if ( rfid.PICC_IsNewCardPresent()) {
    // Если удалось считать серийный номер ключа
        if (rfid.PICC_ReadCardSerial()) {
                   Serial.print("Card ID:");
                  //Перебираем массив rfid.uid.uidByte, ее размер
                  //хранится в переменной rfid.uid.size
                      for (byte i = 0; i < rfid.uid.size; i++) {
                         if(rfid.uid.uidByte[i]!=idCard[i]) 
                               {
                                 acess=0; //Если было хоть одно не совпадение
                               }

                          Serial.print(F(" "));//Отладочная информация
                          //Выводим элемент массива
                          Serial.print(rfid.uid.uidByte[i], HEX);
                        }
                   Serial.println(""); //переносим строку
                   Serial.println(acess);
                          }

              if(acess==1) 
                {
                  enter();
                }
      }
 }
