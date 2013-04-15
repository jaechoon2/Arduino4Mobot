#!/usr/bin/env python

#* ------------------------------------------------------ *
#*                                                        *
#* Copyright (c) 2013, Jarek Zok <jarek.zok@fwioo.pl>     *
#* All rights reserved.                                   *
#*                                                        *
#* This program is free software; you can redistribute it *
#* and/or modify it under the terms of the GNU General    *
#* Public License as published by the Free Software       *
#* Foundation; either version 2 of the License, or        *
#* (at your option) any later version.                    *
#*                                                        *
#* This program is distributed in the hope that it will   *
#* be useful, but WITHOUT ANY WARRANTY; without even the  *
#* implied warranty of MERCHANTABILITY or FITNESS FOR A   *
#* PARTICULAR PURPOSE.  See the GNU General Public        *
#* License for more details.                              *
#*                                                        *
#* You should have received a copy of the GNU General     *
#* Public License along with this program; if not, write  *
#* to the Free Software Foundation, Inc.,                 *
#* 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA *
#*                                                        *
#* Licence can be viewed at                               *
#* http://www.fsf.org/licenses/gpl.txt                    *
#*                                                        *
#*                                                        *
#**********************************************************


import sys, os, argparse
import re

sys.path.append('odsreader')

from ODSReader import ODSReader


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='ODS to Arduino pins converter.')
    parser.add_argument('-i', action='store', dest="input_file", metavar='input-odf-file', required=True, help='Input ODS file')
    parser.add_argument('-o', action='store', dest="output_file", metavar='generated-header-file', type=argparse.FileType('wt'), default='arduino_pins.h', help='Output C++ header file for arduino pins.')
    parser.add_argument('-s', action='store', dest="sheet_name", required=True, help='Name of Sheet in ODS to read pins definition from.')
    parser.add_argument('-m', action='store', dest="device", required=True, help='Name of microcontroler the header is generated (example: ATmega128).')


    ns = parser.parse_args()

    old = os.dup(1)
    os.close(1)
    sys.stdout = ns.output_file

    cpa = re.compile(r'ADC([0-9]+)')
    cpt = re.compile(r'TIMER([0-9]+[A|B|C])')
    doc = ODSReader(ns.input_file)
    table = doc.getSheet(ns.sheet_name)
    ports = {}
    spi = []
    analogin = {}
    timers = {}
    pwm = []
    pins = []
    for r in table[1:]:
        if r[0] == '---':
            break
        pin = {'number' : int(r[0]), 'port' : r[1][1]+r[1][2], 'function' : r[2], 'comment' : r[3]}
        if not ports.has_key(r[1][1]):
            ports[r[1][1]] = []
        ports[r[1][1]].append(pin)
        pins.append(pin)

        if pin['function'] in ('SS', 'MOSI', 'MISO', 'SCK', 'SCL', 'SDA', 'LED_BUILTIN'):
            spi.append({pin['function'] : pin['number']})

        m = re.search(cpa, pin['function'])
        if m:
            analogin[m.group(1)] = {r[1][1] : pin['number']}

        m = re.search(cpt, pin['function'])
        if m:
            timers[pin['number']] = (m.group(1), r[1][1])

        m = re.search(r'PWM', pin['function'])
        if m:
            pwm.append(pin['number'])


    print """#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            {digitalpins}
#define NUM_ANALOG_INPUTS           {analogpins}
#define analogInputToDigitalPin(p)  ((p < {analogpins}) ? (p) + {fanpin} : -1)\n""".format(digitalpins = len(pins), analogpins = len(analogin), fanpin = analogin['0'].items()[0][1])

    print """#if defined(__AVR_{device}__)
#define digitalPinHasPWM(p) ({pwms})
#endif\n""".format(device = ns.device, pwms =  " || ".join(["(p)=={pin}".format(pin=i) for i in pwm]))

    for v in spi:
        for name, pin in v.iteritems():
            print """static const uint8_t {name} = {pin};""".format(name = name, pin = pin)

    print "\n// Analog inputs definition"
    ak = analogin.keys()
    ak.sort()
    for k in ak:
        print """static const uint8_t A{analogpin} = {digitalpin};""".format(analogpin = k, digitalpin = analogin[k].items()[0][1])
    print "// End of analog inputs definition"

    pmode =  """
#ifdef ARDUINO_MAIN

    // these arrays map port names (e.g. port B) to the
    // appropriate addresses for various functions (e.g. reading
    // and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
    NOT_A_PORT,
    """

    pout = """
const uint16_t PROGMEM port_to_output_PGM[] = {
    NOT_A_PORT,
    """

    ppgm =  """
const uint16_t PROGMEM port_to_input_PGM[] = {
    NOT_A_PORT,
    """;

    pk = ports.keys()
    pk.sort()
    for p in pk:
        pmode += "    (uint16_t) &DDR{port},\n".format(port = p)
        pout += "    (uint16_t) &PORT{port},\n".format(port = p)
        ppgm += "    (uint16_t) &PIN{port},\n".format(port = p)
    pmode += "};"
    pout += "};"
    ppgm += "};"

    print pmode
    print pout
    print ppgm

    dptppgm = """const uint8_t PROGMEM digital_pin_to_port_PGM[] = {\n"""
    dptbmpgm = """const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {\n"""
    dpttpgm = """const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {\n"""
    oldp = pins[0]['port'][0]
    pl = "\n    // -- PORT{port} --\n".format(port = oldp)
    dptppgm += pl
    dptbmpgm += pl
    dpttpgm += pl

    for p in pins:
        if oldp <> p['port'][0]:
            pl = "\n    // -- PORT{port} --\n".format(port = p['port'][0])
            dptppgm += pl
            dptbmpgm += pl
            dpttpgm += pl
        dptppgm += "    P{port}, //** {pin} ** P{portbit} - {function} - {comment}\n".format(port = p['port'][0], pin = p['number'], portbit = p['port'], function = p['function'], comment = p['comment'])
        dptbmpgm += "    _BV({portbit}), // ** {pin} ** P{port} - {function} - {comment}\n".format(portbit = p['port'][1], port = p['port'], pin = p['number'], function = p['function'], comment = p['comment'])
        if p['number'] in timers.keys():
            dpttpgm += "    TIMER{timer}, // ** {pin} ** P{port} - {function} - {comment}\n".format(timer =  timers[p['number']][0], port = p['port'], pin = p['number'], function = p['function'], comment = p['comment'])
        else:
            dpttpgm += "    NOT_ON_TIMER, // ** {pin} ** P{port} - {function} - {comment}\n".format(port = p['port'], pin = p['number'], function = p['function'], comment = p['comment'])
        oldp = p['port'][0]
    dptppgm += "};\n"
    dptbmpgm += "};\n"
    dpttpgm += "};\n"

    print dptppgm
    print dptbmpgm
    print dpttpgm

    print "#endif\n#endif"

