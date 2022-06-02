import ntplib
import rsa

class Utils:

    def __init__(self) -> None:
        self._public_key, self._private_key = rsa.newkeys(2048)
        self._pinkeystr = ''.join('{:02x}'.format(x) for x in self.encrypt("PIN"))
        self._endtimekeystr = ''.join('{:02x}'.format(x) for x in self.encrypt("ENDTIME"))

    @staticmethod
    def NTP_TIME():

        c = ntplib.NTPClient()
        response = c.request('europe.pool.ntp.org', version=3)
        return response.tx_time

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
