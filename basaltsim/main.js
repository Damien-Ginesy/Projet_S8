//Parametres par defaut de la simulation
let nodeNumber = 100;
let maliciousNumber = 10;
let viewSize = 6;
let resetCooldown = 10;
let cyclesPerSecond = 10;
let resetNumber = 2;

//Parametres graphiques
let target = 10; //noeud à analyser
let graphMode = "circle";
let graphEnabled = true;

let l; //var global boucle

let canvas_width;

//Mise a jour interface parametres
setParamHTML();

//Lancement de la simulation
start();



function start(){
  stop(); //On arrete si simulation en cours
  console.log("simulation started");
  //On initie le graph
  Graph.init();
  //On cree les noeuds du graph et les pairs
  for (var i = 0; i < nodeNumber; i++) {
    if(i<nodeNumber-maliciousNumber)
      new Peer(); //Pair honnete
    else
      new Malicous(); //Pair malicieux

    Graph.createNode();
  }

  for (var i = 0; i < nodeNumber; i++) {
    //Initialisation des pairs
    Peer.peers[i].init();
    //affichage de l'avancement si l'initialisation est longue
    if(i%100 == 0) console.log("init " + (100*i/nodeNumber) + "%");
  }

  document.getElementById('canvas').width   = canvas_width.toString();
  document.getElementById('canvas').height  = canvas_width.toString();

  //On boucle
  console.log("loop started");
  l = setInterval(loop, 1000/cyclesPerSecond);


  //Fonction de la boucle
  function loop(){
    let t1 = getMs();
    Peer.tickAll();
    //Peer.peers[10].tick();
    //Malicous.tickAllMalicious();
    if(graphEnabled){
      //On affiche
      Graph.edges = Peer.peers[target].getEdges();
      Graph.draw();
    } else {
      console.log(getMs() - t1); //On log le temps de tracé
    }
  }
}

//Stop la boucle et reinitialise la simulation
function stop(){
  clearInterval(l);
  Peer.peers = [];
  Malicous.maliciousPeers = [];
  infectedNode = [];
  Peer.i = 0;
  console.log("simulation stopped");
}



//Code relatif a l'interface HTML

function updateParam(){
  nodeNumber = parseInt(document.getElementById('nodeNumber').value);
  maliciousNumber = parseInt(document.getElementById('maliciousNumber').value);
  viewSize = parseInt(document.getElementById('viewSize').value);
  resetCooldown = parseInt(document.getElementById('resetCooldown').value);
  cyclesPerSecond = parseFloat(document.getElementById('cyclesPerSecond').value);
  resetNumber = parseInt(document.getElementById('resetNumber').value);

  graphMode = document.getElementById('shape').value;
}

function setParamHTML(){
  document.getElementById('nodeNumber').value = nodeNumber;
  document.getElementById('maliciousNumber').value = maliciousNumber;
  document.getElementById('viewSize').value = viewSize;
  document.getElementById('resetCooldown').value = resetCooldown;
  document.getElementById('cyclesPerSecond').value = cyclesPerSecond;
  document.getElementById('resetNumber').value = resetNumber;

  document.getElementById('shape').value = graphMode;
}

function getMs(){
  return (new Date).getSeconds()*1e3 + (new Date).getMilliseconds();
}
