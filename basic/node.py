import sys
import argparse
import time
import struct
from RF24 import RF24, RF24_PA_LOW

radio = RF24(17, 0)
payload = [0.0]

def slave(timeout=6):
    """Listen for any payloads and print the transaction

    :param int timeout: The number of seconds to wait (with no transmission)
        until exiting function.
    """
    radio.startListening()  # put radio in RX mode

    start_timer = time.monotonic()
    while (time.monotonic() - start_timer) < timeout:
        has_payload, pipe_number = radio.available_pipe()
        if has_payload:
            # fetch 1 payload from RX FIFO
            buffer = radio.read(radio.payloadSize)
            # use struct.unpack() to convert the buffer into usable data
            # expecting a little endian float, thus the format string "<f"
            # buffer[:4] truncates padded 0s in case payloadSize was not set
            payload[0] = struct.unpack("<f", buffer[:4])[0]
            # print details about the received packet
            print(
                "Received {} bytes on pipe {}: {}".format(
                    radio.payloadSize,
                    pipe_number,
                    payload[0]
                )
            )
            start_timer = time.monotonic()  # reset the timeout timer

    print("Nothing received in 6 seconds. Leaving RX role")
    # recommended behavior is to keep in TX mode while idle
    radio.stopListening()  # put the radio in TX mode

if __name__ == "__main__":
    if not radio.begin():
        raise RuntimeError("radio hardware is not responding")
    address = [b"1Node", b"2Node"]
    radio.setPALevel(RF24_PA_LOW) 
    radio.openWritingPipe(address[1]) 
    radio.openReadingPipe(1, address[0])  
    radio.payloadSize = len(struct.pack("<f", payload[0]))
    try:
        slave()
    except KeyboardInterrupt:
        radio.powerDown()
        sys.exit()
