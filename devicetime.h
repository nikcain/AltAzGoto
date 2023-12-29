#ifndef devicetime_h
#define devicetime_h

unsigned long timeoffset = 0;

void setDeviceTime(unsigned long _h, unsigned long _m, unsigned long _s)
{
  _h+=24;
  _m+=60;
  _s+=60;
  timeoffset =  ((_h % 24 * 60 * 60 * 1000) +
                (_m % 60 * 60 * 1000) +
                (_s % 60 * 1000)) - millis();
}

int getHour() 
{
  return ((((millis() + timeoffset) / 1000) / 60) / 60) % 24;

  //unsigned long currentMillis = millis() + timeoffset;
  //unsigned long seconds = currentMillis / 1000;
  //unsigned long minutes = seconds / 60;
  //unsigned long hours = minutes / 60;

  //return hours %= 24;
}

int getMinute()
{
  return (((millis() + timeoffset) / 1000) / 60) % 60;

  //unsigned long currentMillis = millis() + timeoffset;
  //unsigned long seconds = currentMillis / 1000;
  //unsigned long minutes = seconds / 60;

  //return minutes %= 60;
}

int getSeconds()
{
  return ((millis() + timeoffset) / 1000) % 60;

  //unsigned long currentMillis = millis() + timeoffset;
  //unsigned long seconds = currentMillis / 1000;

  //return seconds %= 60;
}

String getTimeString()
{
  char buf1[10];
  sprintf(buf1, "%02d:%02d:%02d",  getHour(), getMinute(), getSeconds()); 

  return String(buf1);
}

#endif //devicetime_h