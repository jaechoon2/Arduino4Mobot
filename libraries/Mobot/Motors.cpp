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
#include "Motors.h"

Motors LeftMotors(M_LEFT);
Motors RightMotors(M_RIGHT);

/**
 *  Klasa Motors do obsługi silników napędzających platformę.
 *  Konstruktor klasy przyjmuje parametr M_LEFT lub M_RIGHT
 *  zdefiniowany w nagłówku biblioteki Motors. Zależnie od niego
 *  klasa będzie sterować lewym lub prawym zespołem silników.
 *
 *  @param uint8_t side M_LEFT lub M_RIGHT
 *
 */
Motors::Motors(uint8_t side)
{
  motors = side;
  pinMode(M_PORTS[6], OUTPUT); //_SLEEP
  digitalWrite(M_PORTS[6], LOW);
  pinMode(M_PORTS[motors * 3], OUTPUT); //D1
  pinMode(M_PORTS[7], OUTPUT); //_ENABLE
  pinMode(M_PORTS[motors * 3 + 1], OUTPUT); //IN1
  pinMode(M_PORTS[motors * 3 + 2], OUTPUT); //IN2

  TCCR3A |= (1<<COM3A1)|(1<<COM3B1)|(1<<COM3A0)|(1<<COM3B0)|(1<<WGM30);
  TCCR3B |= (1<<CS30); //8bit Phase Correct PWM inverting mode

}

/**
 * Metoda setDirection wybiera kierunek obrotu silników.
 * Przyjmuje jeden parametr o warości MDIR_FORWARD lub
 * MDIR_BACKWARD.
 *
 * @param bool direction
 */
void Motors::setDirection(bool direction)
{
    mdir = direction;
    if (direction == MDIR_FORWARD)
    {
        digitalWrite(M_PORTS[motors * 3 + 1], HIGH);
        digitalWrite(M_PORTS[motors * 3 + 2], LOW);
    } else
    {
        digitalWrite(M_PORTS[motors * 3 + 1], LOW);
        digitalWrite(M_PORTS[motors * 3 + 2], HIGH);
    }
}

/**
 * Ustawia prędkość silników w zakresie od 0 do 255.
 * Wartość ta zmienia wypełnienie PWM podawanego na
 * wejście H mostków. Można podać pełen zakres, jednak
 * ze względu na fizyczne ograniczenia, dolna wartość
 * wypełnienia zaczyna się w okolicach 40-50.
 *
 * @param uint8_t speed - 0..255
 */
void Motors::setSpeed(uint8_t speed)
{
    analogWrite(M_PORTS[motors * 3], speed);
}

/**
 * Metoda zatrzymuje zespół silników napędowych
 * zależnie od wybranej w konstruktorze
 * strony platformy
 *
 */
void Motors::stop()
{
    digitalWrite(M_PORTS[motors * 3 + 1], HIGH);
    digitalWrite(M_PORTS[motors * 3 + 2], HIGH);
}

/**
 * Zatrzymuje oba zespoły silników nakazując
 * H mostkom zwarcie wyprowadzeń silników do masy.
 * Robi to dla obu zespołów jednocześnie. Nie ważne
 * więc, w której instancji klasy wywołamy metodę.
 */
void Motors::fullStop()
{
    digitalWrite(M_PORTS[1], HIGH);
    digitalWrite(M_PORTS[2], HIGH);
    digitalWrite(M_PORTS[4], HIGH);
    digitalWrite(M_PORTS[5], HIGH);
}

/**
 * Metoda uruchamia silniki.
 *
 */
void Motors::start()
{
    setDirection(mdir);
    digitalWrite(M_PORTS[6], HIGH);
}
