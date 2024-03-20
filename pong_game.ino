/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

// include the library code:
#include <LiquidCrystal.h>
#include <EEPROM.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/*
 Pong - Wersja gry na Ekran LCD 16x2
 Autor: Artur Rusa
 Zasady gry: gdy piłka jest na dolnej lini, gracz musi wcisnąć swój przycisk by ją odbić, w przypadku niepoprawnego
 ruchu naliczany jest punkt dla przeciwnika
 Kod jest stary i zrobiony na szybko na lekcji 2 lata temu ale powinien działać. Pozdro

  Jak podłączyć odpowiednio przyciski:
  7 - gracz po lewej stronie ekranu
  8 - gracz po prawej stronie ekranu
  A0 - PRZYCISK START
  istnieje możliwość zmiany poszczególnych pinów w celu wygody poniżej:
  */
    int rightButton = 8;  //prawy przycisk
    int leftButton = 7;   //lewy przycisk

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  pinMode(leftButton,INPUT); // wejście przycisku gracza po lewej stronie ekranu
  pinMode(rightButton,INPUT); // wejście przycisku gracza po prawej stronie ekranu
  pinMode(A0,INPUT); // wejście przycisku start

  Serial.begin(9600);    // Serial
  
  menu(); // wywołanie ekranu z wyborem liczby graczy oraz trudnością gry
}

void punkt_L(int l){ // w zależności od 1 lub 2 znaków wartość jest wyświetlana w odpowiednim miejscu
  if(l<10){
    lcd.setCursor(1,0);
  }else{
    lcd.setCursor(0,0);
  }
    lcd.print(l);
}

void punkt_P(int p){ // w zależności od 1 lub 2 znaków wartość jest wyświetlana w odpowiednim miejscu
  if(p<10){
    lcd.setCursor(15,0);
  }else{
    lcd.setCursor(14,0);
  }
    lcd.print(p);
}

void poczatek(){ //początek to losowe miejsce piłki przy nowej grze
int poczatek = random(0,2);
        lcd.setCursor(3,poczatek);
        lcd.print("o");
        delay(1000);
        lcd.setCursor(3,poczatek);
        lcd.print(" ");
}

void prawyP(){ //wykrywanie nacisku prawego przycisku
  int buttonStateP = digitalRead(8);
          if(buttonStateP == HIGH){
            lcd.setCursor(13,0);
            lcd.print(" ");
            lcd.setCursor(13,1);
            lcd.print("|");
          }else{
            lcd.setCursor(13,1);
            lcd.print(" ");
            lcd.setCursor(13,0);
            lcd.print("|");
          }
}
void prawyP_bot(){ //wykrywanie nacisku prawego przycisku. Miejsce zostawione na nowy kod

}

void LewyP(){ //wykrywanie nacisku lewego przycisku
  int buttonStateL = digitalRead(7);
          if(buttonStateL == HIGH){
            lcd.setCursor(2,0);
            lcd.print(" ");
            lcd.setCursor(2,1);
            lcd.print("|");
          }else{
            lcd.setCursor(2,1);
            lcd.print(" ");
            lcd.setCursor(2,0);
            lcd.print("|");
          }
}

void rozgrywka(){ // pętla rozgrywki między graczami
  for(long gra=1;gra++;){

    int gameSpeed = random(1,3); //losowanie zmiany prędkości gry u różnych stron w celu balansu rozgrywki
    int opoznienie = 200;
    int czas = 10 * gra;

    if(gameSpeed == 1){
      opoznienie = 200 - czas;
      Serial.println("Pilka przyspiesza z lewej strony");
        if(opoznienie <1){
        opoznienie = 1;
        }
    }
 //przyśpieszanie piłki idacej z lewej strony do prawej z kolejnym odbiciem

    int drogaP = random(0,2); //losowanie lini
        for(int a=4; a<13;a++){
          LewyP();
          prawyP();
        lcd.setCursor(a,drogaP);
        lcd.print("o");
        delay(opoznienie);
        lcd.setCursor(a,drogaP);
        lcd.print(" ");
        }

        if(drogaP == 0){
          int buttonStateP = digitalRead(rightButton);
          if(buttonStateP == HIGH){
            Serial.println("prawy przycisk klikniety");
            int koniec = 1;
            EEPROM.write(3, koniec);
            prawyP();
            return 0;
          }else{
            Serial.println("prawy przycisk wolny");
            prawyP();
          }
        }
        else{
          int buttonStateP = digitalRead(rightButton);
          if(buttonStateP == LOW){
            Serial.println("prawy przycisk wolny");
            int koniec = 1;
            EEPROM.write(3, koniec);
            prawyP();
            return 0;
          }else{
            Serial.println("prawy przycisk klikniety");
            prawyP();
          }
        }

    if(gameSpeed == 2){
      opoznienie = 200 - czas;
      Serial.println("Pilka przyspiesza z prawej strony");
        if(opoznienie <1){
        opoznienie = 1;
        }
    }
 //przyśpieszanie piłki idacej z prawej strony do lewej z kolejnym odbiciem w celu utrudnia rozgrywki i wyeleiminowania braku zwycięzcy

    int drogaL = random(0,2); //losowanie lini
      for(int a=11; a>2;a--){
          LewyP();
          prawyP();
        lcd.setCursor(a,drogaL);
        lcd.print("o");
        delay(opoznienie);
        lcd.setCursor(a,drogaL);
        lcd.print(" ");
        }
        if(drogaL == 0){
          int buttonStateL = digitalRead(leftButton);
          if(buttonStateL == HIGH){
            Serial.println("lewy przycisk klikniety");
            LewyP();
            int koniec = 2;
            EEPROM.write(3, koniec);
            return 0;
          }else{
            Serial.println("lewy przycisk wolny");
            LewyP();
          }
        }
        else{
          int buttonStateL = digitalRead(leftButton);
          if(buttonStateL == LOW){
            Serial.println("lewy przycisk wolny");
            LewyP();
            int koniec = 2;
            EEPROM.write(3, koniec);
            return 0;
          }else{
            Serial.println("lewy przycisk klikniety");
            LewyP();
          }
        }
}
}

void menu(){
  lcd.setCursor(0,0);
  lcd.print("Ilosc graczy:1");
  int p = 1;                          //ilosc graczy

  menu:
  int buttonStateP = digitalRead(8);  //przycisk prawy
  int buttonStateL = digitalRead(7);  //przycisk lewy
          if(buttonStateP == HIGH){   //zapis ilosci graczy
            lcd.setCursor(13,0);
            lcd.print("2");
            p=2;
            Serial.println("Rozgrywka 2 graczy");

          }else if(buttonStateL == HIGH){
            lcd.setCursor(13,0);
            lcd.print("1");
            p=1;
            Serial.println("Rozgrywka 1 gracza");
          }

  int c = millis() / 1000;          //wyswietlanie strzalek. W ten sposob zeby nie bylo opoznienia przyciskow dla graczy
  if(c%2){
    lcd.setCursor(0,1);
    lcd.print("    <     >     ");
  }else{
    lcd.setCursor(0,1);
    lcd.print("                ");
  }

  int buttonStateStart = digitalRead(A0);   //przycisk start
  int m=1;
  if(buttonStateStart == HIGH){
            m=2;
          }

  if(m==1){
    goto menu;
  }else{
    if(p==1){
      EEPROM.write(6, p); //zapis czy gracz czy bot
      lcd.clear();
    }else{
       EEPROM.write(6, p);
       lcd.clear();
       goto end;
    }
    goto trudnosc;
  }
  goto end;

  trudnosc:
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LADOWANIE GRY");
  delay(1000);  //input przycisku bedzie za dlugo i by wywalilo do poczatku gry bez wyboru poziomu trudnosci. Mozna zrobic na petli by czasowo przycisk nie dzialal ale po co.
  lcd.setCursor(0,0);
  lcd.print("Poziom bota:1");
  int t = 1;
  EEPROM.write(20, t); //zapis trudnosci
  trudnosc_wybierz:
  int buttonStateP_diff = digitalRead(rightButton);  //przycisk prawy
  int buttonStateL_diff = digitalRead(leftButton);  //przycisk lewy
          if(buttonStateP_diff == HIGH){
            lcd.setCursor(12,0);
            lcd.print("2");
            t=3;
            Serial.println("Poziom bota: 2");
            EEPROM.write(20, t); //zapis trudnosci

          }else if(buttonStateL_diff == HIGH){
            lcd.setCursor(12,0);
            lcd.print("1");
            t=1;
            Serial.println("Poziom bota: 2");
            EEPROM.write(20, t); //zapis trudnosci
          }

  int ct = millis() / 1000;          //wyswietlanie strzalek
  if(ct%2){
    lcd.setCursor(0,1);
    lcd.print("    <     >     ");
  }else{
    lcd.setCursor(0,1);
    lcd.print("                ");
  }

  int buttonStateStart_diff = digitalRead(A0);   //przycisk start
  int mt=1;
  if(buttonStateStart_diff == HIGH){
            mt=2;
          }

  if(mt==1){
    goto trudnosc_wybierz;              //jezeli nie naciesnieto to robimy petle
  }else{
      lcd.clear();
      Serial.println("przechodze do end");  //zamykamy menu i przechodzimy do gry
      goto end;
  
  }

  goto trudnosc_wybierz;        //awaryjna petla bo wszystko dziala na sline. Wracamy do wyboru poziomu trudnosci

  end:         //tutaj clear lcd i lecimy do gry
  lcd.clear();
  Serial.print("przechodze do gry z botem. Poziom:");
  Serial.println(EEPROM.read(20)); // odczyt trudnosci

}

void rozgrywka_bot(){ // pętla rozgrywki między graczami
  for(long gra=1;gra++;){

    int czas = 10 * gra;
    int opoznienie = 200 - czas;
    if(opoznienie <1){
      opoznienie = 1;
    } //przyśpieszanie piłki z kolejnym odbiciem w celu utrudnia rozgrywki i wyeleiminowania braku zwycięzcy

    int drogaP = random(0,2);
        for(int a=4; a<13;a++){
          LewyP();
          prawyP_bot();
        lcd.setCursor(a,drogaP);
        lcd.print("o");
        delay(opoznienie);
        lcd.setCursor(a,drogaP);
        lcd.print(" ");
        }

        if(drogaP == 0){
          int t = EEPROM.read(20) +2 * 2;  //odczyt trudnosci
          int buttonStateP = random(1,t);
          if(buttonStateP == 1){
            Serial.println("prawy przycisk klikniety");
            int koniec = 1;
            EEPROM.write(3, koniec);
            lcd.setCursor(13,0);
            lcd.print(" ");
            lcd.setCursor(13,1);
            lcd.print("|");
            return 0;
          }else{
            Serial.println("prawy przycisk wolny");
            lcd.setCursor(13,1);
            lcd.print(" ");
            lcd.setCursor(13,0);
            lcd.print("|");
          }
        }
        else{
          int t = EEPROM.read(20) +2 * 2;  //odczyt trudnosci
          int buttonStateP = random(1,t);
          if(buttonStateP == 1 ){
            Serial.println("prawy przycisk wolny");
            int koniec = 1;
            EEPROM.write(3, koniec);
            lcd.setCursor(13,1);
            lcd.print(" ");
            lcd.setCursor(13,0);
            lcd.print("|");
            return 0;
          }else{
            Serial.println("prawy przycisk klikniety");
            lcd.setCursor(13,0);
            lcd.print(" ");
            lcd.setCursor(13,1);
            lcd.print("|");
          }
        }

    int drogaL = random(0,2);
      for(int a=11; a>2;a--){
          LewyP();
          prawyP_bot();
        lcd.setCursor(a,drogaL);
        lcd.print("o");
        delay(opoznienie);
        lcd.setCursor(a,drogaL);
        lcd.print(" ");
        }
        if(drogaL == 0){
          int buttonStateL = digitalRead(leftButton);
          if(buttonStateL == HIGH){
            Serial.println("lewy przycisk klikniety");
            LewyP();
            int koniec = 2;
            EEPROM.write(3, koniec);
            return 0;
          }else{
            Serial.println("lewy przycisk wolny");
            LewyP();
          }
        }
        else{
          int buttonStateL = digitalRead(leftButton);
          if(buttonStateL == LOW){
            Serial.println("lewy przycisk wolny");
            LewyP();
            int koniec = 2;
            EEPROM.write(3, koniec);
            return 0;
          }else{
            Serial.println("lewy przycisk klikniety");
            LewyP();
          }
        }
  }
}

void loop() { //pełna gra, po przekroczeniu wartości 99 gra zaczyna się od nowa
int liczba_graczy = EEPROM.read(6);
if(liczba_graczy==2){
  goto gra_gracze;  //przechodzimy do gry z graczem
}else{
  goto gra_bot;     //przechodzimy do gry z botem
}
gra_gracze:
Serial.println("----------------");
Serial.println("Rozgrywka Graczy");
Serial.println("----------------");
lcd.setCursor(0,0);
  lcd.print("00|          |00");
  lcd.setCursor(0,1);
  lcd.print("P1            P2"); //ekran początkowy nowej gry
  int lewo = 0;
  EEPROM.write(1, lewo);
  int prawo = 0;
  EEPROM.write(2, prawo);

  for(int gra = 1;gra<100; gra++){ //limit  2 znaków skraca długość gry do max 99
    poczatek();
  rozgrywka();

  int strona =  EEPROM.read(3); //odczytywanie która strona otrzymała punkt
  if(strona == 1){
    int lewo = EEPROM.read(1);
    EEPROM.write(1, lewo+1);
    int punkt = EEPROM.read(1); //nadpisanie wartości punktu o 1 więcej
    punkt_L(punkt); //zapisanie na ekranie
  }
  else{
    int prawo = EEPROM.read(2);
    EEPROM.write(2, prawo+1);
    int punkt = EEPROM.read(2); //nadpisanie wartości punktu o 1 więcej
    punkt_P(punkt); //zapisanie na ekranie
  }
  }

goto gra_gracze;  // koniec pętli gry z graczem

gra_bot:                //gra z botem, nowa petla
Serial.println("-----------------");
Serial.println("Rozgrywka z Botem");
Serial.println("-----------------");
lcd.setCursor(0,0);
  lcd.print("00|          |00");
  lcd.setCursor(0,1);
  lcd.print("P1            AI"); //ekran początkowy nowej gry
  int lewo_bot = 0;
  EEPROM.write(11, lewo_bot);
  int prawo_bot = 0;
  EEPROM.write(12, prawo_bot);

  for(int gra = 1;gra<100; gra++){ //limit  2 znaków skraca długość gry do max 99
    poczatek();
  rozgrywka_bot();

  int strona =  EEPROM.read(3); //odczytywanie która strona otrzymała punkt
  if(strona == 1){
    int lewo_bot = EEPROM.read(11);
    EEPROM.write(11, lewo_bot+1);
    int punkt = EEPROM.read(11); //nadpisanie wartości punktu o 1 więcej
    punkt_L(punkt); //zapisanie na ekranie
  }
  else{
    int prawo_bot = EEPROM.read(12);
    EEPROM.write(12, prawo_bot+1);
    int punkt = EEPROM.read(12); //nadpisanie wartości punktu o 1 więcej
    punkt_P(punkt); //zapisanie na ekranie
  }
  }

goto gra_bot; // koniec pętli gry z botem
}
