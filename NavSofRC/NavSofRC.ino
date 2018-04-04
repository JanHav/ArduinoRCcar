/* Gebasseerd op software geschreven door Chris Anderson en Jordi Munoz, aangepast door Jan Haverbeke om te gebruiken voor AGV's */
/* Nov/27/2010
/* Version 1.1 */
/* Released under an Apache 2.0 open source license*/
/* Project home page is at carsexplained.com*/

#include "waypoints.h"
#include <math.h>
#include <PWM.h>

///NavSof////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Global variables definition
int waypoints;                      // Waypoint teller 
byte current_wp=0;                  // Bevat de index waarmee we in de array van de waypoints in de header file geraken, bepaald eindpunt trackline
byte previous_wp=0;                 // Bevat de index waarmee we in de array van de waypoints in de header file geraken, bepaald beginpunt trackline
float xhuidig_wp = 0.0;             // Bevat de huidige x-coördinaat van de AGV, in demonstratiesoftware verkrijgen we dit via userinput 
float yhuidig_wp = 0.0;             // Bevat de huidige y-coördinaat van de AGV, in demonstratiesoftware verkrijgen we dit via userinput
float xstart_wp = 0.0;              // Bevat de x-coördinaat begin trackline
float ystart_wp = 0.0;              // Bevat de y-coördinaat begin trackline
float ErrorTrack = 0.0;             // Bevat de afgeweken afstand tot de trackline 
float RicoHuidigeKoers = 0;         // De rico van de lijn waarop de AGV nu zit
float RicoTrackKoers = 0;           // De rico van de lijn die de AGV het best volgt
float AfstandTotWaypoint = 0;       // De afstand tot het volgende waypoint
double TrackHoek = 0;               // De hoek tussen de de lijn waarop de AGV nu zit en de lijn die de AGV moet volgen
// Flag variables
byte jumplock_wp = 0;   //Dit lock moet ervoor zorgen dat in het begin een trackline kan 
                        //gevormd worden door de trackline op te stellen van het huidig punt tot het volgende waypoint
///RCcontrol/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int motor = 9;                                //Pin 9 UNO voor PWM aandrijfmotor (aandrijving)
int servoMotor = 10;                          //Pin 10 UNO voor PWM servomotor (sturen)
int32_t frequency = 100;                      //Frequentie (in Hz, we willen een PWM met een frequentie van 100 Hz
float GewensteDutyCycle = 43;                 //Dutycycle (38/255) = 15,06%, kan verhoogd worden als er meer apparatuur op het RC car platform geplaatst wordt
float GewensteDutyCycleServo = 35;            //Dutycycle voor de stuurservo in de rechtuitstand
volatile long RcBediening_startPulse;         //Bevat de waarde van micros() op het moment dat een stijgende flank op de interrupt wordt gezien
volatile unsigned int pulse_val;              //Bevat de tijdON van de PWM uitgestuurd door de RC zender
unsigned long previousMillis = 0;             //Variabelen die we gebruiken om het afbouwen van de dutycycle van de aansturing van de aandrijfmotor
unsigned long currentMillis = 0;              //constant te laten verlopen (het afbouwen mag langer maar niet korter duren, te kort = elektromotor heeft te weinig koppel
const int pingPin = 7;                        //Pin waarop signaal ultrasone sensor Parallax toekomt
unsigned int duration, afstand;               //Variabelen voor het halen van een afstand uit de Parallax sensor readings

// Arduino Startup
void setup()
{  
///Seriële monitor///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.begin(115200);                                     //De baudrate van de seriële monitor leggen we vast op 115200   
///NavSof////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  waypoints = sizeof(wps) / sizeof(XasYas); //Berekenen van het aantal waypoints in onze huidige array
  Serial.print(waypoints);
///RC control////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  InitTimersSafe();                                         //Nodig om timers hun frequentie aan te passen (buiten timer 0, deze gebruikt Arduino onder meer voor de millis() functie
  pinMode(3, INPUT);                                        //Op pin 3 zit onze interrupt voor besturing over te nemen via de RC zender
  attachInterrupt(1, rc_begin, RISING);                     //De interrupt moet initieel kijken naar een stijgende flank, als deze gezien wordt gaan we naar de ISR (interrupt service routine)
  SetPinFrequencySafe(motor, frequency);                    //De juiste pinnen voorzien van de juiste frequentie voor het PWM signaal
  SetPinFrequencySafe(servoMotor, frequency);
}


// Arduino main loop
void loop()
{
///NavSof////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Huidige x-coordinaat ");  //De gebruiker naar informatie vragen
  while(Serial.available()<2)               //Wachten tot de gebruiker iets typt
  {};
  xhuidig_wp=Serial.parseFloat();           //De data die de gebruiker heeft ingegeven binnenlezen

  Serial.println("Huidige y-coordinaat ");  //De gebruiker naar informatie vragen
  while(Serial.available()<2)               //Wachten tot de gebruiker iets typt
  {};
  yhuidig_wp=Serial.parseFloat();           //De data die de gebruiker heeft ingegeven binnenlezen
    
  if (jumplock_wp == 0x00)                  //Ervoor zorgen dat de eerste trackline berekend kan worden door het startwaypoint te voorzien van de huidige locatie
  {
    xstart_wp = xhuidig_wp;                 //Het allereerste punt kunnen we niet opslaan in een array omdat deze bepaald wordt door de plaats waar de AGV wordt neergezet
    ystart_wp = yhuidig_wp;                 //daarom bevat xstart en ystart initieel de waarde van xhuidig en yhuidig
    jumplock_wp = 0x01;                     //Nadat de xstart_wp en ystart_wp zijn upgedatet met de huidige locatie, 
                                            //mag dit vervolgens niet meer zo gebeuren,daarom dat we ons lock resetten
                                            //nu is het de bedoeling dat xstart_wp en ystart_wp steeds verwijzen naar het laatste eindwaypoint, 
                                            //zodat de trackline correct kan berekend worden
  }

  RicoHuidigeKoers = Rico(xstart_wp, ystart_wp,wps[current_wp].Xas, wps[current_wp].Yas);                                 //Rico huidige koers
  RicoTrackKoers= Rico(xhuidig_wp, yhuidig_wp,wps[current_wp].Xas, wps[current_wp].Yas);                                  //Rico tracklijn 
  if(isinf(RicoHuidigeKoers)== 1  ||  isinf(RicoTrackKoers)==1)
  {
    ErrorTrack = xhuidig_wp - xstart_wp;
  }
  else
  {
    ErrorTrack = AfstandPuntRechte(xstart_wp, ystart_wp,wps[current_wp].Xas, wps[current_wp].Yas, xhuidig_wp, yhuidig_wp);  //In deze variabele wordt de afwijking van de trackline opgeslaan
  }
  AfstandTotWaypoint = AfstandPuntPunt(xhuidig_wp, yhuidig_wp,wps[current_wp].Xas, wps[current_wp].Yas);                  //Afstand tot volgende waypoint
  //TrackHoek = HoekTweeRechten(ErrorTrack, AfstandTotWaypoint);                                                            //Hoek tussen huidige trackline en oorspronkelijke trackline berekenen
  TrackHoek = Hoek(xhuidig_wp, yhuidig_wp,wps[current_wp].Xas, wps[current_wp].Yas); 

  if (AfstandTotWaypoint < 0.4)             //Op het ogenblik dat de AGV dicht genoeg bij het eindwaypoint is, moet de trackline 
  {                                         //gevormd worden door het huidige eindwaypoint en een volgend waypoint
    previous_wp = current_wp;
    current_wp++;
    xstart_wp = wps[previous_wp].Xas;
    ystart_wp = wps [previous_wp].Yas;
  }

  if (current_wp >= waypoints)                //Als de index die in current_wp zit gelijk is aan het totaal aantal waypoints in de array 
  {                                           //dan moet de index terug naar nul gaan zodat we terug een coördinaat uit onze array kunnen halen
    current_wp = 0;                           //Zo kan onze robot continu blijven een route volgen en kunnen we de nauwkeurigheid van een technologie achterhalen
  }

///RCcontrol/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    autoNoodstop();                                                 //autoNoodstop
    VooruitRijden();                                                //Vooruit rijden
    if((isinf(RicoHuidigeKoers)== 1) && abs(ErrorTrack) >= 0.1  && TrackHoek>0)
    {
      LinksStuur();
    }
    else if((isinf(RicoHuidigeKoers)== 1) && abs(ErrorTrack) >= 0.1  && TrackHoek<0)
    {
      RechtsStuur();
    }
    else if((ErrorTrack >=0.10) && (RicoHuidigeKoers < RicoTrackKoers))
    {
      LinksStuur();
    }
    else if((ErrorTrack >=0.10) && (RicoHuidigeKoers > RicoTrackKoers))
    {
      RechtsStuur();
    }
    else
    {
      NeutraalStuur();
    }
  } 
  Serial.println(GewensteDutyCycle);                                //Weergave variabele in seriële monitor voor debugging
  Serial.println(afstand); 
  //De onderstaande printstatements zijn voor debugging/controle en visualisatie
    Serial.print("Huidige trackPoint x-coördinaat ");
    Serial.println(wps[0].Xas);
    Serial.print("Huidige trackPoint y-coördinaat ");
    Serial.println(wps[0].Yas);             //Array aanspreken uit header file die coördinaten waypoints bevat
    Serial.print("Huidige x-start waypoint ");
    Serial.println(xstart_wp);
    Serial.print("Huidige y-start waypoint ");
    Serial.println(ystart_wp);
    Serial.print("De error op de track is ");
    Serial.println(ErrorTrack);
    Serial.print("Rico huidige koers ");
    Serial.println(RicoHuidigeKoers);
    Serial.print("Rico trackline ");
    Serial.println(RicoTrackKoers);
    Serial.print("Afstand tot waypoint ");
    Serial.println(AfstandTotWaypoint);
    Serial.print("Huidig waypoint ");
    Serial.print("Trackhoek ");
    Serial.println(TrackHoek);
    Serial.println(current_wp);
    Serial.print("Begin ");
    Serial.println(xstart_wp);  

} // end loop ()

