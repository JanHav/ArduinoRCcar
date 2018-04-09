/*****
Doel: De afstand van een punt t.o.v. een rechte bepalen.
Parameters:
float x1      float y1    float x2      float y2
float xPunt     float yPunt  
Return value:
float
*****/
float AfstandPuntRechte(float x1, float y1, float x2, float y2, float xPunt, float yPunt)
{
  float afstand;
  float teller;
  float noemer;
  teller = abs((((y2-y1)/(x2-x1))*xPunt)-(1*yPunt)-((((y2-y1)/(x2-x1))*x1)-y1));
  noemer = sqrt((((y2-y1)/(x2-x1))*((y2-y1)/(x2-x1)))+1);
  afstand = teller/noemer; 
  return afstand;
}

/*****
Doel: De rico berekenen van een rechte gevormd door twee punten
Parameters:
float x1      float y1      float x2      floaty2
Return value:
float
*****/
float Rico(float x1, float y1, float x2, float y2)
{
  float richting;
  richting = ((y2-y1)/(x2-x1));
  return richting;
}

/*****
Doel: De afstand van een punt tot een punt bepalen
Parameters:
float x1      float y1      float x2      floaty2
Return value:
float
*****/
float AfstandPuntPunt (float x1, float y1, float x2, float y2)
{
  float afstand;
  afstand = sqrt(((x2-x1)*(x2-x1))+((y2-y1)*(y2-y1)));
  return afstand;
}

/*****
Doel: De hoek bepalen tussen twee rechten die deel uitmaken van een rechthoekige driehoek
Paramters:
float OverstaandRecht     float SchuinRecht
Return value:
float
*****/
double HoekTweeRechten (float OverstaandRecht, float SchuinRecht)
{
  double hoekRad;
  double hoekDeg;
  hoekRad = asin((OverstaandRecht)/(SchuinRecht)); 
  hoekDeg = hoekRad *57,296;                              //57,296 is om rad naar deg om te zetten (1rad = (180/PI) deg = 57,3 deg
  return hoekDeg;
}

/*****
Doel: De hoek bepalen tussen twee rechten die deel uitmaken van een rechthoekige driehoek met behulp van atan
Paramters:
float x1      float y1      float x2      float y2
Return value:
double
*****/
double Hoek( float x1, float y1, float x2, float y2)
{
  double hoekRad;
  double hoekDeg;
  hoekRad = atan((x1 -x2)/(y2-y1));
  hoekDeg = degrees(hoekRad);
  return hoekDeg;
}

