///ISR (interrupt service routines) RC control

void rc_begin()                                         //We schrijven weg in een variabele op welk moment we een stijgende flank zien,
{                                                       //vervolgens zeggen we dat de interrupt pin nu moet getriggerd zijn door een dalende flank
  RcBediening_startPulse = micros();
  detachInterrupt(1);
  attachInterrupt(1, rc_end, FALLING);
}

void rc_end()                                           //In een variabele schrijven we het verschil tussen het huidige tijdstip waarop we een dalende flank
{                                                       //zien en het tijdstip waarop we een stijgende flank zagen = deze tijd is de tijdON van ons PWM signaal
  pulse_val = micros()-RcBediening_startPulse;          //Vervolgens zeggen we dat de interrupt pin nu terug moet triggeren op een stijgende flank
  detachInterrupt(1);
  attachInterrupt(1, rc_begin, RISING);
}
