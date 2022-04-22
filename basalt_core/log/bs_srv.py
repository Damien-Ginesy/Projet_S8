from flask import Flask, request, jsonify, Response
from dataclasses import dataclass
import random

@dataclass
class Node:
    ip: str
    port: int
    id: int
    type: int

NodeList = []
i = 0

lim = 3
bs_size = 2

app = Flask(__name__)

@app.route('/', methods=['GET'])
def serve():
    if i > lim:
        bs = random.sample(NodeList, bs_size)
        ls = ""
        for x in bs:
            ls += x.ip + " " + str(x.port) + " " + str(x.id) + "\n"
        resp = Response(ls)
        resp.headers['Content-Type'] = 'text/plain'
        return resp
    else:
        return "wait", 206

@app.route('/log', methods=['GET','POST'])
def log():
    global i
    type = request.form.getlist('type')[0]
    port = request.form.getlist('port')[0]
    ip = request.remote_addr
    id = (i:=i+1)
    NodeList.append(Node(ip, port, id, type))
    return "logged"



app.run(port=8080)
