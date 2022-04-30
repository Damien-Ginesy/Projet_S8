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

    static nodeInit(id, cvsDim,nodes, n, winFinal){
        let circle;
        let xspd,yspd;
        if (winFinal){
            circle = Node.setPosition(id, cvsDim,'circular', n);
            xspd = 0,
            yspd = 0;
        }
        else{
            circle = Node.setPosition(id, cvsDim,'default',n);
            while(this.checkBorder(circle, cvsDim) || this.checkOverlap(circle,nodes)){
                circle = Node.setPosition(id, cvsDim,'default',n);
            }
            xspd = Math.floor((Math.random()*0.2))+0.5;
            yspd = Math.floor((Math.random()*0.2))+0.5;
        }
        const c=new Node(id,circle,xspd,yspd);
        return c;
    }

    static setPosition(i, cvsDim,shape, n){
        let circle;
        switch(shape){
            case "circular":
                // A modifier
                const baseRadius = Math.min(cvsDim.width,cvsDim.height)/(n/47);
                const angle = Math.PI / n;
                const s = Math.sin(angle);
                const r = baseRadius * s / (1-s);
                const phi = angle * i * 2;
                const cx = cvsDim.width/2+(baseRadius + r) * Math.cos(phi);
                const cy = cvsDim.height/2+(baseRadius + r) * Math.sin(phi);
                circle = {'x' : cx, 'y' : cy, 'r': r};
                return circle;
            default:
                const rad = Math.min(cvsDim.height,cvsDim.width)/10,
                    x = Math.floor((Math.random()*cvsDim.width)),
                    y = Math.floor((Math.random()*cvsDim.height));
                circle = {'x' : x, 'y' :y, 'r': rad};
                return circle;
        }
    }

    // contour du canvas
    static checkBorder(circle,cvsDim){
        if ((circle.x+circle.r>cvsDim.width || circle.x-circle.r<0) ||
            (circle.y+circle.r>cvsDim.height || circle.y-circle.r<0)){
            
            return true;
        }
        return false;
    }

    static checkOverlap(circle,nodeArray){
        for (let i=0;i<nodeArray.length; i++){
            const dist = Math.sqrt(Math.pow(circle.x-nodeArray[i].circle.x, 2) + 
                Math.pow(circle.y-nodeArray[i].circle.y, 2));
            
            if (dist<circle.r+nodeArray[i].circle.r){
                return true;
            }
        }
        return false;
    }

    static checkCollision(c1,c2) {
        const absx = c1.x - c2.x;
        const absy = c1.y - c2.y;
      
        // find distance between two nodes.
        const distance = Math.sqrt(absx * absx + absy * absy);
        // check if distance is less than sum of two radius - if yes, collision
        if (distance < c1.r + c2.r) {
            return true;
        }
        return false;
    }

    static processCollision(n1,n2){
        n1.sx *= -1;
        n2.sx *= -1;
        n1.sy *= -1;
        n2.sy *= -1;
      }

    static nodeMove(node, cvsDim){
        node.circle.x += node.sx;
        node.circle.y += node.sy;

        if (node.circle.x + node.circle.r > cvsDim.width) {
            node.circle.x = cvsDim.width - node.circle.r;
            node.sx *= -1;
        }
        else if (node.circle.x - node.circle.r < 0) {
            node.circle.x = node.circle.r;
            node.sx *= -1;
        }
        if (node.circle.y + node.circle.r > cvsDim.height) {
            node.circle.y = cvsDim.height - node.circle.r;
            node.sy *= -1;
        }
        else if (node.circle.y - node.circle.r < 0) {
            node.circle.y = node.circle.r;
            node.sy *= -1;
        }

    }

    static nodeDraw(node, ctx){
        ctx.beginPath();        
        ctx.globalAlpha = 0.5;
        ctx.arc(node.circle.x, node.circle.y,node.circle.r, 0, 2 * Math.PI);
        ctx.fillStyle = "#000000";
        ctx.fill();
        ctx.closePath(); 
    }

    static mouseInNode(node, mouse){
        const dx = node.x - mouse.x;
        const dy = node.y - mouse.y;
        const dist = Math.sqrt(dx*dx + dy*dy);
        if (dist <= node.r) 
            return true;
        return false
    }

}
