class Peer {
  static peers = [];
  static i = 0;
  static isMalicious = false;

  constructor() {
    Peer.peers.push(this);
    this.n = Peer.i;
    Peer.i++;
    this.t = 0;
  }

  init(){
    this.seedGen();
    this.bootstrap();
    this.r = 1;
  }

  //Algo
  seedGen(){
    this.seed = [];
    for (var i = 0; i < viewSize; i++) {
      this.seed.push(parseInt(Math.random()*10000));
    }
  }

  bootstrap(){
    this.view = [];
    this.hits = [];
    let bs = [];
    for (let j = 0; j < viewSize; j++) {
      this.view.push({'n':-1});
      this.hits.push(0);
      bs.push(Peer.peers[parseInt(Math.random()*nodeNumber)]);
    }
    this.updateSample(bs);
  }

  //Boucle
  tick(){
    if(this.t % resetCooldown == 0) {this.reset()};
    this.pull(this.selectPeer());
    this.push(this.selectPeer());
    this.t++;
  }

  pull(p){p.onpull(this);}
  onpull(p){this.push(p);}
  push(p){p.onpush(this, this.view);}
  onpush(p, v){
    let nv = [...v]; nv.push(p);
    this.updateSample(nv);
  }

  updateSample(v){
    for (let i = 0; i < viewSize; i++) {
      let h1 = MD5(this.seed[i].toString() + this.view[i].n.toString());
      for (let p of v) {
        let h2 = MD5(this.seed[i].toString() + p.n.toString());
        if(p.n == this.view[i].n){
          this.hits[i]++;
        } else if(this.view[i].n == -1 || h2 < h1) {
           this.view[i] = p; this.hits[i] = 1;
        }
      }
    }
  }

  reset(){
    for (let i = 0; i < resetNumber; i++) {
      this.r = (this.r%viewSize)+1;
      //this.view[this.r-1] = this.view[parseInt(Math.random()*viewSize)]; //sample
      this.seed[this.r-1] = parseInt(Math.random()*10000);
    }
    this.updateSample(this.view);

  }

  selectPeer(){
    let index = [];
    let min = Math.min(...this.hits);
    for (var i = 0; i < viewSize; i++) {
      if(this.hits[i] == min){
        index.push(i);
      }
    }
    let r = index[parseInt(Math.random()*index.length)];
    
    this.hits[r]++;
    return this.view[r];
  }

  //Metric

  getMaliciousCountInView(){
    let m = 0;
    for (let peer of this.view) {
      if(peer.isMalicious) m++;
    }
    return m;
  }

  //Technique
  getEdges() {
    let e = [];
    for (let p of this.view) {
        e.push([this.n,p.n]);
    }
    return e;
  }

  static tickAll(){
    for (let peer of this.peers) {
      peer.tick();
    }
  }

  static getAllEdges(){
    let e = [];
    for (let p1 of this.peers) {
      for (let p2 of p1.view) {
          e.push([p1.n, p2.n]);
      }
    }
    return e;
  }


}
