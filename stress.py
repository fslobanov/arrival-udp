import subprocess
import os
import signal

from time import sleep
from random import randrange

script_path = os.path.dirname(os.path.realpath(__file__))
server_path = script_path + "/artifacts/arrival-server"
endpoint_path = script_path + "/artifacts/arrival-endpoint"

subprocess.call("pkill -f arrival-server")
subprocess.call("pkill -f arrival-endpoint")

server = subprocess.Popen([server_path], start_new_session=True)
endpoint = subprocess.Popen([endpoint_path], start_new_session=True)

wait = randrange(3, None, 0.01)

sleep(wait)
server.send_signal(signal.SIGUSR2)
endpoint.send_signal(signal.SIGUSR2)

server.wait()
endpoint.wait()
