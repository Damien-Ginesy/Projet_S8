const canvas = document.getElementById('particles-js');
canvas.width = window.innerWidth;
canvas.height = window.innerHeight;
const ctx = canvas.getContext('2d');
const circles = [];
const col=['red','blue','green','yellow','magenta'],
    bounce=-1;

class Circle {
  constructor(x, y, sx, sy) {
      this.x = x;
      this.y = y;
      this.sx = sx;
      this.sy = sy;
      this.r = 20;

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

      this.pointInCircle = function (mouse) {
        const dx = this.x - mouse.x;
        const dy = this.y - mouse.y;
        const dist = Math.sqrt(dx*dx + dy*dy);
        if (dist < this.r) 
            return true;
        return false
      }
    }
}


// total nodes / nb gp
for(let i=0;i<5;i++){
    const _x = Math.floor((Math.random()*(canvas.width-15)))+15,
        _y = Math.floor((Math.random()*(canvas.height-15)))+15,
        xspd = Math.floor((Math.random()))+0.5,
        yspd = Math.floor((Math.random()))+0.5,
        /* xspd = 0,
        yspd = 0, */
        c=new Circle(_x,_y,xspd,yspd);
    circles.push(c);
}

function update(){
    ctx.clearRect(0,0,canvas.width,canvas.height);
    for(let i=0;i<circles.length;i++){
        circles[i].drawCircle();
        circles[i].moveCircle();
    }
    requestAnimationFrame(update);
}
update();

function mousePosition(event) {
    return {
        x: Math.round(event.clientX),
        y: Math.round(event.clientY)
    }
}

// Click event
canvas.addEventListener("click", function(event) {
    mouse = mousePosition(event)
    circles.forEach(element => {
        // method using point to circle collision detection
        if(element.pointInCircle(mouse)) {
            console.log(mouse)
        }
    });
    },
    false
);

// mouseover
canvas.addEventListener("mousemove", function(event){
  mouse = mousePosition(event)
  circles.forEach(element => {
      if (element.pointInCircle(mouse)) {
          console.log('hover')
          nodeHover(element);
      }
      else
        nodeClearHover();
  });   
  },
  false
)
