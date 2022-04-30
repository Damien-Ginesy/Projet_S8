const pcd = document.getElementById('topNavLeft');
const popupCross = document.getElementById('popupQuit');

class NetworkGraph {
    constructor(nodeNb,nodeFinalMax,infectedArray,viewArray){
        this.canvas = document.querySelector("#particles-js");
        this.ctx = this.canvas.getContext('2d');
        this.canvas.width = window.innerWidth;
        this.canvas.height = window.innerHeight;

        this.nodeNb = nodeNb;
        this.nodeFinalMax = nodeFinalMax;
        this.gpWin = 0;
        this.viewArray = viewArray;
        this.nodesWin = [];
        this.nodes = [];
        this.infectedArray = infectedArray;
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
            Math.floor(nodes/4)<225 ? subGrp=2 : subGrp = 4;
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
            const infect = (winFinal && this.infectedArray.includes(idx))?1:0;
            const view = winFinal?this.viewArray[idx]:this.nodes;
            const info = {
                'infect': infect,
                'view':view,
            }
            const node = Node.nodeInit(idx,cvsDim, this.nodes, end, winFinal, info);
            this.nodes.push(node);
        }
    }

    deleteNodesArray(){
        this.nodes.forEach((value, idx) => {
            this.nodes.splice(idx);
        })
    }

    addWin(nodeWin){
        if (this.nodesWin[this.gpWin].length===1)
            return;
        this.deleteNodesArray();
        this.checkSetWindow(nodeWin);
        this.nodesSet(this.nodesWin[this.gpWin]);
    }

    deleteNodesWinArray(){
        this.nodesWin.splice(this.gpWin);
        this.gpWin--;
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
                    Node.processCollision(this.nodes[i].circle,this.nodes[j].circle);
                }
            }
            const cvsDim = {
                'width': this.canvas.width,
                'height': this.canvas.height
            }

            Node.nodeDraw(this.nodes[i], this.ctx);
            Node.nodeDrawLine(i,this.nodes, this.ctx, this.nodesWin[this.gpWin]);
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
      const infectedNodeId = Math.floor(Math.random() * nbNodeTot);
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
const nodeFinalMax = 450;
const nbInfectedNode = 4;
const viewSize = 5;

const viewArray = getViewList(viewSize, nbNodeTot);
const infectArray = getInfectedList(nbInfectedNode ,nbNodeTot);

c = new NetworkGraph(nbNodeTot,nodeFinalMax,infectArray,viewArray);
c = c.init();
c.update();


/******************************************************************* */
/*************************EVENT LISTENER**************************** */   
/******************************************************************* */
const breakException = {};

pcd.addEventListener('click', function(event) {
    if(c.gpWin<2)
        return;
    c.deleteNodesArray();
    c.deleteNodesWinArray();
    c.nodesSet(c.nodesWin[c.gpWin]);
    }, false
)

popupCross.addEventListener('click', function(event) {
    nodeClearHover();
})

c.canvas.addEventListener("mousemove", function(event){
    const mouse = mousePosition(event);
    const nodeWin = c.nodesWin[c.gpWin];
    const final = nodeWin.length===1?1:0;
    try{
        c.nodes.forEach((el, idx)=>{
            if(Node.mouseInNode(el.circle, mouse) && !final){
                nodeHover(nodeWin[el.id],final);
                throw breakException;
            }
            else if (Node.mouseInNode(el.circle, mouse) && final){
                console.log(c.nodes[idx], idx);
                Node.nodeHighlight(idx,c.nodes, c.ctx,c.nodesWin[c.gpWin]);
                nodeHover(el,final);
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
                throw breakException;
            }
        })
   }catch(err){
        if(err!== breakException)
            throw err   
   }}, false
)

// Resize window 
//window.addEventListener('resize',c.resizeCanvas,false);