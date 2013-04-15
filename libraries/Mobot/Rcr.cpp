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
#include "Rcr.h"

Rcr RadioUsart(&Serial1);

Rcr::Rcr(HardwareSerial *port)
{
    serial = port;
    serial->flush();
    serial->begin(57600);
}

void Rcr::setSpeed(uint8_t speed)
{
    speed = speed > 56 ? 56 : speed < 1 ? 1 : speed;
    _write(RCR_CMD_SPEED, speed);
}


void Rcr::setChannel(uint8_t channel)
{
    channel = channel > 9 ? 9 : channel;
    _write(RCR_CMD_CHANNEL, channel);
}

void Rcr::setPower(uint8_t power)
{
    power = power > 7 ? 7 : power;
    _write(RCR_CMD_POWER, power);
}

void Rcr::setSens(uint8_t sens)
{
    sens = sens > 3 ? 3 : sens;
    _write(RCR_CMD_SENSITI, sens);
}

void Rcr::setBuffer(uint8_t size)
{
    size = size > 128 ? 128 : size < 1 ? 1 : size;
    _write(RCR_CMD_BUFFER, size);
}

uint16_t Rcr::_write(uint8_t cmd, uint8_t param)
{
    uint16_t *r = (uint16_t*) ret;
    cmds[3] = cmd;
    cmds[4] = param;
    serial->write(cmds, 5);
    serial->readBytesUntil('K', ret, 2);
    return r[0];
}
