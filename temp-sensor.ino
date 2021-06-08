#include <spark-dallas-temperature.h>
#include <OneWire.h>
#include <math.h>

OneWire oneWire(A1);
DallasTemperature ds(&oneWire);

double targetTempC = 65.0;

double firstTempC;
double secondTempC;
double rate;
double timeRemaining;
void setup()
{
    Serial.begin(9600);
	Serial.println("Temperature Readings!");
	pinMode(D7, OUTPUT);
    ds.begin();
}
void loop()
{
    //Get first temp reading
    ds.requestTemperatures();
    firstTempC = ds.getTempCByIndex(0);
    
    if (isnan(firstTempC))
    {
		return;
	}
    
    if(firstTempC <= -50 || firstTempC >= 120)
    {
        Serial.println("MAX TEMP REACHED");
        return;
    }
    
    //Print first temp reading
    Serial.print("Temp: ");
    Serial.print(firstTempC);
    Serial.println(" C");
    
    //Wait one second
    delay(1000);
    
    noTone(D0);
    
    digitalWrite(D7, LOW);
    
    //Get second temp reading
    ds.requestTemperatures();
    secondTempC = ds.getTempCByIndex(0);
    
    if (isnan(secondTempC))
    {
		return;
	}
    
    //Get rate of temperature change
    rate = secondTempC - firstTempC;
    
    //Get time remaining till target is reached
    timeRemaining = (targetTempC - secondTempC)/rate;
    
    //Print time remaining
    Serial.print("Time remaining: ");
    Serial.print(timeRemaining);
    Serial.println(" seconds");
    
    //Print rate
    Serial.print("Rate: ");
    Serial.print(rate);
    Serial.println(" C/s");
    
    //Check if target is reached
    if(secondTempC >= targetTempC || rate + secondTempC >= targetTempC || (timeRemaining <= 3 && !(timeRemaining <= 0)))
    {
        Serial.println("TARGET ACHIEVED!");
        digitalWrite(D7, HIGH);
        tone(D0, 1000);
        return;
    }
    
    Serial.println();
}
