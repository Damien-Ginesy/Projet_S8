const pcd = document.getElementById('topNavLeft');
const popupCross = document.getElementById('popupQuit');

class NetworkGraph {
    constructor(nodeNb,nodeFinalMax,infectedNodeNb,viewSize){
        this.canvas = document.querySelector("#particles-js");
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
            'firstId':0,
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
            let subGrp;
            Math.floor(nodes/4)<100 ? subGrp=2 : subGrp = 4;
            const nodesInGpMean = Math.floor(nodes/subGrp);
            const nodeRemain = nodes%subGrp;
            const nodesNewWinInfo = [];
            for (let i=0; i<subGrp;i++){
                let nodesInGp = nodesInGpMean;
                if (i<nodeRemain){
                    nodesInGp++;
                }
                const nodeInfo = {
                    'gprId': i,
                    'nodeNbInGrp': nodesInGp,
                    'firstId':(!i? nodesWinInfo.firstId
                            :nodesNewWinInfo[i-1].firstId+nodesNewWinInfo[i-1].nodeNbInGrp),
                }
                nodesNewWinInfo.push(nodeInfo);
            }
            this.nodesWin.push(nodesNewWinInfo);
            this.gpWin++;
        }else{
            const nodesNewWinInfo = [];
            const nodeInfo = {
                'gprId': 0,
                'nodeNbInGrp': nodes,
                'firstId':nodesWinInfo.firstId,
            }
            nodesNewWinInfo.push(nodeInfo);
            this.nodesWin.push(nodesNewWinInfo);
            this.gpWin++;
        }
    }

    nodesSet(nodesWin){
        const winFinal = nodesWin.length === 1;
        const end = winFinal ? nodesWin[0].nodeNbInGrp : nodesWin.length;
        const cvsDim = {
            'width': this.canvas.width,
            'height': this.canvas.height
        }
        const firstId = winFinal ? nodesWin[0].firstId : 0;
        for (let i=0;i<end;i++){
            const idx = i+firstId;
            const node = Node.nodeInit(idx,cvsDim, this.nodes, end, winFinal);
            this.nodes.push(node);
        }
    }

    deleteNodesArray(){
        this.nodes.forEach((value, idx) => {
            this.nodes.splice(idx);
        })
    }

    addWin(nodeWin){
        this.deleteNodesArray();
        this.checkSetWindow(nodeWin);
        this.nodesSet(this.nodesWin[this.gpWin]);
    }

    resizeCanvas(){
        this.c.canvas.width = window.innerWidth;
        this.c.canvas.height = window.innerHeight;
        this.c.update();
    }

    update(){
        this.ctx.clearRect(0,0,this.canvas.width,this.canvas.height);
        
        for(let i=0; i<this.nodes.length; i++){
            // Collision entre noeuds
            for (let j=i+1;j<this.nodes.length;j++){
                if (Node.checkCollision(this.nodes[i].circle,this.nodes[j].circle)){
                    Node.processCollision(this.nodes[i],this.nodes[j]);
                }
            }
            const cvsDim = {
                'width': this.canvas.width,
                'height': this.canvas.height
            }
            Node.nodeDraw(this.nodes[i], this.ctx);
            Node.nodeMove(this.nodes[i], cvsDim);
        }
        requestAnimationFrame(this.update.bind(this));
    }
}

function mousePosition(event) {
    return {
        x: event.clientX,
        y: event.clientY,
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

const nbNodeTot = 1000;
const nodeFinalMax = 200;
const nbInfectedNode = 4;
const viewSize = 5;

c = new NetworkGraph(nbNodeTot,nodeFinalMax,nbInfectedNode,viewSize);
c = c.init();
c.update();


/******************************************************************* */
/*************************EVENT LISTENER**************************** */   
/******************************************************************* */
const breakException = {};

pcd.addEventListener('click', function(event) {
    if(c.gpWin<2)
        return;
    console.log('more')
    }, false
)

popupCross.addEventListener('click', function(event) {
    nodeClearHover();
})

c.canvas.addEventListener("mousemove", function(event){
    const mouse = mousePosition(event);
    const nodeWin = c.nodesWin[c.gpWin];
    try{
        c.nodes.forEach(el=>{
            if(Node.mouseInNode(el.circle, mouse)){
                const final = nodeWin.length===1?1:0;
                nodeHover(nodeWin[el.id],final);
                throw breakException;
            }
        })

    }catch(err){
        if(err!== breakException)
            throw err
    }
    }, false
)

c.canvas.addEventListener('click', function(event){
    mouse = mousePosition(event);
    const nodeWin = c.nodesWin[c.gpWin];
    try{
        c.nodes.forEach(el=>{
            if(Node.mouseInNode(el.circle, mouse)){
                c.addWin(nodeWin[el.id]);
            }
        })
   }catch(err){
       console.log(err);
   }}, false
)

// Resize window 
//window.addEventListener('resize',c.resizeCanvas,false);