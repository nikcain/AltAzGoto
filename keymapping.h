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
#define key_left 68
#define key_right 67
#define key_up 9
#define key_down 7
#define key_volup 70
#define key_voldown 21
#define key_calibrate 25 // - eq key, used as calibrate key
// used for back #define key_rpt 13
#define key_power 69

#endif // ELEGOO

#ifdef WOKWI


#define key_0 104
#define key_1 48
#define key_2 24
#define key_3 122
#define key_4 16
#define key_5 56
#define key_6 90
#define key_7 66
#define key_8 74
#define key_9 82

#define key_menu 226
#define key_play 168
#define key_back 194
#define key_left 224
#define key_right 144
#define key_up 2
#define key_down 152
#define key_volup 70
#define key_voldown 21
#define key_calibrate 25
#define key_power 162
#define key_cancel 176

#endif // WOKWI

#endif // keymapping_h