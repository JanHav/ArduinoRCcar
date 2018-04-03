/*****
 Doel: M.b.v. dit programma kan men een Himoto RC car aansturen met een Arduino UNO of MEGA (getest op een UNO)
 Nodig: 
 #include <PWM.h>   //Dit zorgt ervoor dat we een PWM met de UNO kunnen uitsturen met de gewenste frequentie en dutycycle

 Pinbezetting Arduino UNO
 9 en 10: Pinnen die we gebruiken voor een PWM signaal uit te sturen
 5V referentie UNO verbinden met Logical converter
 3,3V referentie UNO verbinden met Logical converter
 Grounds verbinden met elkaar tussen Himoto RC car en UNO

 Laatste aanpassing:
 2018/04/01: Basisversie RC control via UNO
 2018/04/02: Reverse engineering dutycycle aandrijfmotor voorruit rijden 
             Reverse engineering dutycycle servormotor links/ rechts
 2018/04/03: Comments bij code plaatsen, afregelen dutycycle waarden om de ESC aan te sturen
*****/

#include <PWM.h>

///RC control
int motor = 9;                                //Pin 9 UNO voor PWM aandrijfmotor (aandrijving)
int servoMotor = 10;                          //Pin 10 UNO voor PWM servomotor (sturen)
int32_t frequency = 100;                      //Frequentie (in Hz, we willen een PWM met een frequentie van 100 Hz
float GewensteDutyCycle = 43;                 //Dutycycle (38/255) = 15,06%, kan verhoogd worden als er meer apparatuur op het RC car platform geplaatst wordt
float GewensteDutyCycleServo = 35;            //Dutycycle voor de stuurservo in de rechtuitstand
volatile long RcBediening_startPulse;         //Bevat de waarde van micros() op het moment dat een stijgende flank op de interrupt wordt gezien
volatile unsigned int pulse_val;              //Bevat de tijdON van de PWM uitgestuurd door de RC zender
unsigned long previousMillis = 0;             //Variabelen die we gebruiken om het afbouwen van de dutycycle van de aansturing van de aandrijfmotor
unsigned long currentMillis = 0;              //constant te laten verlopen (het afbouwen mag langer maar niet korter duren, te kort = elektromotor heeft te weinig koppel


void setup()
{
  ///Seriële monitor
  Serial.begin(115200);                                     //De baudrate van de seriële monitor leggen we vast op 115200                                     
  
  ///RC control
  InitTimersSafe();                                         //Nodig om timers hun frequentie aan te passen (buiten timer 0, deze gebruikt Arduino onder meer voor de millis() functie
  pinMode(3, INPUT);                                        //Op pin 3 zit onze interrupt voor besturing over te nemen via de RC zender
  attachInterrupt(1, rc_begin, RISING);                     //De interrupt moet initieel kijken naar een stijgende flank, als deze gezien wordt gaan we naar de ISR (interrupt service routine)
  SetPinFrequencySafe(motor, frequency);                    //De juiste pinnen voorzien van de juiste frequentie voor het PWM signaal
  SetPinFrequencySafe(servoMotor, frequency);
}


void loop()
{
  ///RC control
  if (pulse_val > 600 && pulse_val <2400)                           //Als pulse_val een waarde hiertussen bevat wil dat zeggen dat de RC zender aanstaat m.a.w.
  {                                                                 //dat de zender op channel 3 van de RC ontvanger een PWM plaatst.
    pinMode(motor,INPUT);                                           //De motorpin en servomotorpin als INPUT benoemen zodat ze niet het signaal van de RC zender beïnvloeden
    pinMode(servoMotor, INPUT);                                     //Opdat de auto na afzetten RC zender terug bestuurd zou kunnen worden door de UNO dienen we de variabele die
    --pulse_val;                                                    //de laatste tijdON bevat te verkleinen we in het else gedeelte (=UNO aansturing) geraken
    GewensteDutyCycle = 43;                                         //Variabelen krijgen deze waarden bij overgang controle RC zender naar UNO                                         
    GewensteDutyCycleServo = 35;               
  }                                                                 
  else
  {
    VooruitRijden();                                                //Vooruit rijden
    //LinksStuur();                                                 //Links sturen
    //NeutraalStuur();                                              //Stuur in neutraalpositie plaatsen
    //RechtsStuur();                                                //Rechts sturen
  } 
  Serial.println(GewensteDutyCycle);                                //Weergave variabele in seriële monitor voor debugging
}



