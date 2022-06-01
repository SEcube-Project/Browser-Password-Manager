from flask_restful import Resource, Api, reqparse
from L0 import L0

class API_Devices(Resource):

    def __init__(self, l0: L0) -> None:
        super().__init__()
        self._l0 : L0 = l0;

    def get(self):
        return {
            'count': self._l0.getDeviceListSize(),
            'devices': [{
                'index': i,
                'path': x, 
                'serial': y
            } for i, x, y in self._l0.getDeviceList()]
        }
