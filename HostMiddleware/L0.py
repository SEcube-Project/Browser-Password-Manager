import ctypes
import pathlib

class L0:

    def __init__(self):
        self._libname = f"{pathlib.Path().absolute()}/lib.so"
        self._c_lib = ctypes.CDLL(self._libname)
        self._l0inst = self._c_lib.createL0Instance()

    def getDeviceListSize(self):
        return self._c_lib.L0_getDeviceListSize(self._l0inst)
