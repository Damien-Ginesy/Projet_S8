#!/bin/bash

curl -X POST http://localhost:8080/log -H "Content-Type: application/json" -d '{"port": 3000, "type": 0}'