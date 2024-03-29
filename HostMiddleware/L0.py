import ctypes
import pathlib

class L0:

    def __init__(self, path_lib: str):
        self._libname = path_lib
        self._c_lib = ctypes.CDLL(self._libname)

        # Create L0 instance
        self._c_lib.createL0Instance.argtypes = []
        self._c_lib.createL0Instance.restype = ctypes.c_void_p

        # Destroy L0 instance
        self._c_lib.destroyL0Instance.argtypes = [ctypes.c_void_p]
        self._c_lib.destroyL0Instance.restype = None

        # Get device list size
        self._c_lib.L0_getDeviceListSize.argtypes = [ctypes.c_void_p]
        self._c_lib.L0_getDeviceListSize.restype = ctypes.c_int

        # Get device list
        self._c_lib.L0_getDeviceList.argtypes = [ctypes.c_void_p, ctypes.c_char_p, ctypes.c_char_p, ctypes.c_int]
        self._c_lib.L0_getDeviceList.restype = None

        # Get device path size
        self._c_lib.L0_get_DevicePathSize.argtypes = [ctypes.c_void_p]
        self._c_lib.L0_get_DevicePathSize.restype = ctypes.c_int

        # Get device serial size
        self._c_lib.HOST_get_SerialSize.argtypes = []
        self._c_lib.HOST_get_SerialSize.restype = ctypes.c_int

        self._l0inst = self._c_lib.createL0Instance()

    def getDeviceListSize(self):
        '''
        Get the number of devices connected to the host.
        '''
        return self._c_lib.L0_getDeviceListSize(self._l0inst)

    def getDeviceList(self, size: int = None):
        '''
        Get the list of devices connected to the host.
        '''

        if size is None:
            size = self.getDeviceListSize()

        # Create buffers for device list
        # plen is the path size in bytes (including null terminator)
        # ylen is the serial size in bytes (including null terminator)
        plen = size * (self._c_lib.L0_get_DevicePathSize(self._l0inst) + 1)
        ylen = size * (self._c_lib.HOST_get_SerialSize() + 1)

        # p and y are pointers to char
        p = (ctypes.c_char*plen)()
        y = (ctypes.c_char*ylen)()
        self._c_lib.L0_getDeviceList(self._l0inst, p, y, size)

        # Convert c string to python string (L0_getDeviceList returns c strings separated by \n)
        lp = ''.join([x.decode('utf-8') for x in list(p)])
        ps = [s for s in lp.split("\n") if s.startswith("/")]

        ly = ''.join([x.decode('utf-8') for x in list(y)])
        ys = [s for s in ly.split("\n")]

        # Return a list of tuples (index, path, serial) starting from two separated lists of strings
        return [(i, *x) for i, x, in enumerate([(x, y) for x, y in zip(ps, ys[:len(ps)])])]

    def restart(self):
        '''
        Restart the L0 instance.
        '''
        self._c_lib.destroyL0Instance(self._l0inst)
        self._l0inst = self._c_lib.createL0Instance()
