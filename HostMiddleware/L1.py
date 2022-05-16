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

    def GetAllPasswords(self, hostname: str = None, llen: int = -1):

        lh, ph = self._str2charptr(hostname) if isinstance(hostname, str) else (0, 0x0)
        
        hostsize = (ctypes.c_uint16)()
        usersize = (ctypes.c_uint16)()
        passsize = (ctypes.c_uint16)()
        totallen = (ctypes.c_uint16)()
        self._c_lib.L1_GetPasswords_Sizes(self._l1inst, ctypes.byref(hostsize), ctypes.byref(usersize), ctypes.byref(passsize), ctypes.byref(totallen), ph, lh)

        hostsize = hostsize.value
        usersize = usersize.value
        passsize = passsize.value
        totallen = totallen.value

        if llen != -1 and totallen > llen:
            totallen = llen

        ids = (ctypes.c_uint32*totallen)()
        hostsizes = (ctypes.c_uint16*totallen)()
        usersizes = (ctypes.c_uint16*totallen)()
        passsizes = (ctypes.c_uint16*totallen)()

        hh = hostsize*totallen
        uu = usersize*totallen
        pp = passsize*totallen
        rhosts = (ctypes.c_char*hh)()
        rusers = (ctypes.c_char*uu)()
        rpasss = (ctypes.c_char*pp)()

        self._c_lib.L1_GetPasswords(self._l1inst, ids, hostsizes, usersizes, passsizes, rhosts, rusers, rpasss, ph, lh)
        # entries = [ (id, str(rhosts[i*hostsize:i*hostsize+h].decode('utf-8')), str(rusers[i*usersize:i*usersize+u].decode('utf-8')), str(rpasss[i*passsize:i*passsize+p].decode('utf-8'))) for i, (id, h, u, p) in enumerate(zip(ids, hostsizes, usersizes, passsizes)) ]

        entries = []
        hidx = uidx = pidx = 0
        for i, (id, h, u, p) in enumerate(zip(ids, hostsizes, usersizes, passsizes)):
            # print(f"i: {i:5d}, h: {h:5d}, hidx: {hidx:5d}, start: {hidx:5d}, end: {hidx+h-1:5d}, len: {h:5d}")
            entries.append((id, rhosts[hidx:hidx+h].decode('utf-8'), rusers[uidx:uidx+u].decode('utf-8'), rpasss[pidx:pidx+p].decode('utf-8')))
            hidx += h
            uidx += u
            pidx += p

        return entries[:totallen]

    def AddPassword(self, hostname:str , username:str , password:str):

        ln, ph = self._str2charptr(hostname)
        lu, pu = self._str2charptr(username)
        lp, pp = self._str2charptr(password)

        self._c_lib.L1_AddPassword(self._l1inst, ph, ln, pu, lu, pp, lp)

    def _bool2uint8(self, b: bool):
        return ctypes.c_uint8(1 if b else 0)

    def _str2charptr(self, s: str):
        ln = len(s)
        return (ln, (ctypes.c_char*ln)(*[ord(c) for c in s]))
