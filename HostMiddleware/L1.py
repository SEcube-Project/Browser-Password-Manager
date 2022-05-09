import ctypes
import pathlib

class L1:

    def __init__(self):
        self._libname = f"{pathlib.Path().absolute()}/lib.so"
        self._c_lib = ctypes.CDLL(self._libname)
        self._l1inst = self._c_lib.createL1Instance()

    def SelectSECube(self, indx: int):
        return self._c_lib.L1_SelectSECube_Indx(self._l1inst, indx)
