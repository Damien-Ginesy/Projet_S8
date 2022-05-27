const express = require('express');
const fs = require('fs');
const app = express();
const port = 3000;

var database = {};

//database = JSON.parse(fs.readFileSync('nodeData.json',{encoding:'utf8', flag:'r'}));

var s = false;

app.use('/panel', express.static('panel'));
app.use(express.json());

app.post('/', (req, res) => {
  if(!s){s = true; console.log("Started!")}

  let data = req.body;
  let id = data[0]["nodeID"]["adresseVirtuelle"];
  database[id] = data[0];

  res.send();
})

app.get('/nodeData', (req, res) => {
  res.setHeader('Access-Control-Allow-Origin','*');
  res.send(JSON.stringify(database));
})


app.listen(port, () => {
  console.log(`Logging server running on port ${port}`)
})
