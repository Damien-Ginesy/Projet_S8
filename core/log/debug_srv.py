from flask import Flask, request, jsonify, Response
from dataclasses import dataclass
import logging, os

logging.getLogger('werkzeug').setLevel(logging.CRITICAL)
print("Serveur de logging");

@dataclass
class Node:
    ip: str
    port: int
    id: int
    type: int

NodeList = []

app = Flask(__name__)

@app.route('/', methods=['GET','POST'])
def serve():
    #print(request.data)
    v = " . ";
    if request.json[0]['nodeID']['adresseVirtuelle'] == 1:
        for n in request.json[0]['vue']:
            v = v + str(n['nodeID']['adresseVirtuelle']) + " . "
        print(v)
        for n in request.json[0]['vue']:
            print(n['seed'], n['hitCount'])


    return "hello\n", 206

app.run(host="0.0.0.0", port=8081)
