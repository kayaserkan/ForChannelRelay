
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
#include <Keypad.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27, A4, A5); // SDA A4 , SCL A5  set the LCD address to 0x27 for a 16 chars and 2 line display
SoftwareSerial  mySerial(A1, A2);  // RX , TX Densi Seri port modülde (1 Power +) - (2 Power -) - (3 A1 nolu pin) -(4 A2 nolu pin)

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = { { '1', '2', '3', 'A' }, { '4', '5', '6', 'B' },  { '7', '8', '9', 'C' }, { '*', '0', '#', 'D' } };
char tempVal[8], tempVal2[8], tempVal3[8], tempVal4[8];
char kgDonustur[20];
char temp[20];

byte rowPins[ROWS] = {2, 3, 4, 5};//{ 22, 24, 26, 28 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9}; //connect to the column pinouts of the keypad

String inputString = "";         // a string to hold incoming data
String displaySea = ""; // Ekrana ağırlık girilirken gösterilmesi için

boolean stringComplete = false;  // whether the string is complete
boolean val = false;
boolean noneSerial = true;
boolean runTime = false;
boolean inputValA = false, inputValB = false, inputValC = false, inputValD = false;

String sending;
double Kilo, DolumVal1, DolumVal2, DolumVal3, DolumVal4;

int sayac, valCount;

#define RELAY1 10
#define RELAY2 11
#define RELAY3 12
#define RELAY4 13

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  mySerial.begin(9600);
  lcd.init();                      // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();

  keypad.addEventListener(keypadEvent); // Add an event listener for this keypad

  DolumVal1 = EEPROM.get(0, DolumVal1);
  DolumVal2 = EEPROM.get(10, DolumVal2);
  DolumVal3 = EEPROM.get(20, DolumVal3);
  DolumVal4 = EEPROM.get(30, DolumVal4);
  /*
    DolumVal1 = EEPROM_readDouble(0); // DolumVal1 e epromdan değer okuyarak atama yapılıyor
    DolumVal2 = EEPROM_readDouble(10); // DolumVal1 e epromdan değer okuyarak atama yapılıyor
    DolumVal3 = EEPROM_readDouble(20); // DolumVal1 e epromdan değer okuyarak atama yapılıyor
    DolumVal4 = EEPROM_readDouble(30); // DolumVal1 e epromdan değer okuyarak atama yapılıyor
  */
  lcd.print("      DENSi");
  lcd.setCursor(0, 1);
  lcd.print("END. TARTI SiST. ");
  delay(3000);
  lcd.clear();
}

void loop()
{
  char key = keypad.getKey();
  serialFunc();
}

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case PRESSED:

      if (key == 'A')//A tuşuna basıldığı zaman DolumVal1 e değer girilecek ve epprom a yazılacak
      {
        inputValA = true;
        inputValB = inputValC = inputValD = false;
        DolumVal1 = 0;
        displaySea = "";
        for (int i = 0; i < 0; i++)
          tempVal[i] = 0;
        valCount = 0;
        lcd.setCursor(0, 0);
        lcd.print("1.Limit =       ");

        val = true;
        break;
      }

      if (key == 'B')//B tuşuna basıldığı zaman DolumVal2 ye değer girilecek ve epprom a yazılacak
      {
        inputValB = true;
        inputValA = inputValC = inputValD = false;
        DolumVal2 = 0;
        displaySea = "";
        for (int i = 0; i < 0; i++)
          tempVal2[i] = 0;
        valCount = 0;
        lcd.setCursor(0, 0);
        lcd.print("2.Limit =       ");

        val = true;
        break;
      }

      if (key == 'C')//C tuşuna basıldığı zaman DolumVal3 e değer girilecek ve epprom a yazılacak
      {
        inputValC = true;
        inputValA = inputValB = inputValD = false;
        DolumVal3 = 0;
        displaySea = "";
        for (int i = 0; i < 0; i++)
          tempVal3[i] = 0;
        valCount = 0;
        lcd.setCursor(0, 0);
        lcd.print("3.Limit =       ");

        val = true;
        break;
      }

      if (key == 'D')//D tuşuna basıldığı zaman DolumVal4 e değer girilecek ve epprom a yazılacak
      {
        inputValD = true;
        inputValA = inputValB = inputValC = false;
        DolumVal4 = 0;
        displaySea = "";
        for (int i = 0; i < 0; i++)
          tempVal4[i] = 0;
        valCount = 0;
        lcd.setCursor(0, 0);
        lcd.print("4.Limit =       ");

        val = true;
        break;
      }

      if (val == true && key == '#')// girilen değeri double a dönüştür
      {
        lcd.setCursor(0, 0);
        lcd.print("     HAZIR       ");
        DolumVal1 = atof(tempVal);
        DolumVal2 = atof(tempVal2);
        DolumVal3 = atof(tempVal3);
        DolumVal4 = atof(tempVal4);

        EEPROM.put(0, DolumVal1); // 0. byte a DolumVal1  değeri yazılıyor
        EEPROM.put(10, DolumVal2); // 10. byte a DolumVal2  değeri yazılıyor
        EEPROM.put(20, DolumVal3); // 20. byte a DolumVal3  değeri yazılıyor
        EEPROM.put(30, DolumVal4); // 30. byte a DolumVal4  değeri yazılıyor
        /*
          EEPROM_writeDouble(0, DolumVal1); //eep nin 0 byte ına 1. dolum değeri yazılıyor
          EEPROM_writeDouble(10, DolumVal2); //eep nin 2 byte ına 2. dolum değeri yazılıyor
          EEPROM_writeDouble(20, DolumVal3); //eep nin 4 byte ına 3. dolum değeri yazılıyor
          EEPROM_writeDouble(30, DolumVal4); //eep nin 6 byte ına 4. dolum değeri yazılıyor
        */
        val = false;
        inputValA = inputValB = inputValC = inputValD = false;        
        break;
      }

      if (val == false && key == '#' && DolumVal1 != NULL) // çalışma anında Start vermek için kullanılacak;
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("KAPASiTE1=");
        lcd.setCursor(9, 0);
        lcd.print(DolumVal1, 3);
        digitalWrite(RELAY1, HIGH);
        digitalWrite(RELAY2, HIGH);
        digitalWrite(RELAY3, HIGH);
        digitalWrite(RELAY4, HIGH);
        //AllStart();
        runTime = true;

      }
      ////////////////////////////////////////////////////////////////////////////
      if (val == true && inputValA == true)
      {
        tempVal[valCount] = key;
        valCount++;
        displaySea += key;
        lcd.setCursor(9, 0);
        lcd.print(displaySea);
      }

      if (val == true && inputValB == true)
      {
        tempVal2[valCount] = key;
        valCount++;
        displaySea += key;
        lcd.setCursor(9, 0);
        lcd.print(displaySea);
      }

      if (val == true && inputValC == true)
      {
        tempVal3[valCount] = key;
        valCount++;
        displaySea += key;
        lcd.setCursor(9, 0);
        lcd.print(displaySea);
      }

      if (val == true && inputValD == true)
      {
        tempVal4[valCount] = key;
        valCount++;
        displaySea += key;
        lcd.setCursor(9, 0);
        lcd.print(displaySea);
      }
      ///////////////////////////////////////////////////////////////////////
      if (key == '*' && inputValA == true)
      {
        tempVal[valCount - 1] = '.';
        for (int i = 0; i < 8; i++)
        {
          displaySea[i] = tempVal[i];
        }
        lcd.setCursor(9, 0);
        lcd.print(displaySea);
      }

      if (key == '*' && inputValB == true)
      {
        tempVal2[valCount - 1] = '.';
        for (int i = 0; i < 8; i++)
        {
          displaySea[i] = tempVal2[i];
        }
        lcd.setCursor(9, 0);
        lcd.print(displaySea);
      }

      if (key == '*' && inputValC == true)
      {
        tempVal3[valCount - 1] = '.';
        for (int i = 0; i < 8; i++)
        {
          displaySea[i] = tempVal3[i];
        }
        lcd.setCursor(9, 0);
        lcd.print(displaySea);
      }

      if (key == '*' && inputValD == true)
      {
        tempVal4[valCount - 1] = '.';
        for (int i = 0; i < 8; i++)
        {
          displaySea[i] = tempVal4[i];
        }
        lcd.setCursor(9, 0);
        lcd.print(displaySea);
      }
      break;

    case RELEASED:
      if (key == '*')
      {
      }
      break;

    case HOLD:
      if (key == '*')
      { //////////* tuşuna basılı tutulursa bütün role leri stop konumuna al///////////
        if (runTime == true)
        {
          AllStop();
          lcd.clear();
          lcd.print("     S T O P    ");
        }
      }

      if (key == 'A')
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 1. ROLE DEGERi ");
        lcd.setCursor(5, 1);
        lcd.print(EEPROM.get(0, DolumVal1));
      }

      if (key == 'B')
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 2. ROLE DEGERi ");
        lcd.setCursor(5, 1);
        lcd.print(EEPROM.get(10, DolumVal2));
      }

      if (key == 'C')
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 3. ROLE DEGERi ");
        lcd.setCursor(5, 1);
        lcd.print(EEPROM.get(20, DolumVal3));
      }

      if (key == 'D')
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" 4. ROLE DEGERi ");
        lcd.setCursor(5, 1);
        lcd.print(EEPROM.get(30, DolumVal4));
      }

      break;
  }
}



void serialFunc()
{

  if (stringComplete)
  {
    //lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print(Kilo);
    //lcd.setCursor(11,1);
    lcd.print("Kg  ");
    inputString = "";
    sending = "";
    stringComplete = false;
    sayac = 0;

    if (runTime == true && Kilo > DolumVal1) // kiloya ulaşana kadar role çekili kalmasını sağlayan fonksiyon
    {
      digitalWrite (RELAY1, HIGH);
    }

    else if (runTime == true && Kilo > DolumVal2) // kiloya ulaşana kadar role çekili kalmasını sağlayan fonksiyon
    {
      digitalWrite (RELAY2, HIGH);
    }

    else if (runTime == true && Kilo > DolumVal3) // kiloya ulaşana kadar role çekili kalmasını sağlayan fonksiyon
    {
      digitalWrite (RELAY3, HIGH);
    }

    else if (runTime == true && Kilo > DolumVal4) // kiloya ulaşana kadar role çekili kalmasını sağlayan fonksiyon
    {
      digitalWrite (RELAY4, HIGH);
    }
  }

  while (mySerial.available())
  {
    char inChar = (char)mySerial.read();
    // add it to the inputString:
    inputString += inChar;
    temp[sayac] = inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    sayac++;

    if (inChar == '\n') {
      sending = inputString.substring(8, 15);  // seri port dan bilgi okuyan string kesilerek sadece değerleri alan yeni bir stringe atanıyor
      //daraSending = inputString.substring(17,23);

      for (int i = 0; i < 8; i++)
      {
        kgDonustur[i] = sending[i];  //değerler double türüne dönüştürmek için önce bir char dizisine teker teker aktarılıyor
        //tareDonustur[i] = daraSending[i];
      }
      Kilo = atof(kgDonustur);      // char dizisi double türüne dönüştürülüyor...
      //Dara = atof(tareDonustur);

      stringComplete = true;
    }
  }
}

void AllStart()
{
  //lcd.setCursor(0,0);
  lcd.print("role deneme");
  delay(1000);
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
  digitalWrite(RELAY3, LOW);
  digitalWrite(RELAY4, LOW);
}

void AllStop()
{
  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
}
/*
  void EEPROM_writeDouble(int ee, double value) // Dolum değeri için belirlenen doble değeri EEprom a yazan fonksiyon
  {
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
       EEPROM.write(ee++, *p++);
  }

  double EEPROM_readDouble(int ee) // Dolum değeri için belirlenen doble değeri EEprom dan okuyan fonksiyon
  {
   double value = 0.0;
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
        p++ = EEPROM.read(ee++);
   return value;
  }

*/
