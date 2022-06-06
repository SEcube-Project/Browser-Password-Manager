import ntplib
from threading import Thread, Event
import rsa

class Utils:

    def __init__(self) -> None:
        self._public_key, self._private_key = rsa.newkeys(2048)
        self._pinkeystr = ''.join('{:02x}'.format(x) for x in self.encrypt("PIN"))
        self._endtimekeystr = ''.join('{:02x}'.format(x) for x in self.encrypt("ENDTIME"))
        self._tickcnt = 0
        self._tickth = Thread(target=self._tickth)
        self._stpevt = Event()
        self._tickth.start()

    def _tickth(self):
        
        while not self._stpevt.is_set():
            self._tickcnt += 1
            self._stpevt.wait(1)

    @property
    def tick(self):
        return self._tickcnt

    def stop_tick(self):
        self._stpevt.set()

    @staticmethod
    def HAS_EXPIRED(a, b):
        return a != "0" and int(a) < int(b) + 10

    def encrypt(self, data):
        return rsa.encrypt(data.encode(), self._public_key)

    def decrypt(self, data):
        return rsa.decrypt(data, self._private_key).decode()

    @property
    def pinkeystr(self):
        return self._pinkeystr

    @property
    def endtimekeystr(self):
        return self._endtimekeystr
