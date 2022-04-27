class NetworkGraph {
    constructor(nodeNb,nodeFinalMax,infectedNodeNb,viewSize){
        this.canvas = document.getElementById('particles-js');
        this.ctx = this.canvas.getContext('2d');
        this.canvas.width = window.innerWidth;
        this.canvas.height = window.innerHeight;

        this.nodeNb = nodeNb;
        this.nodeFinalMax = nodeFinalMax;
        this.gpWin = 0;
        this.infectedNodeNb = infectedNodeNb;
        this.viewSize = viewSize;
        this.nodesWin = [];
        this.nodes = [];
        this.infectedList = [];
    }

    init(){
        const nodesWinInfo = [];
        const nodeInfo = {
            'gprId': 0,
            'nodeNbInGrp': this.nodeNb,
        }
        nodesWinInfo.push(nodeInfo);
        this.nodesWin.push(nodesWinInfo);
        this.checkSetWindow(nodesWinInfo[0]);
        this.nodesSet(this.nodesWin[1]);
        return this;
    }

    checkSetWindow(nodesWinInfo){
        const nodes = nodesWinInfo.nodeNbInGrp;
        if (nodes>this.nodeFinalMax){
            const subGrp = 4;
            const nodesInGpMean = Math.floor(nodes/subGrp);
            const nodeRemain = nodes%subGrp;
            const nodesWinInfo = [];
            for (let i=0; i<subGrp;i++){
                let nodesInGp = nodesInGpMean;
                if (i<nodeRemain){
                    nodesInGp++;
                }
                const nodeInfo = {
                    'gprId': i,
                    'nodeNbInGrp': nodesInGp,
                }
                nodesWinInfo.push(nodeInfo);
            }
            this.nodesWin.push(nodesWinInfo);
            this.gpWin++;
        }else{
            const nodesWinInfo = [];
            const nodeInfo = {
                'gprId': 0,
                'nodeNbInGrp': this.nodeNb,
            }
            nodesWinInfo.push(nodeInfo);
            this.nodesWin.push(nodesWinInfo);
        }
    }

    nodesSet(nodesWin){
        const winFinal = nodesWin.length === 1;
        const end = winFinal ? nodesWin[0].nodeNbInGrp : nodesWin.length;
        const cvsDim = {
            'width': this.canvas.width,
            'height': this.canvas.height
        }
        for (let i=0;i<end;i++){
            const node = Node.nodeInit(i,cvsDim, end, winFinal);
            this.nodes.push(node);
        }
    }

    resizeCanvas(){
        this.c.canvas.width = window.innerWidth;
        this.c.canvas.height = window.innerHeight;
        this.c.update();
    }

    update(){
        this.ctx.clearRect(0,0,this.canvas.width,this.canvas.height);
        
        for(let i=0; i<this.nodes.length; i++){
            Node.nodeDraw(this.nodes[i], this.ctx);
        }
        //requestAnimationFrame(this.update);
    }
}


/******************************************************************* */
/*************************APPEL FONCTIONS*************************** */   
/******************************************************************* */

function getInfectedList(nbInfectedNode, nbNodeTot){
    const infectList = [];
    while(infectList.length!==nbInfectedNode){
      const infectedNodeId = Math.floor(Math.random() * nbNodeTot)+1;
      if (!infectList.includes(infectedNodeId))
        infectList.push(infectedNodeId);
    }
    return infectList;
  }
  
  function getViewList(viewSize, nbNodeTot){
    const viewList = [];
    for (let i=0;i<nbNodeTot;i++){
      const viewNodeList = [];
      while(viewNodeList.length!==viewSize){
        const inView = Math.floor(Math.random() * nbNodeTot)+1;
        if ( (!viewNodeList.includes(inView)) && (inView!==i))
          viewNodeList.push(inView);
      }
      viewList.push(viewNodeList);
    }
    return viewList;
}

const nbNodeTot = 21;
const nodeFinalMax = 20;
const nbInfectedNode = 4;
const viewSize = 5;

c = new NetworkGraph(nbNodeTot,nodeFinalMax,nbInfectedNode,viewSize);
c = c.init();
c.update();


// Resize window 
//window.addEventListener('resize',c.resizeCanvas,false);