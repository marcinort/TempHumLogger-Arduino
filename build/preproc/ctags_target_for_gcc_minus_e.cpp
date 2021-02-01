# 1 "d:\\Marcin\\Programowanie\\Arduino\\Projects\\TempHumLogger\\TempHumLogger.ino"
# 2 "d:\\Marcin\\Programowanie\\Arduino\\Projects\\TempHumLogger\\TempHumLogger.ino" 2

# 4 "d:\\Marcin\\Programowanie\\Arduino\\Projects\\TempHumLogger\\TempHumLogger.ino" 2
# 5 "d:\\Marcin\\Programowanie\\Arduino\\Projects\\TempHumLogger\\TempHumLogger.ino" 2

# 7 "d:\\Marcin\\Programowanie\\Arduino\\Projects\\TempHumLogger\\TempHumLogger.ino" 2
# 8 "d:\\Marcin\\Programowanie\\Arduino\\Projects\\TempHumLogger\\TempHumLogger.ino" 2

# 10 "d:\\Marcin\\Programowanie\\Arduino\\Projects\\TempHumLogger\\TempHumLogger.ino" 2
# 11 "d:\\Marcin\\Programowanie\\Arduino\\Projects\\TempHumLogger\\TempHumLogger.ino" 2

# 13 "d:\\Marcin\\Programowanie\\Arduino\\Projects\\TempHumLogger\\TempHumLogger.ino" 2




float t = 0, h = 0, DStemp = 0;
int year = 0, id = 0;
byte t0 = 0, t1 = 0, deltat = 0;
byte month = 0, day = 0, hour = 0, minute = 0, second = 0;
byte interval = 10; // interval time between measurements

DHT dht;
DS1307 clock;
RTCDateTime dt;
File results;

OneWire onewire(5 /* Sensor DS18B20 data connection - PIN 5*/);
DS18B20 sensors(&onewire);
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte degree[8] = {
    4,
    10,
    17,
    10,
    4,
    0,
    0,
    0};

byte temp[8] = {
    4,
    6,
    4,
    6,
    4,
    4,
    14,
    14};

byte hum[8] = {
    0,
    4,
    4,
    14,
    14,
    23,
    23,
    14};

byte address[8] = {0x28, 0x9A, 0xBA, 0xA7, 0xC, 0x0, 0x0, 0x6}; // DS18B20 Address

void setup()
{

    //Serial.begin(9600);
    //Serial.println("Serial initialized");

    // LCD initialize
    lcd.begin();
    lcd.clear();
    lcd.createChar(0, degree);
    lcd.createChar(1, hum);
    lcd.createChar(2, temp);
    lcd.setCursor(0, 0);
    lcd.print("Initialize...");
    //Serial.println("LCD initialized");

    //DHT Initialize
    dht.setup(2 /* Sensor DHT22 data connection - PIN 2*/);
    //Serial.println("DHT22 initialized");

    // DS18B20 initialize
    sensors.begin();
    sensors.request(address);
    //Serial.println("DS18B20 initialized");

    // DS1307 initialize
    clock.begin();
    // If date not set
    if (!clock.isReady())
    {
        // Set sketch compiling time
        clock.setDateTime("Feb  1 2021", "22:33:04");
    }

    //SD card initialize
    SD.begin(10);
    //Serial.println("SD Card initialized");

    // File header
    DateRead();

    //Serial.println("Data to save: ");
    //Serial.println(String("Date(YYYY-MM-DD): ") + year + "-" + month + "-" + day);
    //Serial.println(String("Time(HH:MM:SS): ") + hour + ":" + minute + ":" + second);
    //Serial.println("ID,YYYY,MM,DD,HH,MM,Temp[C],RH[%],DSTemp[C]");

    results = SD.open("results.txt", (O_READ | O_WRITE | O_CREAT | O_APPEND));
    results.println(String("Date(YYYY-MM-DD): ") + year + "-" + month + "-" + day);
    results.println(String("Time(HH:MM:SS): ") + hour + ":" + minute + ":" + second);
    results.println("ID,YYYY,MM,DD,HH,MM,Temp[C],RH[%],DSTemp[C]");
    results.close();
    delay(300);

    // First measurement
    Measurement();
    LoggerSave(year, month, day, hour, minute, t, h, DStemp);
    t0 = minute;
    //Serial.println("First Measurement done");

    lcd.clear();
}

void loop()
{
    Measurement();

    DateRead();

    // Sending data via UART interface
    /*

    if (isnan(t) || isnan(h))

    {

        Serial.println("Failed to read from DHT");

    }

    else

    {

        Serial.print("ID: ");

        Serial.print(id);

        Serial.print("\tMeasurment: ");

        Serial.print(dht.getStatusString());

        Serial.print("\t");

        Serial.print("Humidity: ");

        Serial.print(h);

        Serial.print("% Temperature: ");

        Serial.print(t);

        Serial.print("*C ");

        Serial.print("DS Temperature: ");

        Serial.print(DStemp);

        Serial.print("*C");

        Serial.print("\tt0: ");

        Serial.print(t0);

        Serial.print(" t1: ");

        Serial.print(t1);

        Serial.print(" deltat: ");

        Serial.println(deltat);

    }

*/
# 161 "d:\\Marcin\\Programowanie\\Arduino\\Projects\\TempHumLogger\\TempHumLogger.ino"
    // LCD dispaly

    lcd.setCursor(0, 0);
    lcd.write(byte(2));
    lcd.print(" ");
    lcd.print(t, 1);
    lcd.write(byte(0));
    lcd.print("C");
    lcd.setCursor(9, 0);
    lcd.write(byte(1));
    lcd.print(" ");
    lcd.print(h, 1);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("D ");
    lcd.print(DStemp, 1);
    lcd.write(byte(0));
    lcd.print("C   ");
    if (hour <= 9)
    {
        lcd.print(" ");
        lcd.print(hour);
    }
    else
    {
        lcd.print(hour);
    }
    lcd.print(":");
    if (minute <= 9)
    {
        lcd.print("0");
        lcd.print(minute);
    }
    else
    {
        lcd.print(minute);
    }

    t1 = minute;
    deltat = ((t1 - t0)>0?(t1 - t0):-(t1 - t0));

    if (deltat == interval || deltat == (60 - interval))
    {
        LoggerSave(year, month, day, hour, minute, t, h, DStemp);
    }
}

float Measurement()
{
    t = dht.getTemperature(); // for DHT_markruys.h
    h = dht.getHumidity(); // for DHT_markruys.h
    if (sensors.available())
    {
        DStemp = sensors.readTemperature(address);
        sensors.request(address);
    }
    return t, h, DStemp;
}

byte DateRead()
{
    dt = clock.getDateTime();
    year = dt.year;
    month = dt.month;
    day = dt.day;
    hour = dt.hour;
    minute = dt.minute;
    second = dt.second;
    return year, month, day, hour, minute, second;
}
void LoggerSave(int year, byte month, byte day, byte hour, byte minute, float t, float h, float DStemp)
{

    id++;
    lcd.setCursor(11, 1);
    lcd.print(" SAVE");
    Serial.println("LoggerSave: ");
    Serial.println(id + String(",") + year + "," + month + "," + day + "," + hour + "," + minute + "," + t + +"," + h + "," + DStemp);
    results = SD.open("results.txt", (O_READ | O_WRITE | O_CREAT | O_APPEND));
    results.println(id + String(",") + year + "," + month + "," + day + "," + hour + "," + minute + "," + t + +"," + h + "," + DStemp);
    results.close();
    delay(1000); // tested with 300ms
    //Serial.println("Data saved");
    t0 = t1;
}
