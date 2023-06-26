// include libraries
#include <LiquidCrystal.h>
#include <IRremote.h>
// Define sensor pin for IR remote
const int RECV_PIN = 10;
int RoomTemp = 24; // Default Room Temp
// Pins used for 3 LEDs 
const int WLED = 9;
const int RLED = 8;
const int BLED = 7;
// Button pin
const int buttonPin = 6;
int buttonState =0;
// define integer to remember toggle state
int togglestate = 0;
//define IR receiver and results objects
IRrecv irrecv(RECV_PIN);
decode_results results;
const int tempPin = A0; //Define a constant to identify the temp pin
const int numberOfReadings = 10; //number of samples to average
float ReadTemperature(int pin)
/* this function reads analogue input passed through parameter 'pin'
and calculate the corresponding temp value in Â°c */
{
	float analogueInAverage = 0;
	for (int i = 0; i < numberOfReadings; i++)
	{
		analogueInAverage += analogRead(pin);
		delay(20); // waits 20 ms before taking another reading
	}
	analogueInAverage /= (float)numberOfReadings;
	/* above statement is the same as;
	analogueInAverage = (analogueInAverage / numberOfReadings) */
	float inVoltage = 5.0 * analogueInAverage / 1023.0; // equation (2)
	float temperature = (inVoltage - 0.5) / 0.01; // equation (1)
	return temperature;
}
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup() 
{
	// enable IR receiver
	irrecv.enableIRIn();
	// set up the LCD's number of columns and rows:
	lcd.begin(16, 2);
	// print startup message
	lcd.print("System On, Hello");
  	delay(750);
  	lcd.clear(); /* clears text from the lcd each time this
    				function is used to allow new text to be
                    printed to the screen on each loop */
	//set outputs
	pinMode(BLED, OUTPUT);
	pinMode(RLED, OUTPUT);
	pinMode(WLED, OUTPUT);
	Serial.begin(9600);
  
  	// Button INPUT (for manual switch on) 
  	pinMode(buttonPin, INPUT);
}
void loop()
{
	if (irrecv.decode(&results)) 
	{ // decodes the hexidecimal code sent by IR remote
        Serial.println(results.value, HEX); // added to check the remote code
		switch (results.value) 
		{
			// increase temp
		case 0xFD807F:// IR remote button to increase temp
		
			RoomTemp++;
			//lcd.begin(16, 2);
			lcd.print("Increase Temp");
          	delay(500);
          	lcd.clear();
			lcd.setCursor(0, 1);
			lcd.print(RoomTemp); //shows new room temp
			break;
			// decrease temp
		case 0xFD906F:// IR remote button to decrease temp
			RoomTemp--;
			//lcd.begin(16, 2);
			lcd.print("Decrease Temp");
          	delay(500);
          	lcd.clear();
			lcd.setCursor(0, 1);
			lcd.print(RoomTemp); //shows new room temp 
			break;
		case 0xFDA05F:// Button to toggle WLED on/off
				
			lcd.print("Lights On");
          	delay(500);
          	lcd.clear();
			if (togglestate == 0) 
			{
				digitalWrite(WLED, HIGH); // Turns LED off
				delay(1000);
              	togglestate = 1;
			}
			else
			{
				digitalWrite(WLED, LOW); // Turns LED on
				togglestate = 0;
			}
			break;
		}
		irrecv.resume();
	}
	if (ReadTemperature(tempPin) >= RoomTemp) // checks if temp higher than 24c
	{
		digitalWrite(BLED, HIGH);  // switch blue LED on
		digitalWrite(RLED, LOW);
      	lcd.setCursor(0,1);
      	lcd.print(tempPin);
		lcd.print("(C): Fan on");
      	delay(500);
      	lcd.clear();
	}
	if (ReadTemperature(tempPin) <= RoomTemp) // checks if temp lower than 24c
	{
		digitalWrite(BLED, LOW);
		digitalWrite(RLED, HIGH);  // switch red LED on
      	lcd.setCursor(0,1);
      	lcd.print(tempPin);
		lcd.print("(C): Heater on");
      	delay(500);
      	lcd.clear();
	}
  	  buttonState = digitalRead(buttonPin);
  	
  if (buttonState == HIGH) { 
    
    digitalWrite(WLED, LOW); 
  }
  
  else {
    
    digitalWrite(WLED, HIGH); 
  }
}
