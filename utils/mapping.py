#!/usr/bin/env python3

import itertools
import collections

all_pins = {'PA1', 'PA2'}.union({f'P{i}{j}' for i in 'CD' for j in range(8)})

tim2 = [
    ['PD4', 'PD3', 'PC0', 'PD7'],
    ['PC5', 'PC2', 'PD2', 'PC1'],
    ['PC1', 'PD3', 'PC0', 'PD7'],
    ['PC1', 'PC7', 'PD6', 'PD5']
]

tim1 = [
    [['PD2', 'PD0'], ['PA1', 'PA2'], ['PC3', 'PD1'], ['PC4']],
    [['PC6', 'PC3'], ['PC7', 'PC4'], ['PC0', 'PD1'], ['PD3']],
    [[]],
    # [['PD2', 'PD0'], ['PA1', 'PA2'], ['PC3', 'PD1'], ['PC4']],
    [['PC4', 'PC3'], ['PC7', 'PD2'], ['PC5', 'PC6'], ['PD4']]
]

mosi_pin = 'PC6'
swio_pin = 'PD1'
tx_pins = {'PD5', 'PD0', 'PD6', 'PC0'}

t1_mapc = 2
t2_mapc = 2
timer_pinc = 13

solutions = set()

for embassy_timer in range(2):
    for embassy_channel in range(4):
        for t2 in itertools.combinations_with_replacement(enumerate(tim2), t2_mapc):
            t2_mappings, t2_pins = zip(*t2)
            if embassy_timer == 1:
                t2_pins = ((el for i, el in enumerate(t2_map) if i != embassy_channel) for t2_map in t2_pins)
            for t1 in itertools.combinations_with_replacement(enumerate(tim1), t1_mapc):
                t1_mappings, t1_pins = zip(*t1)
                if embassy_timer == 0:
                    t1_pins = ((el for i, el in enumerate(t1_map) if i != embassy_channel) for t1_map in t1_pins)
                for t1_prod in itertools.product(*itertools.chain(*t1_pins)):
                    unique_pins = set(itertools.chain(*t2_pins, t1_prod))
                    for pins in itertools.combinations(unique_pins, timer_pinc):
                        pins = set(pins)
                        remaining = all_pins - pins

                        if mosi_pin not in remaining:
                            continue
                        remaining.remove(mosi_pin)

                        if swio_pin not in remaining:
                            continue
                        remaining.remove(swio_pin)

                        for tx_pin in remaining.intersection(tx_pins):
                            remaining2 = remaining - {tx_pin}
                            usb_cands = {i for i in remaining2 if int(i[2]) < 5}
                            usb_port_counts = collections.Counter((i[1] for i in usb_cands))
                            for usb_port, usb_port_count in usb_port_counts.items():
                                if usb_port_count < 2:
                                    continue
                                for usb_pins in itertools.combinations(
                                        (i for i in usb_cands if i[1] == usb_port), 2):
                                    usb_pins = set(usb_pins)
                                    remaining3 = remaining2 - usb_pins
                                    for usb_dpu_pin in remaining3:
                                        if usb_dpu_pin[1] != usb_port:
                                            continue
                                        print(
                                            't1:', *t1_mappings,
                                            't2:', *t2_mappings, 
                                            'tim:', *sorted(pins),
                                            'spi:', mosi_pin,
                                            'tx:', tx_pin,
                                            'usb:', *sorted(usb_pins), usb_dpu_pin,
                                            'emb:', embassy_timer + 1, embassy_channel
                                        )
