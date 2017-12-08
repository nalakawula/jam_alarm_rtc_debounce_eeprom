#include <EEPROM.h>
#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <RBD_Timer.h>  // https://github.com/alextaujenis/RBD_Timer
#include <RBD_Button.h> // https://github.com/alextaujenis/RBD_Button

LiquidCrystal_I2C lcd(0x27, 16, 2); // Display  I2C 20 x 4
DS1307 RTC;

RBD::Button bawah(12);
RBD::Button kanan(11);
RBD::Button tengah(10);
RBD::Button atas(9);
RBD::Button kiri(8);

#define LED 13
#define buzzer 2

int jam;
int menit;
int tahun;
int bulan;
int tanggal;
int menu = 0;
int setAlarm = 0;

uint8_t jamAlarm ;
uint8_t menitAlarm;
int alamatJam = 0;
int alamatMenit = 1;

// jam pasir
byte jam_pasir[] = {
  B11111,
  B10001,
  B01010,
  B00100,
  B01010,
  B10001,
  B11111,
  B11111
};

// bel
byte bel[] = {
  B00100,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B00000,
  B00100
};

// panah sebelah kiri
byte panah_ki[] = {
  B01000,
  B01100,
  B01110,
  B01111,
  B01111,
  B01110,
  B01100,
  B01000
};

// panah sebelah kanan
byte panah_ka[] = {
  B00010,
  B00110,
  B01110,
  B11110,
  B11110,
  B01110,
  B00110,
  B00010
};

// Custom Char buat loading
byte p1[8] = {
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10,
  0x10
};

byte p2[8] = {
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18,
  0x18
};

byte p3[8] = {
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C,
  0x1C
};

byte p4[8] = {
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E,
  0x1E
};

byte p5[8] = {
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F,
  0x1F
};


void setup()
{

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, jam_pasir);
  lcd.createChar(1, bel);
  lcd.createChar(2, panah_ki);
  lcd.createChar(3, panah_ka);

  lcd.createChar(4, p1);
  lcd.createChar(5, p2);
  lcd.createChar(6, p3);
  lcd.createChar(7, p4);
  lcd.createChar(8, p5);

  lcd.setCursor(0, 0);
  loading(0);
  loading(1);
  lcd.clear();

  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT); // Set buzzer as an output

  jamAlarm = EEPROM.read(alamatJam);
  menitAlarm = EEPROM.read(alamatMenit);

  printAlarmOff();
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // Set the date and time at compile time
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  // RTC.adjust(DateTime(__DATE__, __TIME__)); //removing "//" to adjust the time
  // The default display shows the date and time
  int menu = 0;
}

void loop()
{

  if (tengah.onPressed()) // cek tombol tengah, jika ditekan, menu index naik
  {
    menu = menu + 1;
  }

  if (atas.onPressed()) // cek tombol atas, jika ditekan, set alarm
  {

    DisplaySetJamAlarm();
    DisplaySetMenitAlarm();
    DisplayAkhirAlarm();
  }

  if (menu == 0)
  {
    DisplayWaktu();
    Alarm(); // Alarm control

  }
  
  if (menu == 1)
  {
    DisplaySetJam();
  }
  
  if (menu == 2)
  {
    DisplaySetMenit();
  }
  
  if (menu == 3)
  {
    DisplaySetTahun();
  }
  
  if (menu == 4)
  {
    DisplaySetBulan();
  }
  
  if (menu == 5)
  {
    DisplaySetTanggal();
  }
  
  if (menu == 6)
  {
    SimpanPengaturanWaktu();
    delay(500);
    menu = 0;
    lcd.clear();
  }
  delay(100);
}

void DisplayWaktu ()
{
  DateTime now = RTC.now();

  // HARI
  char DOW[][10] = {"Ming", "Sen", "Sel", "Rab", "Kam", "Jum", "Sab"};
  lcd.setCursor(0, 0);
  //lcd.print(DOW[now.dayOfTheWeek()]);
  lcd.print(DOW[now.dayOfWeek()]); lcd.print(",");

  // TANGGAL
  lcd.setCursor(6, 0);
  if (now.day() <= 9)
  {
    lcd.print("0");
  }
  
  lcd.print(now.day(), DEC);
  tanggal = now.day();
  lcd.print("/");
  
  if (now.month() <= 9)
  {
    lcd.print("0");
  }
  lcd.print(now.month(), DEC);
  bulan = now.month();
  lcd.print("/");
  lcd.print(now.year(), DEC);
  tahun = now.year();


  // JAM
  lcd.setCursor(0, 1);
  if (now.hour() <= 9)
  {
    lcd.print("0");
  }
  lcd.print(now.hour(), DEC);
  jam = now.hour();
  lcd.print(":");
  
  if (now.minute() <= 9)
  {
    lcd.print("0");
  }
  lcd.print(now.minute(), DEC);
  menit = now.minute();
  lcd.print(":");
  
  if (now.second() <= 9)
  {
    lcd.print("0");
  }
  lcd.print(now.second(), DEC);

}

void DisplaySetJam()
{
  lcd.clear();
  DateTime now = RTC.now();
  if (kanan.isPressed())
  {
    if (jam == 23)
    {
      jam = 0;
    }
    else
    {
      jam = jam + 1;
    }
  }
  if (kiri.isPressed())
  {
    if (jam == 0)
    {
      jam = 23;
    }
    else
    {
      jam = jam - 1;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Set Jam:");
  lcd.setCursor(0, 1);
  lcd.print(jam, DEC);
  delay(200);
}

void DisplaySetMenit()
{
  // Setting the minutes
  lcd.clear();
  if (kanan.isPressed())
  {
    if (menit == 59)
    {
      menit = 0;
    }
    else
    {
      menit = menit + 1;
    }
  }
  if (kiri.isPressed())
  {
    if (menit == 0)
    {
      menit = 59;
    }
    else
    {
      menit = menit - 1;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Set Menit:");
  lcd.setCursor(0, 1);
  lcd.print(menit, DEC);
  delay(200);
}

void DisplaySetTahun()
{
  // setting the year
  lcd.clear();
  if (kanan.isPressed())
  {
    tahun = tahun + 1;
  }
  if (kiri.isPressed())
  {
    tahun = tahun - 1;
  }
  lcd.setCursor(0, 0);
  lcd.print("Set Tahun:");
  lcd.setCursor(0, 1);
  lcd.print(tahun, DEC);
  delay(200);
}

void DisplaySetBulan()
{
  // Setting the month
  lcd.clear();
  if (kanan.isPressed())
  {
    if (bulan == 12)
    {
      bulan = 1;
    }
    else
    {
      bulan = bulan + 1;
    }
  }
  if (kiri.isPressed())
  {
    if (bulan == 1)
    {
      bulan = 12;
    }
    else
    {
      bulan = bulan - 1;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Set Bulan:");
  lcd.setCursor(0, 1);
  lcd.print(bulan, DEC);
  delay(200);
}

void DisplaySetTanggal()
{
  // Setting the day
  lcd.clear();
  if (kanan.isPressed())
  {
    if (tanggal == 31)
    {
      tanggal = 1;
    }
    else
    {
      tanggal = tanggal + 1;
    }
  }
  if (kiri.isPressed())
  {
    if (tanggal == 1)
    {
      tanggal = 31;
    }
    else
    {
      tanggal = tanggal - 1;
    }
  }
  lcd.setCursor(0, 0);
  lcd.print("Set tanggal:");
  lcd.setCursor(0, 1);
  lcd.print(tanggal, DEC);
  delay(200);
}

void SimpanPengaturanWaktu()
{
  // Variable saving
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Saving..........");
  loading(1);

  RTC.adjust(DateTime(tahun, bulan, tanggal, jam, menit, 0));
  delay(200);
}

void DisplaySetJamAlarm()// Setting the alarm minutes
{
  while (tengah.isReleased()) {

    lcd.clear();

    if (kanan.isPressed())
    {
      if (jamAlarm == 23)
      {
        jamAlarm = 0;
      }
      else
      {
        jamAlarm = jamAlarm + 1;
      }
    }
    if (kiri.isPressed())
    {
      if (jamAlarm == 0)
      {
        jamAlarm = 23;
      }
      else
      {
        jamAlarm = jamAlarm - 1;
      }
    }
    lcd.setCursor(0, 0);
    lcd.print("Atur jamAlarm:");
    //lcd.setCursor(0, 1);

    lcd.setCursor(0, 1);
    lcd.write(2); lcd.print(jamAlarm, DEC);
    delay(200);
  }
  delay(200);
}

void DisplaySetMenitAlarm()// Setting the alarm minutes
{
  while (tengah.isReleased()) {

    lcd.clear();
    if (kanan.isPressed())
    {
      if (menitAlarm == 59)
      {
        menitAlarm = 0;
      }
      else
      {
        menitAlarm = menitAlarm + 1;
      }
    }
    if (kiri.isPressed())
    {
      if (menitAlarm == 0)
      {
        menitAlarm = 59;
      }
      else
      {
        menitAlarm = menitAlarm - 1;
      }
    }
    lcd.setCursor(0, 0);
    lcd.print("Atur Menit Alarm");
    lcd.setCursor(0, 1);
    lcd.print(jamAlarm, DEC); lcd.print(":"); lcd.print(menitAlarm, DEC); lcd.write(3);
    delay(200);
  }
  delay(200);
}

void DisplayAkhirAlarm()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alarm diatur:");
  lcd.setCursor(5, 1);
  lcd.print(jamAlarm, DEC);
  lcd.print(":");
  lcd.print(menitAlarm, DEC);
  EEPROM.write(alamatJam, jamAlarm);
  EEPROM.write(alamatMenit, menitAlarm);
  delay(3000);
  lcd.setCursor(0, 0); lcd.print("Saving..........");
  loading(1);
  lcd.clear();
}

void printAlarmOn() {
  lcd.setCursor(9, 1);

  if (jamAlarm <= 9)
  {
    lcd.print("0");
  }
  lcd.print(jamAlarm, DEC);

  lcd.print(":");
  if (menitAlarm <= 9)
  {
    lcd.print("0");
  }
  lcd.print(menitAlarm, DEC);

  lcd.setCursor(15, 1);
  lcd.write(1);

}

void printAlarmOff() {
  lcd.setCursor(0, 1);
}

void Alarm() {
  if (bawah.onPressed())
  {
    setAlarm = setAlarm + 1;
    lcd.clear();
  }
  if (setAlarm == 0)
  {
    printAlarmOff();
    DisplayWaktu();
    noTone (buzzer);
    digitalWrite(LED, LOW);
  }
  if (setAlarm == 1)
  {

    printAlarmOn();

    DateTime now = RTC.now();
    if ( now.hour() == jamAlarm && now.minute() == menitAlarm )
    {
      lcd.noBacklight();
      DateTime now = RTC.now();
      digitalWrite(LED, HIGH);
      tone(buzzer, 880); //play the note "A5" (LA5)
      delay (300);
      tone(buzzer, 698); //play the note "F6" (FA5)
      lcd.backlight();
    }
    else {
      noTone (buzzer);
      digitalWrite(LED, LOW);
    }

  }
  if (setAlarm == 2)
  {
    setAlarm = 0;
  }
  delay(200);
}

void loading(int x) {
  lcd.print("                ");

  for (int i = 0; i < 16; i++)
  {

    for (int j = 4; j < 9; j++)
    {
      lcd.setCursor(i, x);
      lcd.write(j);
      delay(10);
    }


  }


}


