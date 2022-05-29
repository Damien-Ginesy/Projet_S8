//Parametres graphiques
let IdTarget = 1;
let graphEnabled = true;

let l, dl; //var global boucle
l = setInterval(getData, 500);
getData();

let identifiant = [];
let nodeArray;
let maliciousNode = [];

function plot(){
  clearInterval(dl);

  //On log tt le monde
  for (let node of Object.values(nodeArray)) {
    let idSource = node.nodeID.adresseVirtuelle;
    if(idSource == 0) {console.log(idSource + " contient zero source");}

    if (!identifiant.includes(idSource)) identifiant.push(idSource);
    if(node.malicieux != 0) if (!maliciousNode.includes(idSource)) maliciousNode.push(idSource);

    for (let v of node.vue) {
      idDestination = v.nodeID.adresseVirtuelle;
      if(idDestination == 0) {console.log(idSource + " contient zero");}
      if (!identifiant.includes(idDestination)) identifiant.push(idDestination);
    }
  }

  let sum = 0;
  for (let node of Object.values(nodeArray)) {
    if(!maliciousNode.includes(node.nodeID.adresseVirtuelle)) sum += infectionRate(node.vue);
  }

  sum = sum/(Object.values(nodeArray).length - maliciousNode.length);
  document.getElementById("globalInfectionRate").innerText = sum;

  /*
  for (var id of identifiant) {
    if(nodeArray[id] == "undefined")
  }
  */

  if(graphEnabled){
  Graph.init(identifiant.length);

  for (let node of Object.values(nodeArray)) {
    let idSource = node.nodeID.adresseVirtuelle;
      for (let v of node.vue) {
        idDestination = v.nodeID.adresseVirtuelle;
        if(idSource == identifiant[IdTarget]){
          Graph.createEdge(identifiant.indexOf(idSource),
            identifiant.indexOf(idDestination));
        }
      }
  }
  dl = setInterval(function(){Graph.draw()}, 10);
  }


}

function getData(){
  var req = new XMLHttpRequest();
  req.open('GET', '/nodeData', true);
  req.onload  = function() {
    try {
      let obj = JSON.parse(req.response);
      nodeArray = obj;
      plot();
      updateNodesInfo();
    } catch (e) {
      console.log(e);
    }

  };
  req.send(null);
}



let canvas = document.getElementById("canvas");

let focusedNode = -1;
canvas.addEventListener('mousemove', e => {
  focusedNode = getFocusedNode(canvas, e);
});
canvas.addEventListener('click', e => {
  let n = getFocusedNode(canvas, e);
  if (n!=-1){
    IdTarget = n;
    plot();
  }
});

function getFocusedNode(canvas, evt) {
  var rect = canvas.getBoundingClientRect();
  let cursor = {
      x: (evt.clientX - rect.left)/Graph.scale,
      y: (evt.clientY - rect.top)/Graph.scale
  };

  for (let n in Graph.nodes) {
    node = Graph.nodes[n];
    d = (cursor.x - node.x)**2 + (cursor.y - node.y)**2;
    if(d < 100) return n;
  }
  return -1;
}
