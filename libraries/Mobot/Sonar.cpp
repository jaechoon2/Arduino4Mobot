/* ------------------------------------------------------*
*                                                        *
* Copyright (c) 2013, Jarek Zok <jarek.zok@fwioo.pl>     *
* All rights reserved.                                   *
*                                                        *
* This program is free software; you can redistribute it *
* and/or modify it under the terms of the GNU General    *
* Public License as published by the Free Software       *
* Foundation; either version 2 of the License, or        *
* (at your option) any later version.                    *
*                                                        *
* This program is distributed in the hope that it will   *
* be useful, but WITHOUT ANY WARRANTY; without even the  *
* implied warranty of MERCHANTABILITY or FITNESS FOR A   *
* PARTICULAR PURPOSE.  See the GNU General Public        *
* License for more details.                              *
*                                                        *
* You should have received a copy of the GNU General     *
* Public License along with this program; if not, write  *
* to the Free Software Foundation, Inc.,                 *
* 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA *
*                                                        *
* Licence can be viewed at                               *
* http://www.fsf.org/licenses/gpl.txt                    *
*                                                        *
*                                                        *
**********************************************************/

#include "Arduino.h"
#include "Arduino.h"
#include "Wire.h"
#include "Sonar.h"

Sonar Sonar1(1);
Sonar Sonar2(2);
Sonar Sonar3(3);

Sonar::Sonar(uint8_t address)
{
    addr = address;
    Wire.begin();
}

uint16_t Sonar::getDistance()
{
  uint8_t i = 0;
  buffer[0] = SONAR_CMD;
  buffer[1] = SONAR_CMD_MEASURE;
  _write(2);

  if (status == 0)
  {
    delay(20);
    buffer[0] = SONAR_READ_DISTANCE;
    _write(1);
    delay(1);

    if (status == 0)
    {
        if (Wire.requestFrom(addr, 2) > 1)
        {
            while(Wire.available())
            {
                buffer[i++] = Wire.read();
                if (i == 3)
                    break;
            }
            return (buffer[0] | (buffer[1] << 8));
        }
        return 1;
    }
  }
  return 0;
}

void Sonar::setSens(int8_t sens)
{
    sens = sens > 20 ? 20 : sens < -20 ? -20 : sens;
    buffer[0] = SONAR_CMD;
    buffer[1] = SONAR_CMD_SENSITI;
    buffer[2] = sens;
    _write(3);
}

void Sonar::setRange(uint16_t sens)
{
    sens = sens > 4000 ? 4000 : sens < 100 ? 100 : sens;
    buffer[0] = SONAR_CMD;
    buffer[1] = SONAR_CMD_RANGE;
    buffer[2] = sens & 0x0000FFFF;
    buffer[3] = sens >> 8;
    _write(4);
}

void Sonar::setThreshold(uint16_t sens)
{
    sens = sens > 4000 ? 4000 : sens < 60 ? 60 : sens;
    buffer[0] = SONAR_CMD;
    buffer[1] = SONAR_CMD_THRESHOLD;
    buffer[2] = sens & 0x0000FFFF;
    buffer[3] = sens >> 8;
    _write(4);
}

void Sonar::setPulseCount(uint8_t cnt)
{
    cnt = cnt > 32 ? 32 : cnt < 1 ? 1 : cnt;
    buffer[0] = SONAR_CMD;
    buffer[1] = SONAR_CMD_PULSECNT;
    buffer[2] = cnt;
    _write(3);
}

void Sonar::_write(uint8_t len)
{
    Wire.beginTransmission(addr);
    Wire.write(buffer, len);
    status = Wire.endTransmission();
}