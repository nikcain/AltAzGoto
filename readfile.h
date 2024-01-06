#include "PF.h"
#include "PetitSerial.h"

PetitSerial PS;
// Use PetitSerial instead of Serial.
#define Serial PS

// The SD chip select pin is currently defined as 10
// in pffArduino.h.  Edit pffArduino.h to change the CS pin.

FATFS fs;     /* File system object */

bool FindCelestialObjectRecord(int id, CelestialGotoObject* obj)
{
  // all files should be four lines:
  // NGC number
  // RA
  // DEC
  // Name
  char i[2];
  i[1] = 0;
  i[0] = (int)(id/500)+ 65;
  String filename = /*String(i) +*/ "C/" + String(id) + ".TXT";
  Serial.println(filename);

  if (id > 9000) obj->isPlanet = true;

  char buf[32];
  memset(buf, 0, 32);
  // Initialize SD and file system.
  FRESULT r = PF.begin(&fs);
  if (PF.open("/C/1111.TXT")) return false; //filename.c_str())) return false;
  
  UINT nr;
  if (PF.readFile(buf, sizeof(buf), &nr)) return false;
  if (nr == 0) return false;
  memset(buf, 0, 32);
  if (PF.readFile(buf, sizeof(buf), &nr)) return false;
  if (nr == 0) return false;
  if (!obj->isPlanet) obj->rightascension = String(buf).toDouble();
  memset(buf, 0, 32);
  if (PF.readFile(buf, sizeof(buf), &nr)) return false;
  if (nr == 0) return false;
  if (!obj->isPlanet) obj->declination = String(buf).toDouble();
  memset(buf, 0, 32);
  if (PF.readFile(buf, sizeof(buf), &nr)) return false;
  if (nr == 0) return false;
  obj->name = buf;
  
  obj->isValid = true;
  return true;
}
