class Malicous extends Peer {
  static maliciousPeers = [];
  isMalicious = true;

  constructor() {
    super();
    Malicous.maliciousPeers.push(this);
    infectedNode.push(this.identifier);
  }

  init(){
    this.view = [];
    for (var i = 0; i < viewSize; i++) {
      this.view.push(Malicous.maliciousPeers[parseInt(Math.random()*maliciousNumber)]);
    }
  }

  selectPeer(){
    return Peer.peers[parseInt(Math.random()*nodeNumber)]; //noeud random
    //return Peer.peers[10]; //focus 10
  }

  tick(){
    let spam = 1;
    for (var i = 0; i < spam; i++) {
        this.push(this.selectPeer());
    }
  }

  static tickAllMalicious(){
    for (let peer of this.maliciousPeers) {
      peer.tick();
    }
  }
  pull(p){
    p.onpull(this);

    //Pour compatibilité avec les naifs
    return this.view[parseInt(Math.random()*viewSize)];
  }
  updateSamples(v){/* rien */}
  reset(){ /* rien */}
  onpush(p, v){ /* rien */}
}
