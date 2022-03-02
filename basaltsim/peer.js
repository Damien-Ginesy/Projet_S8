let samples = []; //pairs envoyé à la couche applicative

//Gestion des pairs dans la simulation
class Peer {
  static peers = []; //Liste de TOUTE les pairs
  static i = 0; //compteur des pairs
  static isMalicious = false;

  constructor() {
    Peer.peers.push(this);
    this.identifier = Peer.i; //identifier
    Peer.i++;
    this.t = 0; //cycle
  }

  init(){
    this.seedGen(); //Generation des seeds
    this.bootstrap(); //Boot
    this.r = 1;
  }

  //ALGO

  //Generation des seeds
  seedGen(){
    this.seed = [];
    for (var i = 0; i < viewSize; i++) {
      this.seed.push(parseInt(Math.random()*10000));
    }
  }

  //boot
  bootstrap(){
    this.view = [];
    this.hits = [];
    let bs = [];
    for (let j = 0; j < viewSize; j++) {
      this.view.push({'identifier':-1});
      this.hits.push(0);
      bs.push(Peer.peers[parseInt(Math.random()*nodeNumber)]);
    }
    this.updateSamples(bs);
  }

  //Boucle
  tick(){
    if(this.t % (resetCooldown) == 0) {this.reset()}; //reset
    this.pull(this.selectPeer());
    this.push(this.selectPeer());
    this.t++;
  }

  pull(p){p.onpull(this);}
  onpull(p){this.push(p);}
  push(p){p.onpush(this, this.view);}
  onpush(p, v){
    let nv = [...v]; nv.push(p);
    this.updateSamples(nv);
  }

  //mise a jour des vues
  updateSamples(v){
    for (let i = 0; i < viewSize; i++) {
      let h1 = hash(this.seed[i].toString() + this.view[i].identifier.toString());
      for (let p of v) {
        let h2 = hash(this.seed[i].toString() + p.identifier.toString());
        if(p.identifier == this.view[i].identifier){
          this.hits[i]++;
        } else if(this.view[i].identifier == -1 || h2 < h1) {
           this.view[i] = p; this.hits[i] = 1;
        }
      }
    }
  }

  //reinitialisation
  reset(){
    for (let i = 0; i < resetNumber; i++) {
      this.r = (this.r%viewSize)+1;
      samples.push(this.view[this.r-1]);
      this.seed[this.r-1] = parseInt(Math.random()*10000);
    }
    this.updateSamples(this.view);

  }

  //choix des pairs
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



  //METRIQUE
  getMaliciousCountInView(){
    let m = 0;
    for (let peer of this.view) {
      if(peer.isMalicious) m++;
    }
    return m;
  }

  static getSamplesInfection(){
    let m = 0;
    for (var peer of samples) {
      if(peer.isMalicious) m++;
    }
    return m/samples.length;
  }

  getView(){
    let str = "[ ";
    for (let peer of this.view) {
      str+= peer.identifier+" ";
    }
    str += "]";
    return str;
  }

  //TECHNIQUE

  //generation des aretes pr le graph
  getEdges() {
    let e = [];
    for (let p of this.view) {
        e.push([this.identifier,p.identifier]);
    }
    return e;
  }

  //tick all
  static tickAll(){
    for (let peer of this.peers) {
      peer.tick();
    }
  }

  //Recuperation de tte les aretes pr le graph
  static getAllEdges(){
    let e = [];
    for (let p1 of this.peers) {
      for (let p2 of p1.view) {
          e.push([p1.identifier, p2.identifier]);
      }
    }
    return e;
  }


}
