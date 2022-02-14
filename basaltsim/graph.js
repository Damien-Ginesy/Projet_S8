lowerY = window.innerHeight;
class Graph {
  static canvas;
  static ctx;
  static nodes = [];
  static edges = [];
  static i = 0;

  static init() {
    this.canvas = document.getElementById('canvas');
    this.ctx = this.canvas.getContext('2d');
    this.resize();
    this.nodes = [];
    this.edges = [];
    this.i = 0;
  }

  static createNode(){
    let i = this.i;
    let pasX = 40;
    let pasY = 40;
    let maxX = window.innerWidth - 200;
    let x = pasX * (i%parseInt(maxX/pasX)) + Math.random()*pasX/2;
    let y = parseInt(i / parseInt(maxX/pasX) )*pasY +  Math.random()*pasY/2;
    let delta = ((y/pasY)%2)*pasX/2;
    this.nodes.push({ 'x': x + delta, 'y': y });
    this.i++;

    lowerY = Math.max(lowerY, y);
  }

  static createEdge(a,b){
    this.edges.push([a,b]);
  }

  static draw(){
    this.clear();
    this.resize();

    let orig = [];
    let dest = [];

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

  static resize(){
    this.canvas.width = window.innerWidth;
    this.canvas.height = lowerY+200;
    this.canvas.style.height = this.canvas.height + "px;";
    this.ctx.translate(100, 100);
  }

  static clear(){
    this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
  }
}
