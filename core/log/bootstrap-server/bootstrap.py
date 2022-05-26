from flask import Flask, request, jsonify, Response
from dataclasses import dataclass
import random
import logging, os

# logging.getLogger('werkzeug').setLevel(logging.CRITICAL)


@dataclass
class Node:
    ip: str
    port: int
    id: int
    type: int

NodeList = []
MaliciousList = []
i = 0

lim = 39 #au minimum bs_size
bs_size = 5

app = Flask(__name__)

@app.route('/', methods=['GET'])
def serve():
    bs_size = int(request.args.get('bs_size'));
    if i >= lim and bs_size<i:
        bs = random.sample(NodeList, bs_size)
        ls = ""
        for x in bs:
            ls += x.ip + " " + str(x.port) + " " + str(x.id) + "\n"
        resp = Response(ls)
        resp.headers['Content-Type'] = 'text/plain'
        print(ls)
        return resp
    else:
        return "wait\n", 206

@app.route('/m', methods=['GET'])
def serveM():
    if i >= lim:
        ls = ""
        for x in MaliciousList:
            ls += x.ip + " " + str(x.port) + " " + str(x.id) + "\n"
        resp = Response(ls)
        resp.headers['Content-Type'] = 'text/plain'
        print(ls)
        return resp
    else:
        return "wait\n", 206

@app.route('/all', methods=['GET'])
def serveAll():
    ls = ""
    for x in NodeList:
        ls += x.ip + " " + str(x.port) + " " + str(x.id) + "\n"
    resp = Response(ls)
    resp.headers['Content-Type'] = 'text/plain'
    print(ls)
    return resp

@app.route('/log', methods=['POST'])
def log():
    global i
    type = request.json['type']
    port = request.json['port']
    ip = request.remote_addr
    id = (i:=i+1)
    NodeList.append(Node(ip, port, id, type))
    if(int(type) > 0):
        MaliciousList.append(Node(ip, port, id, type))
    print("Logged " + ip + ":" + str(port) + " (strategy=" + str(type) + ") as vId=" + str(id))
    if i == lim:
        print("Ready to serve bootstrap!")
    return "%s %i\n" % (ip, id)



app.run(host="0.0.0.0", port=8080)
