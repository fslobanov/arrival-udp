import subprocess
import os
import signal

from time import sleep
from random import uniform

script_path = os.path.dirname(os.path.realpath(__file__))
server_path = script_path + "/artifacts/arrival-server"

subprocess.Popen(['pkill', "-f", "arrival-server"]).wait()

server = subprocess.Popen([server_path], shell=True, stderr=subprocess.PIPE)


for test_number in range(10):
    print("##############################################")
    print("Test:", test_number)
    waiting_for = uniform(0.1, 2.0)
    print("Waiting for:", waiting_for)

    sleep(waiting_for)

    print("Sending USR2")
    server.send_signal(12)

    server.wait()

    server_ret = server.returncode;
    print("Server returns: ", server_ret)

