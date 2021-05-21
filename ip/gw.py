import sys
import argparse
import time
import struct
from RF24 import RF24, RF24_PA_LOW

radio = RF24(17, 0)
payload = [0.0]

def master():
    """Transmits an incrementing float every second"""
    radio.stopListening()  # put radio in TX mode
    failures = 0
    while failures < 6:
        # use struct.pack() to packet your data into the payload
        # "<f" means a single little endian (4 byte) float value.
        buffer = struct.pack("<f", payload[0])
        start_timer = time.monotonic_ns()  # start timer
        result = radio.write(buffer)
        end_timer = time.monotonic_ns()  # end timer
        if not result:
            print("Transmission failed or timed out")
            failures += 1
        else:
            print(
                "Transmission successful! Time to Transmit: "
                "{} us. Sent: {}".format(
                    (end_timer - start_timer) / 1000,
                    payload[0]
                )
            )
            payload[0] = "hello"
        time.sleep(1)
    print(failures, "failures detected. Leaving TX role.")

if __name__ == "__main__":
    if not radio.begin():
        raise RuntimeError("radio hardware is not responding")
    address = [b"1Node", b"2Node"]
    radio.setPALevel(RF24_PA_LOW) 
    radio.openWritingPipe(address[0]) 
    radio.openReadingPipe(1, address[1])  
    radio.payloadSize = len(struct.pack("<f", payload[0]))
    try:
        master()
    except KeyboardInterrupt:
        radio.powerDown()
        sys.exit()
