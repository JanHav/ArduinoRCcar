///Functies voor Arduino control

/*****
Doel: De RC car vooruit laten rijden
Parameters:
void
Return value:
geen
*****/

void VooruitRijden()
{
  pinMode(motor,OUTPUT);
  currentMillis = millis();
   if (currentMillis-previousMillis>=1 && GewensteDutyCycle > 42.00)     //Met deze waarden kan je spelen als de belasting op de RC car stijgt of je hem trager/ sneller wilt laten rijden 42.01
    {
      GewensteDutyCycle = GewensteDutyCycle-0.01;
      previousMillis = millis();
    }
  pwmWrite(motor, GewensteDutyCycle);                                   //De motor aansturen met de gewenste dutycycle 
}

/*****
Doel: De RC car naar links laten rijden
Parameters:
void
Return value:
geen
*****/
void LinksStuur(double angle)
{
  pinMode(servoMotor,OUTPUT);
  double abhoek = abs(angle);
  //GewensteDutyCycleServo = 44;    //47
  if (0 < abhoek <= 90)
  {
  GewensteDutyCycleServo = 35 + (abhoek*9/10);
  }
  else
  {
    GewensteDutyCycleServo = 44;
  }
  pwmWrite(servoMotor, GewensteDutyCycleServo);                         //De motor aansturen met de gewenste dutycycle
}

/*****
Doel: Stuur RC car in neutraalpositie brengen
Parameters:
void
Return value:
geen
*****/
void NeutraalStuur()
{
  pinMode(servoMotor,OUTPUT);
  GewensteDutyCycleServo = 35;    //35
  pwmWrite(servoMotor, GewensteDutyCycleServo);                     //De motor aansturen met de gewenste dutycycle
}

/*****
Doel: De RC car naar rechts laten rijden
Parameters:
void
Return value:
geen
*****/
void RechtsStuur(double angle)
{
  pinMode(servoMotor,OUTPUT);
  double abhoek = abs(angle);
  //GewensteDutyCycleServo = 25;//22
  if (0 < abhoek <= 90)
  {
  GewensteDutyCycleServo = 35 - (abhoek*9/10);
  }
  else
  {
    GewensteDutyCycleServo = 26;
  }
  pwmWrite(servoMotor, GewensteDutyCycleServo);                     //De motor aansturen met de gewenste dutycycle
}

/*****
Doel: Automatische noodstop als er een object binnen een bepaalde afstand van de RC car komt
Parameters:
void
Return value:
geen
*****/
void autoNoodstop()
{                                     //Code hieronder dient om signaal van de Parallax ultrasone sensor binnen te lezen
  pinMode(pingPin, OUTPUT);           //Set pin to OUTPUT
  digitalWrite(pingPin, LOW);         //Ensure pin is low
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);        //Start ranging
  delayMicroseconds(5);               //with 5 microsecond burst
  digitalWrite(pingPin, LOW);         //End ranging
  pinMode(pingPin, INPUT);            //Set pin to INPUT
  duration = pulseIn(pingPin, HIGH);  //Read echo pulse
  afstand = duration/29/2;            //Convert to centimeters
                                      //The speed of sound is 340 m/s or 29 microseconds per centimeter.
                                      //The ping travels out and back, so to find the distance of the object we
                                      //take half of the distance travelled.
  if (afstand <= 80)                  //Als er een object op een afstand gelijk aan of kleiner dan 80 cm wordt, willen we de RC car afremmen
  {
    GewensteDutyCycle = 26;           //Deze dutycycle zorgt ervoor dat de motor snel wordt afgeremd
  }
}

