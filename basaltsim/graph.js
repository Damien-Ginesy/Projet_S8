//Pour tracer des graphs avec des noeuds et des liens

let infectedNode = []; //liste des id des malicieux pour les afficher en violet
class Graph {
  static canvas;
  static ctx;
  static nodes; //liste des sommets
  static edges; //liste des aretes (orienté)
  static i = 0;

  //Initialisation du graph
  static init() {
    this.canvas = document.getElementById('canvas');
    this.ctx = this.canvas.getContext('2d');

    this.canvas.width = 700;
    this.canvas.height  = 700;

    //parametre graphiques
    this.radius = 4*nodeNumber; //rayon du cercle
    this.offset = 20;

    this.resize();

    this.nodes = [];
    this.edges = [];
    this.i = 0;
  }

  //Rajout d'un noeud
  static createNode(){
    let i = this.i++; //Numéro du noeud

    //Position du noeud
    let x;
    let y;

    //Modes graphiques
    switch (graphMode) {

      case "hexa": //hexagone
        let pasX = 40; //espacement horizontal
        let pasY = 40*0.7; //espacement vertical
        let maxNodeOnLine = 20; //nb max de noeud en ligne

        x = pasX * (i%maxNodeOnLine);
        y = parseInt(i / maxNodeOnLine)*pasY;
        let delta = ((y/pasY)%2)*pasX/2;
        x = x + delta;
        break;

      case "circle": //cercle
        let r = this.radius;
        let c = {'x':r, 'y': r}; //centre du cercle

        if(i == target){ // the target
          x = c.x;
          y = c.y;
        } else {
          let j = i; if(i>target) j--;

          let alpha = Math.PI * 2 * (parseInt(j)-1)/(nodeNumber-1);
          let c = {'x':r, 'y': r}; //centre du cercle
          x = Math.sin(alpha)*r + c.x;
          y = Math.cos(alpha)*r + c.y;

        }
        break;

    }

    x+=this.offset; y+=this.offset;

    this.nodes.push({ 'x': x, 'y': y}); //On ajoute le noeud à la liste
  }

  //Creation d'arete
  static createEdge(a,b){
    this.edges.push([a,b]);
  }

  //Affichage d'une frame
  static draw(){
    this.clear(); //On efface le graph

    let orig = []; //liste des noeuds origine d'une arete
    let dest = []; //liste des noeuds destination d'une arete

    //Affichage des aretes
    for (let i in this.edges) {
      let edge = this.edges[i];
      let node1 = this.nodes[edge[0]];
      let node2 = this.nodes[edge[1]];
      this.ctx.beginPath();
      this.ctx.moveTo(node1.x, node1.y);
      this.ctx.lineTo(node2.x, node2.y);
      this.ctx.stroke();

      orig.push(edge[0]);
      dest.push(edge[1]);

    }

    //Affichage des noeuds
    for (let i in this.nodes) {
      let node = this.nodes[i];
      this.ctx.beginPath();
      this.ctx.arc(node.x, node.y, 10, 0, 2 * Math.PI);
      if(infectedNode.includes(parseInt(i))){
        this.ctx.fillStyle = "#9b59b6";
      } else if(orig.includes(parseInt(i))){
        this.ctx.fillStyle = "#e74c3c";
      } else if(dest.includes(parseInt(i))) {
          this.ctx.fillStyle = "#2ecc71";
      } else {
        this.ctx.fillStyle = "orange";
      }
      this.ctx.fill();
      this.ctx.stroke();
      this.ctx.fillStyle = "black";
      this.ctx.fillText(i, node.x-3*("" + i).length, node.y+3);
    }
  }

  //Redimensionnement de la fenetre de dessin
  static resize(){
    this.scale = 700/(2*this.radius + 2*this.offset);
    let s = this.scale;
    this.ctx.resetTransform();
    this.ctx.scale(s, s);
  }

  //Effacement de la fenetre de dessin
  static clear(){
    this.ctx.clearRect(0, 0, this.canvas.width/this.scale, this.canvas.height/this.scale);
  }

  static drawInfectionRate(){
    document.getElementById("arrow").style.transform = "rotate(" + (globalInfectionRate*180) + "deg)";
  }
}
