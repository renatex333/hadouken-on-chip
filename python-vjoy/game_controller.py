import serial
import argparse
import time
import logging
import pyvjoy # Windows apenas


class MyControllerMap:
    def __init__(self):
        self.set_button = {'A': 1, 'B': 2, 'C': 3, 'D': 4, 'E' : 5, 'F' : 6, 'G' : 7, 'H': 8, 'I': 9, 'J': 10, 'K': 11, 'L': 12}
        

class SerialControllerInterface:

    # Protocolo
    # byte 1 -> Botão 1 (estado - Apertado 1 ou não 0)
    # byte 2 -> EOP - End of Packet -> valor reservado 'X'

    def __init__(self, port, baudrate):
        self.ser = serial.Serial(port, baudrate=baudrate)
        self.mapping = MyControllerMap()
        self.j = pyvjoy.VJoyDevice(1)
        self.incoming = '0'

    def set_button(self,buttonID,state):
        """Set a given button (numbered from 1) to On (1 or True) or Off (0 or False)"""
        return self._sdk.SetBtn(state,self.rID,buttonID)

    def update(self):
        ## Sync protocol
        while self.incoming != b'X':
            self.incoming = self.ser.read()
            logging.debug("Received INCOMING: {}".format(self.incoming))

        data = self.ser.read()
        ## logging.debug("Received DATA: {}".format(data))

        if data == b'1':
            logging.info("Sending press 1")
            self.j.set_button(self.mapping.set_button['A'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['A'], 0)
            time.sleep(0.1)

        elif data == b'2':
            logging.info("Sending press 2")
            self.j.set_button(self.mapping.set_button['B'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['B'], 0)
            time.sleep(0.1)
        
        elif data == b'3':
            logging.info("Sending press 3")
            self.j.set_button(self.mapping.set_button['C'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['C'], 0)
            time.sleep(0.1)

        elif data == b'4':
            logging.info("Sending press 4")
            self.j.set_button(self.mapping.set_button['D'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['D'], 0)
            time.sleep(0.1)

        elif data == b'5':
            logging.info("Sending press 5")
            self.j.set_button(self.mapping.set_button['E'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['E'], 0)
            time.sleep(0.1)

        elif data == b'6':
            logging.info("Sending press 6")
            self.j.set_button(self.mapping.set_button['F'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['F'], 0)
            time.sleep(0.1)

        elif data == b'7':
            logging.info("Sending press 7")
            self.j.set_button(self.mapping.set_button['G'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['G'], 0)
            time.sleep(0.1)

        elif data == b'8':
            logging.info("Sending press 8")
            self.j.set_button(self.mapping.set_button['H'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['H'], 0)
            time.sleep(0.1)
        
        elif data == b'w':
            logging.info("Sending press up")
            self.j.set_button(self.mapping.set_button['I'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['I'], 0)
            time.sleep(0.1)

        elif data == b's':
            logging.info("Sending press down")
            self.j.set_button(self.mapping.set_button['J'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['J'], 0)
            time.sleep(0.1)

        elif data == b'a':
            logging.info("Sending press left")
            self.j.set_button(self.mapping.set_button['K'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['K'], 0)
            time.sleep(0.1)

        elif data == b'd':
            logging.info("Sending press right")
            self.j.set_button(self.mapping.set_button['L'], 1)
            time.sleep(0.1)
            self.j.set_button(self.mapping.set_button['L'], 0)
            time.sleep(0.1)
        

    




        self.incoming = self.ser.read()

    def handshake(self):
        while(1):
            data = self.ser.read()
            if data == b'P':
                logging.info("recieved handshake")
                self.ser.write(b'1')
                logging.info("Sent handshake")
                time.sleep(1)
                break

            else:
                logging.info("Handshake in progress")
                logging.debug("Received INCOMING: {}".format(self.incoming))
                time.sleep(1)           



class DummyControllerInterface:
    def __init__(self):
        self.mapping = MyControllerMap()
        self.j = pyvjoy.VJoyDevice(1)

    def update(self):
        self.j.set_button(self.mapping.button['A'], 1)
        time.sleep(0.1)
        self.j.set_button(self.mapping.button['A'], 0)
        logging.info("[Dummy] Pressed A button")
        time.sleep(1)


if __name__ == '__main__':
    interfaces = ['dummy', 'serial']
    argparse = argparse.ArgumentParser()
    argparse.add_argument('serial_port', type=str)
    argparse.add_argument('-b', '--baudrate', type=int, default=9600)
    argparse.add_argument('-c', '--controller_interface', type=str, default='serial', choices=interfaces)
    argparse.add_argument('-d', '--debug', default=False, action='store_true')
    args = argparse.parse_args()
    if args.debug:
        logging.basicConfig(level=logging.DEBUG)

    print("Connection to {} using {} interface ({})".format(args.serial_port, args.controller_interface, args.baudrate))
    if args.controller_interface == 'dummy':
        controller = DummyControllerInterface()
    else:
        controller = SerialControllerInterface(port=args.serial_port, baudrate=args.baudrate)

    controller.handshake();

    while True:
        controller.update()
