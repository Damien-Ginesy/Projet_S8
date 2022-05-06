//Parametres graphiques
let IdTarget = 1;

let l, dl; //var global boucle
l = setInterval(getData, 1000);
getData();

let identifiant = [];
let nodeArray;
let infectedNode = [];

function plot(){
  clearInterval(dl);

  //On log tt le monde
  for (let node of nodeArray) {
    let idSource = node.nodeID.adresseVirtuelle;

    if (!identifiant.includes(idSource)) identifiant.push(idSource);
    if(node.malicieux != 0) if (!infectedNode.includes(idSource)) infectedNode.push(idSource);

      for (let v of node.vue) {
        idDestination = v.nodeID.adresseVirtuelle;
        if (!identifiant.includes(idDestination)) identifiant.push(idDestination);
      }
  }

  Graph.init(nodeArray.length);

  for (let node of nodeArray) {
    let idSource = node.nodeID.adresseVirtuelle;
      for (let v of node.vue) {
        idDestination = v.nodeID.adresseVirtuelle;
        if(idSource == IdTarget){
          Graph.createEdge(identifiant.indexOf(idSource),
            identifiant.indexOf(idDestination));
        }
      }
  }

  dl = setInterval(function(){Graph.draw()}, 10);
}

function getData(){
  var req = new XMLHttpRequest();
  req.open('GET', '/log.log', true);
  req.onload  = function() {
     obj = JSON.parse(req.response);
     nodeArray = [];
     for (let n in obj) {
      nodeArray.push(obj[n])
     }
     plot();
     updateNodesInfo();
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
  IdTarget = identifiant[n];
  plot();
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
