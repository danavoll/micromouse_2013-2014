int delays[] = {0,56568, 23432, 17780, 15158, 13354, 12072, 11102,
                10334, 9706};
int delayCount = 0;
int delaySize = sizeof(delays)/sizeof(delays[0]); //size of the array
boolean steppersEnabled = false;

void setup()
{
  cli();//stop interrupts

  //set timer1 interrupt at 2MHz
  TCCR1A = 0;// set entire TCCR0A register to 0
  TCCR1B = 0;// same for TCCR0B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR1A = 56568;// compare register (16Mhz/8/rate)
  // turn on CTC mode
  TCCR1A |= (1 << WGM11);
  // Set CS01 and CS00 bits for 8 prescaler
  TCCR1B |= (1 << CS21);   
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei();//allow interrupts
  
}

void loop()
{
  
}

ISR(TIMER1_COMPA_vect){  //change the 0 to 1 for timer1 and 2 for timer2
   if (!stepperEnabled) return;  // check if the steppers have been enabled
   STEP_LEFT=0;
   delay_us(5);
   STEP_LEFT=1;
   delayCount++;
   if(delayCount > delaySize - 1) //avoid going over the max speed
     delayCount = delaySize - 1;
   OCRIA = delays[delayCount];
 }
