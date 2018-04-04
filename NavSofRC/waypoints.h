

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

#if 0 // Schuetzensee
XasYas wps[] = 
{      
1.00,2.00,
1.00,5.00,
3.00,5.00,
1.00,2.00,
};
#endif  

#if 0 // one long leg with approx 350m in length North to south and back
XasYas wps[] = 
{                
1.00,2.00,
1.00,5.00,
3.00,5.00,
1.00,2.00,
};
#endif

#if 0 // Test_LowSpeed.kml triangular course with one 90ÔøΩ turn
XasYas wps[] = 
{                
1.00,2.00,
1.00,5.00,
3.00,5.00,
1.00,2.00,
};
#endif

#if 1 //Welke waypoint lijst we gebruiken hangt af van welke ik aan zet met de #if 1 
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

