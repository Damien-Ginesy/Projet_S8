const pcd = document.getElementById('topNavLeft');
const popupCross = document.getElementById('headerTitle');

class Node {
    /* constructor(id, x, y, sx, sy, r, view) {
        this.id = id;
        this.x = x;
        this.y = y;
        this.sx = sx;
        this.sy = sy;
        this.r = r;
        this.view = view;
    } */

    constructor(id,circle, sx, sy) {
        this.id = id;
        this.circle = circle;
        this.sx = sx;
        this.sy = sy;
    }

    static nodeInit(id, cvsDim,nodes, winFinal){
        let circle;
        if (winFinal){
            circle = Node.setPosition(id, cvsDim,'circular', nodes);
        }
        else{
            circle = Node.setPosition(id, cvsDim,'default',nodes);
        const _x = Math.floor((Math.random()*(cvsDim.width-20))),
            _y = Math.floor((Math.random()*(cvsDim.height-20))),
            /* xspd = Math.floor((Math.random()*0.2))+0.5,
            yspd = Math.floor((Math.random()*0.2))+0.5, */
            xspd = 0,
            yspd = 0;
        
        //    const pos = { 'x': _x,'y': _y}
        }
        const xspd = 0,
            yspd = 0;
            c=new Node(id,circle,xspd,yspd);
        return c;
    }

    static setPosition(i, cvsDim,shape, n){
        let circle;
        switch(shape){
            case "circular":
                // A modifier
                const baseRadius = Math.min(cvsDim.width,cvsDim.height)/(n/2.6);
                const angle = Math.PI / n;
                const s = Math.sin(angle);
                const r = baseRadius * s / (1-s);
                const phi = angle * i * 2;
                const cx = cvsDim.width/2+(baseRadius + r) * Math.cos(phi);
                const cy = cvsDim.height/2+(baseRadius + r) * Math.sin(phi);
                circle = {'x' : cx, 'y' : cy, 'r': r};
                return circle;
            default:
                const rad = Math.min(cvsDim.height,cvsDim.width)/8;
                const x = (cvsDim.width/4)*i+rad;
                const y = rad+100;
                circle = {'x' : x, 'y' :y, 'r': rad};
                return circle;
        }
    }

    // contour du canvas
    static checkBorder(x,y,radius,cvsDim){
        let _x=x,
            _y=y;
        if (x+radius>cvsDim.width){
            _x = cvsDim.width - radius;
        }else if(x-radius<0){
            _x = radius;
        }

        if (y+radius>cvsDim.height){
            _y=cvsDim.height-radius;
        }else if (y-radius<0){
            _y=radius;
        }
        return {'x':_x,'y':_y};
    }

    static nodeDraw(node, ctx){
        ctx.beginPath();        
        ctx.globalAlpha = 0.5;
        ctx.arc(node.circle.x, node.circle.y,node.circle.r, 0, 2 * Math.PI);
        ctx.fillStyle = "#000000";
        ctx.fill();
        ctx.closePath(); 
    }

}
