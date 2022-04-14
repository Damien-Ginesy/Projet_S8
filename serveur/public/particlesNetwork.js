const canvas = document.getElementById('particles-js');
canvas.width = window.innerWidth;
canvas.height = window.innerHeight;
const ctx = canvas.getContext('2d');
const circles = [];
const col=['red','blue','green','yellow','magenta'],
    bounce=-1;


class Circle {
  constructor(id, x, y, sx, sy, r, infect, view) {
      this.id = id;
      this.x = x;
      this.y = y;
      this.sx = sx;
      this.sy = sy;
      this.r = r;
      this.infect = infect;
      this.view = view;

      this.drawCircle = function () {
        ctx.beginPath();
        ctx.arc(this.x, this.y, this.r, 0, 2 * Math.PI, false);
        ctx.fillStyle = "#000000";
        ctx.fill();
        ctx.globalAlpha = 0.5
          
      };

      this.moveCircle = function () {
        this.x += this.sx;
        this.y += this.sy;
        if (this.x + this.r > canvas.width) {
            this.x = canvas.width - this.r;
            this.sx *= bounce;
        }
        else if (this.x - this.r < 0) {
            this.x = this.r;
            this.sx *= bounce;
        }
        if (this.y + this.r > canvas.height) {
            this.y = canvas.height - this.r;
            this.sy *= bounce;
        }
        else if (this.y - this.r < 0) {
            this.y = this.r;
            this.sy *= bounce;
        }
      };

      this.infectedNode = function () {
        if (this.infect){
          ctx.fillStyle = "#C70039";
          ctx.fill();
        }
      }

      this.pointInCircle = function (mouse) {
        const dx = this.x - mouse.x;
        const dy = this.y - mouse.y;
        const dist = Math.sqrt(dx*dx + dy*dy);
        if (dist <= this.r) 
            return true;
        return false
      }
    }
}

/******************************************************************* */
/***************************FONCTIONS******************************* */
/******************************************************************* */


function circleInit(idList, radius, infectList, viewList){
  let infect = false;
  for(let i=0;i<idList.length;i++){
    const _x = Math.floor((Math.random()*(canvas.width-15)))+15,
        _y = Math.floor((Math.random()*(canvas.height-15)))+15,
        xspd = Math.floor((Math.random()))+0.5,
        yspd = Math.floor((Math.random()))+0.5,
        id = idList[i],
        view = viewList[i],
        r = radius;
        /* xspd = 0,
        yspd = 0, */
    if (infectList.includes(i))
      infect= true;
    c=new Circle(id, _x,_y,xspd,yspd, r, infect, view);
    circles.push(c);
    infect=false;
  }
}

function circleDelete(){
  circles.forEach((value, index) =>{
    circles.splice(index);
  })
}

function update(){
    ctx.clearRect(0,0,canvas.width,canvas.height);
    for(let i=0;i<circles.length;i++){
        circles[i].drawCircle();
        circles[i].infectedNode();
        circles[i].moveCircle();
    }
    requestAnimationFrame(update);
}

function mousePosition(event) {
    return {
        x: Math.round(event.clientX),
        y: Math.round(event.clientY)
    }
}

/******************************************************************* */
/***************************PARAMETRAGE***************************** */
/******************************************************************* */

function getInfectedList(nbInfectedNode, nbNodeTot){
  const infectList = [];
  for (let i=0;i<nbInfectedNode;i++){
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
    for (let j=0;j<viewSize;j++){
      const inView = Math.floor(Math.random() * nbNodeTot)+1;
      if (!viewNodeList.includes(inView))
        viewNodeList.push(inView);
    }
    viewList.push(viewNodeList);
  }
  return viewList;
}

/******************************************************************* */
/*************************APPEL FONCTIONS*************************** */   
/******************************************************************* */

const nbNodeTot = 100;
const nbNodeSub = 50;
const nbSubGp = 2;
const nbInfectedNode = 4;
const viewSize = 5;

const infectedList = getInfectedList(nbInfectedNode, nbNodeTot);
const viewList = getViewList(viewSize, nbNodeTot);

circleInit(Array.from({length: nbSubGp}, (_, i) => i + 1),100,[],[]);
update();
/* circleInit(50, 20, infectedList);
update(); */

/******************************************************************* */
/*************************EVENT LISTENER**************************** */   
/******************************************************************* */


const breakException = {};

// Click event
canvas.addEventListener("click", function(event) {
    mouse = mousePosition(event)
    try{
      circles.forEach(element => {
          // method using point to circle collision detection
          if(element.pointInCircle(mouse) && (nbNodeSub!==circles.length)) {      
              circleDelete();
              const groupId = element.id;
              const arraySubGroup = [];
              const arraySubView  = [];
              for (let i=1+nbNodeSub*(groupId-1); i<nbNodeSub*groupId+1;i++){
                arraySubGroup.push(i);
                arraySubView.push(viewList[i]);
              }
              circleInit(arraySubGroup,20, infectedList, arraySubView);
            throw breakException;
          }
      });
    }catch(err){
      if (err!== breakException)
        throw err
      }
    },
    false
);

// mouseover
canvas.addEventListener("mousemove", function(event){
  mouse = mousePosition(event)
  try{
    circles.forEach(element => {
        if (element.pointInCircle(mouse)) {
            nodeHover(element, circles.length, nbNodeSub);
            throw breakException;
        }
        else{
          nodeClearHover();
        }
    });   
  }catch(err){
    if (err!== breakException)
      throw err
    }
  },
  false
)