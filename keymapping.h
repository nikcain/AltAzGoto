#ifndef keymapping_h
#define keymapping_h

#define ELEGOO 1

#ifdef ELEGOO
#define key_0 22
#define key_1 12
#define key_2 24
#define key_3 94
#define key_4 8
#define key_5 28
#define key_6 90
#define key_7 66
#define key_8 82
#define key_9 74

#define key_menu 71
#define key_play 64
#define key_back 13
#define key_left 67
#define key_right 68
#define key_up 9
#define key_down 7
#define key_volup 70
#define key_voldown 21
#define key_eq 25
// used for back #define key_rpt 13
#define key_power 69

#endif // ELEGOO

#ifdef WOKWI
  for (int i = 0; i < 250; i++) remotekey[i]=-1;
  remotekey[48]=1;
  remotekey[24]=2;
  remotekey[122]=3;
  remotekey[16]=4;
  remotekey[56]=5;
  remotekey[90]=6;
  remotekey[66]=7;
  remotekey[74]=8;
  remotekey[82]=9;
  remotekey[82]=9;
  remotekey[104]=0;
  remotekey[176]=10; // cancel
  remotekey[2]=11; // plus
  remotekey[152]=12; // minus
  remotekey[224]=13; // rewind
  remotekey[144]=14; // FF
  remotekey[34]=15; // test
  remotekey[168]=16; // play
  remotekey[194]=17; // back
  remotekey[226]=18; // menu (set time)
  remotekey[162]=19; //power
#endif // WOKWI

#endif // keymapping_h