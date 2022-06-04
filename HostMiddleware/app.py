import os
import sys
import logging
from flask import Flask
from flask_session import Session
from flask_restful import Api
from flask_cors import CORS

from L0 import L0
from L1 import L1
from CustomFormatter import CustomFormatter

from APIs import Utils
from APIs.v0 import API_Time, API_Devices, API_Device_Generate, API_Device_Password_ID, API_Device_Sessions, API_Device_Passwords

if __name__ == "__main__":

    # remove flask_session directory if it exists
    if os.path.exists("flask_session"):
        for f in os.listdir("flask_session"):
            os.remove(os.path.join("flask_session", f))
    
    app = Flask(__name__)

    app.config["SESSION_PERMANENT"] = False
    app.config["SESSION_TYPE"] = "filesystem"
    app.config["SESSION_COOKIE_SAMESITE"] = "None"
    app.config["SESSION_COOKIE_SECURE"] = True

    api = Api(app)
    CORS(app, supports_credentials=True)
    Session(app)

    l0 = L0()
    l1 = L1()
    utils = Utils()

    logger = logging.getLogger("main")
    logger.setLevel(logging.DEBUG)
    c_handler = logging.StreamHandler(stream=sys.stdout)
    c_handler.setFormatter(CustomFormatter())
    c_handler.setLevel(logging.DEBUG)
    logger.addHandler(c_handler)

    device_cnt = l0.getDeviceListSize()
    if not device_cnt > 0:
        logger.warning("No devices found!")
    else:
        logger.info(f"Found {device_cnt} devices")

    api.add_resource(API_Time, "/api/v0/time")
    api.add_resource(API_Devices, "/api/v0/devices", resource_class_args=[l0])
    api.add_resource(API_Device_Sessions, "/api/v0/device/<int:indx>/sessions", resource_class_args=[logger, l0, l1, utils])
    api.add_resource(API_Device_Generate, "/api/v0/device/<int:indx>/generate", resource_class_args=[logger, l0, l1, utils])
    api.add_resource(API_Device_Passwords, "/api/v0/device/<int:indx>/passwords", resource_class_args=[logger, l0, l1, utils])
    api.add_resource(API_Device_Password_ID, "/api/v0/device/<int:indx>/password/<int:id>", resource_class_args=[logger, l0, l1, utils])
    app.run(ssl_context=('cert.pem', 'key.pem'), debug=False, threaded=False)
