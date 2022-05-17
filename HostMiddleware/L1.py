import ctypes
import pathlib

class L1:

    def __init__(self):
        self._libname = f"{pathlib.Path().absolute()}/lib.so"
        self._c_lib = ctypes.CDLL(self._libname)
        self._l1inst = self._c_lib.createL1Instance()

    def SelectSECube(self, indx: int):
        return self._c_lib.L1_SelectSECube_Indx(self._l1inst, indx)

    def Login(self, pin: str, isAdmin: bool, force: bool) -> bool:

        ln = self._c_lib.HOST_get_PinSize()
        ppin = (ctypes.c_uint8*ln)(*[ord(c) for c in pin])
        return self._c_lib.L1_Login(self._l1inst, ppin, self._bool2uint8(isAdmin), self._bool2uint8(force))

    def Logout(self):
        return self._c_lib.L1_Logout(self._l1inst)

    def GetAllPasswords(self, hostname: str = None, len: int = None):
        return []

    def _bool2uint8(self, b: bool):
        return ctypes.c_uint8(1 if b else 0)
