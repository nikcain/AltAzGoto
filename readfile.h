// the sdfat library pushes the program size beyond the arduino Uno limit 
// (probably ok for Mega), while petitfs will fit, and if the pffconf.h
// is edited to restrict use to just reading files (which is all we need)
// then it's even smaller:
// #define	_USE_DIR	0	/* Enable pf_opendir() and pf_readdir() function */
// #define	_USE_LSEEK	0	/* Enable pf_lseek() function */
// #define	_USE_WRITE	0	/* Enable pf_write() function */
//
// #define _FS_FAT12	1	/* Enable FAT12 */
// #define _FS_FAT16	1	/* Enable FAT16 */
// #define _FS_FAT32	0	/* Enable FAT32 */
//
// the worst part is finding a 2GB micro SD card, and then being
// restricted to 512 files per folder! So 2000's!
// plus I'm not a wokwi member, so won't work here anyway, but I'll
// keep this file up to date.

#ifndef wokwi

#include "PF.h"
#include "PetitSerial.h"

PetitSerial PS;
// Use PetitSerial instead of Serial.
#define Serial PS

// The SD chip select pin is currently defined as 10
// in pffArduino.h.  Edit pffArduino.h to change the CS pin.

FATFS fs;     /* File system object */

bool InitSDCard()
{
  // Initialize SD and file system.
  return (FR_OK == PF.begin(&fs));
}

bool FindCelestialObjectRecord(CelestialGotoObject* obj)
{
  // all files should be four lines:
  // NGC number
  // RA (not for planets)
  // DEC (not for planets)
  // Name

  obj->isPlanet = (obj->id > 9990);

  char i[2];
  i[1] = 0;
  i[0] = (int)(obj->id/500)+ 65;
  String filename = String(i) + "/" + String(obj->id) + ".TXT";

  char buf[64];

  if (PF.open(filename.c_str())) { 
    Serial.println(filename);
    Serial.println("can't open");
    return false; 
  }
  
  UINT nr;
  String txt = "";

  memset(buf, 0, 64);
  if (PF.readFile(buf, sizeof(buf), &nr)) return false;
  txt = String(buf);

  int idx = txt.indexOf("\r\n");
  int idx2 = txt.indexOf("\r\n", idx+1);
  String val;
  if (!obj->isPlanet) 
  {
    val = txt.substring(idx+1, idx2);
    obj->rightascension = val.toDouble();
  }
  idx = txt.indexOf("\r\n", idx2+1);
  if (!obj->isPlanet) 
  {
    val = txt.substring(idx2+1, idx);
    obj->declination = val.toDouble();
  }
  idx2 = txt.indexOf("\r\n", idx+1); 
  obj->name = txt.substring(idx+2, idx2);
  obj->isValid = true;
  return true;
}

#else
// stub
bool FindCelestialObjectRecord(CelestialGotoObject* obj) 
{

  obj->isPlanet = false;
  obj->isValid = true;

  // you can't be sirius!!
  obj->name = "Sirius";
  obj->rightascension = 6.7525;  // 6h45m9s
  obj->declination = -16.72525;  // -16deg43'31''
  
  return true;
}
#endif

