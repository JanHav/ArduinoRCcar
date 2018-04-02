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
   if (currentMillis-previousMillis>=1 &&GewensteDutyCycle > 41.01)
    {
      GewensteDutyCycle = GewensteDutyCycle-0.0001;
      previousMillis = millis();
    }
  pwmWrite(motor, GewensteDutyCycle);                     //De motor aansturen met de gewenste dutycycle 
}

/*****
Doel: De RC car naar links laten rijden
Parameters:
void
Return value:
geen
*****/
void LinksStuur()
{
  pinMode(servoMotor,OUTPUT);
  GewensteDutyCycleServo = 44;    //47
  pwmWrite(servoMotor, GewensteDutyCycleServo);                     //De motor aansturen met de gewenste dutycycle
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
  GewensteDutyCycleServo = 32;    //35
  pwmWrite(servoMotor, GewensteDutyCycleServo);                     //De motor aansturen met de gewenste dutycycle
}

/*****
Doel: De RC car naar rechts laten rijden
Parameters:
void
Return value:
geen
*****/
void RechtsStuur()
{
  pinMode(servoMotor,OUTPUT);
  GewensteDutyCycleServo = 22;
  pwmWrite(servoMotor, GewensteDutyCycleServo);                     //De motor aansturen met de gewenste dutycycle
}
