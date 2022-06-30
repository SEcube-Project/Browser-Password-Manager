from flask_restful import Resource, Api, reqparse
from ..Utils import Utils

class API_Time(Resource):
    '''
    Class to manage Time API endpoint.
    Supported methods: GET
    '''

    def __init__(self, utils: Utils) -> None:
        super().__init__()
        self._utils = utils

    def get(self):
        return {
            'time': str(self._utils.tick)
        }
