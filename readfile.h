#include "PF.h"
#include "PetitSerial.h"

PetitSerial PS;
// Use PetitSerial instead of Serial.
//#define Serial PS

// The SD chip select pin is currently defined as 10
// in pffArduino.h.  Edit pffArduino.h to change the CS pin.

FATFS fs;     /* File system object */

bool FindCelestialObjectRecord(String id, CelestialGotoObject* obj)
{

  obj->isPlanet = false;
  obj->isValid = true;

  // atria (southern star)
  /*
  obj->RA_hour = 16;
  obj->RA_minute = 51;
  obj->DEC_hour = -69;
  obj->DEC_minute = 01;
  return true;
*/

  // sirius
  
   // degs declination = -16.7424;
   // degs right_ascension = 6.768 * 15;

  obj->RA_hour = 6;
  obj->RA_minute = 46;
  obj->DEC_hour = -16;
  obj->DEC_minute = 43;
  return true;

    uint8_t buf[32];
  Serial.println("finding");
  // Initialize SD and file system.
  FRESULT r = PF.begin(&fs);
  Serial.println(r);
  
  Serial.println("opening");
  // Open test file.
  if (PF.open("1111.TXT")) return false;
  
  Serial.println("reading");
  // Dump test file to Serial.
  while (1) {
    UINT nr;
    if (PF.readFile(buf, sizeof(buf), &nr)) return false;
    if (nr == 0) break;
    Serial.write(buf, nr);
  }
  return false;
  /*
  // Initialize the SD.
  if (!sd.begin(SD_CONFIG)) {
    sd.initErrorHalt(&Serial);
    obj->isValid = false;
    return false;
  }

  // Create the file.
  file = sd.open(id + String(".txt"));
  if (!file) {
    obj->isValid = false;
    return false;
  }

  char line[40];
  if (file.fgets(line, sizeof(line)) > 0) obj->name = line; else {
    obj->isValid = false;
    return false;
  }
  
  obj->isValid = true;
  obj->RA_hour = 1;
  obj->RA_minute = 33.51;
  obj->DEC_hour = 30;
  obj->DEC_minute = 39;
  file.close();
  return true;
  */
}
