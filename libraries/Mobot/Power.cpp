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
#include "Power.h"

/**
 * Instancje do wykorzystania we własnym kodzie.
 */
MotorsCurrent MotorsCMeasure;
Measure AdcMeasure;
Battery VBat;

/**
 * Klasa Measure zajmuje się ustawieniem oraz
 * sterowaniem ADC mikrokontrolera. Wartości
 * odczytywane z ADC mogą służyć potem do pomiarów
 * poboru prądu silników napędowych oraz pomiaru
 * napięcia akumulatorów platformy.
 *
 */
Measure::Measure(void)
{
    cli();
    DDRF = 0x00;
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz
    ADMUX |= (1 << REFS0) | 0b01001; // Set ADC reference to AVCC
    ADCSRA |= (1 << ADFR);  // Set ADC to Free-Running Mode
    ADCSRA |= (1 << ADEN);  // Enable ADC
    ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt
    sei();
    ADCSRA &= ~(1 << ADSC);  // Stop A2D Conversions
}

/**
 * Uruchamia konwersję przez ADC. Konstruktor
 * przygotowuje tylko mikrokontroler do ciągłego
 * pomiaru wartości z ADC. Ta metoda uruchamia cały proces
 *
 */
void Measure::startConversion(void)
{
    ADCSRA|=(1<<ADSC);
};

/**
 * Włącza ADC w mikrokontrolerze
 *
 */
void Measure::turnOn(void)
{
    ADCSRA |= (1<<ADEN);
}

/**
 * Wyłącza używanie ADC w mikrokontrolerze
 *
 */
void Measure::turnOff(void)
{
    ADCSRA &= ~(1<<ADEN);
}

/**
 * Klasa służy do zwracania aktualnej
 * wartości prądu pobieranego przez silniki.
 * Wartość mierzona jest w miliamperach.
 */
MotorsCurrent::MotorsCurrent(void)
{
    currentMotor1 = 0;
    currentMotor2 = 0;
};

/**
 * Ustawia wartość prądu dla odpowiedniego zespołu silników.
 *
 * @param uint8_t MEASURE_1 (dla lewego zespołu), MEASURE_2 (dla prawego zespołu)
 * @param uint16_t wartość w miliamperach
 */
void MotorsCurrent::setCurrent(uint8_t motor, uint16_t value)
{
    if (motor == 1)
        currentMotor1 = value;
    else
        currentMotor2 = value;
};

/**
 * Zwraca aktualną wartość poboru prądu przez podany
 * w parametrze zespół silników.
 *
 * @param uint8_t MEASURE_1 (dla lewego zespołu), MEASURE_2 (dla prawego zespołu)
 * @return uint16_t wartość prądu w mA
 *
 */
uint16_t MotorsCurrent::getCurrent(uint8_t motor)
{
    if (motor == 1)
        return currentMotor1;
    else
        return currentMotor2;

};

/**
 * Klasa zwraca wartość pomiaru napięcia baterii akumulatorów
 * zasilających platformę.
 *
 */
Battery::Battery(void)
{

};


/**
 * Ustawia napięcie baterii w woltach.
 *
 * @param float - napięcie baterii
 */
void Battery::setVoltage(float voltage)
{
    vbatt = voltage;
}

/**
 * Zwraca wartość wpisaną za pomocą
 * setVoltage()
 *
 * @return float - aktualne napięcie baterii
 *
 */
float Battery::getVoltage(void)
{
    return vbatt;
}

/*
 * Obsługa przerwania generowanego przez ADC
 * po dokonaniu konwersji. Służy do pomiaru
 * prądu silników oraz napięcia baterii akumulatorów
 * zasilających platformę. Sumuje tyle pomiarów z kanału ile jest
 * zapisane w ADC_MSR_COUNTER, po czym dzieli przez ilość
 * pomiarów uśredniając fluktuacje oraz inne zakłócenia.
 * Na końcu przełącza na kolejny kanał ADC i zaczyna pomiary na nowo
 * aż licznik osiągnie zero.
 *
 */
ISR(ADC_vect)
{
    switch(ADC_channel)
    {
        case MEASURE_1:
        {
            measure += ADC;
            if (--measurescnt == 0)
            {
                AdcMeasure.turnOff();
                ADC_channel = MEASURE_2;
                measurescnt = ADC_MSR_COUNTER;

                MotorsCMeasure.setCurrent(MEASURE_1, 3.68*(measure >> 5));

                ADMUX = (1<<REFS0)| 0b01101;
                measure = 0;
                AdcMeasure.turnOn();
                AdcMeasure.startConversion();
            }
            break;
        }
        case MEASURE_2:
        {
            measure += ADC;
            if (--measurescnt == 0)
            {
                AdcMeasure.turnOff();
                ADC_channel = MEASURE_3;
                measurescnt = ADC_MSR_COUNTER;

                MotorsCMeasure.setCurrent(MEASURE_2, 3.68*(measure >> 5));

                ADMUX = (1<<REFS0) | 0b00111 ;
                measure = 0;
                AdcMeasure.turnOn();
                AdcMeasure.startConversion();
            }
            break;
        }
        case MEASURE_3:
        {
            measure += ADC;
            if (--measurescnt == 0)
            {
                AdcMeasure.turnOff();
                ADC_channel = MEASURE_1;
                measurescnt = ADC_MSR_COUNTER;

                measure = (measure>>5);

                //(12V * R16/R17+R16 / 1024) * 10
                VBat.setVoltage((float)(measure*3.8) * 0.011526);

                ADMUX = (1<<REFS0) | 0b01001 ;
                measure = 0;
                AdcMeasure.turnOn();
                AdcMeasure.startConversion();

            }
        }
    }
}