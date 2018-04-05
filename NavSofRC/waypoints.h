

// type definition for waypoint data
typedef struct {  float Xas; 
                  float Yas;
               }XasYas; 

// The following defines are the hardcoded waypoints for the missions
// There are various paths, they can be switched on and of by setting the #if directives to #if 1 (deze array gebruiken we) of #if 0 (deze array gebruiken we niet)
#if 0 // Seefeldsee #2
XasYas wps[] = 
{      
1.00,2.00,
1.00,5.00,
3.00,5.00,
1.00,2.00,
};
#endif


#if 0 // Seefeldsee #1
XasYas wps[] = 
{      
1.00,2.00,
1.00,5.00,
3.00,5.00,
1.00,2.00,/* Seeeldsee Home*/
};
#endif

#if 1 // Schuetzensee
XasYas wps[] = 
{      
3000,800,
3000,5200,
6000,5200,
600,800,
};
#endif  

#if 0 // one long leg with approx 350m in length North to south and back
XasYas wps[] = 
{                
1200,800,
1800,800,
2400,1000,
3000,1400,
3000,2000,
3000,3000,
2800,3700,
2600,4600,
2000,5200,
1400,4900,
800,4600,
600,4300,
600,3000,
600,2000,
600,800,
};
#endif

#if 0 // Test_LowSpeed.kml triangular course with one 90ÔøΩ turn
XasYas wps[] = 
{                
800.00,400.00,
1000.00,1000.00,
900.00,2000.00,
};
#endif

#if 0 //Welke waypoint lijst we gebruiken hangt af van welke ik aan zet met de #if 1 
XasYas wps[] = 
{                
1.00,11.00,
11.00,11.00,
11.00,1.00,
1.00,2.00,
};
#endif


#if 0 // T2010_03_25.kml zigzag north/south
XasYas wps[] = 
{      
1.00,2.00,
1.00,5.00,
3.00,5.00,
1.00,2.00,
};
#endif


#if 0 // T10_9.kml  very long course to test the endurance
XasYas wps[] = 
{                
1.00,2.00,
1.00,5.00,
3.00,5.00,
1.00,2.00,
};
#endif

