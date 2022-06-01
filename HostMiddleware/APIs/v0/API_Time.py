from flask_restful import Resource, Api, reqparse
from ..Utils import Utils

class API_Time(Resource):

    def __init__(self) -> None:
        super().__init__()

    def get(self):
        return {
            'time': str(Utils.NTP_TIME())
        }
