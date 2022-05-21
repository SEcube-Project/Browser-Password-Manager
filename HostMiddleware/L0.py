import ctypes
import pathlib

class L0:

    def __init__(self):
        self._libname = f"{pathlib.Path().absolute()}/lib.so"
        self._c_lib = ctypes.CDLL(self._libname)
        self._l0inst = self._c_lib.createL0Instance()

    def getDeviceListSize(self):
        return self._c_lib.L0_getDeviceListSize(self._l0inst)

    def getDeviceList(self, size: int = None):

        if size is None:
            size = self.getDeviceListSize()

        plen = size * (self._c_lib.L0_get_DevicePathSize(self._l0inst) + 1)

        ylen = size * (self._c_lib.HOST_get_SerialSize() + 1)

        p = (ctypes.c_char*plen)()
        y = (ctypes.c_char*ylen)()
        self._c_lib.L0_getDeviceList(self._l0inst, p, y, size)

        lp = ''.join([x.decode('utf-8') for x in list(p)])
        ps = [s for s in lp.split("\n") if s.startswith("/")]

        ly = ''.join([x.decode('utf-8') for x in list(y)])
        ys = [s for s in ly.split("\n")]

        return [(i, *x) for i, x, in enumerate([(x, y) for x, y in zip(ps, ys[:len(ps)])])]
