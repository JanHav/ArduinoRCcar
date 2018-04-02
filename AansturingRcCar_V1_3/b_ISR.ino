///ISR (interrupt service routines) RC control

void rc_begin()
{
  RcBediening_startPulse = micros();
  detachInterrupt(1);
  attachInterrupt(1, rc_end, FALLING);
}

void rc_end()
{
  pulse_val = micros()-RcBediening_startPulse;
  detachInterrupt(1);
  attachInterrupt(1, rc_begin, RISING);
}
