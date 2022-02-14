//Param
let nodeNumber = 100;
let maliciousNumber = 10;
let viewSize = 6;
let resetCooldown = 10;
let cyclesPerSecond = 10;
let resetNumber = 2;

let l;
let infectedNode = []; //liste des id des malicieux pour les plot en violet

setParamHTML();
start();




function start(){

  stop();
  console.log("simulation started");

  //On initie le canvas
  Graph.init();

  //On cree les noeuds et les pairs
  for (var i = 0; i < nodeNumber-maliciousNumber; i++) {
    new Peer();
    Graph.createNode();
  }
  for (var i = 0; i < maliciousNumber; i++) {
    new Malicous();
    Graph.createNode();
  }

  //On les inits
  console.log("init starting... this may take time");
  for (var i = 0; i < nodeNumber; i++) {
    Peer.peers[i].init();
    if(i%100 == 0){
      console.log("init " + (100*i/nodeNumber) + "%");
    }
  }
  console.log("init finished !");

  //On boucle
  console.log("loop started");
  l = setInterval(loop, 1000/cyclesPerSecond);

  graphEnabled = true;
  function loop(){
    let t1 = getMs();

    Peer.tickAll();
    //Peer.peers[10].tick();
    //Malicous.tickAllMalicious();


    if(graphEnabled){
      Graph.edges = Peer.peers[10].getEdges();
      Graph.draw();
    } else {
      console.log(getMs() - t1);
    }
  }
}

function stop(){
  clearInterval(l);
  Peer.peers = [];
  Malicous.maliciousPeers = [];
  infectedNode = [];
  Peer.i = 0;
  console.log("simulation stopped");
}

function updateParam(){
  nodeNumber = parseInt(document.getElementById('nodeNumber').value);
  maliciousNumber = parseInt(document.getElementById('maliciousNumber').value);
  viewSize = parseInt(document.getElementById('viewSize').value);
  resetCooldown = parseInt(document.getElementById('resetCooldown').value);
  cyclesPerSecond = parseFloat(document.getElementById('cyclesPerSecond').value);
  resetNumber = parseInt(document.getElementById('resetNumber').value);
}

function setParamHTML(){
  document.getElementById('nodeNumber').value = nodeNumber;
  document.getElementById('maliciousNumber').value = maliciousNumber;
  document.getElementById('viewSize').value = viewSize;
  document.getElementById('resetCooldown').value = resetCooldown;
  document.getElementById('cyclesPerSecond').value = cyclesPerSecond;
  document.getElementById('resetNumber').value = resetNumber;
}

function getMs(){
  return (new Date).getSeconds()*1e3 + (new Date).getMilliseconds();
}
