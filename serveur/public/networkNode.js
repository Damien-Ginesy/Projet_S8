class Node {
    constructor(id,circle,info) {
        this.id = id;
        this.circle = circle;
        this.info = info
    }

    static nodeInit(id, cvsDim,nodes, n, winFinal, info){
        let circle;
        if (winFinal){
            circle = Node.setPosition(cvsDim,22);
            while(this.checkBorder(circle, cvsDim) || this.checkOverlap(circle,nodes)){
                // changer l'argument
                circle = Node.setPosition(cvsDim,22);
            }
        }
        else{
            circle = Node.setPosition(cvsDim,n);
            while(this.checkBorder(circle, cvsDim) || this.checkOverlap(circle,nodes)){
                circle = Node.setPosition(cvsDim,n);
            }
        }
        const c=new Node(id,circle, info);
        return c;
    }

    static setPosition(cvsDim,n){
        /* A MODIFIER */
        const rad = n===22 ? n : Math.min(cvsDim.height,cvsDim.width)/10,
            xspd = n===22 ? 0 : Math.floor((Math.random()*0.2))+0.5,
            yspd = n===22 ? 0 : Math.floor((Math.random()*0.2))+0.5,
            x = Math.floor((Math.random()*cvsDim.width)),
            y = Math.floor((Math.random()*cvsDim.height));
        const circle = {'x' : x, 'y' :y, 'r': rad, 'sx': xspd, 'sy': yspd};
        return circle;
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
        node.circle.x += node.circle.sx;
        node.circle.y += node.circle.sy;

        if (node.circle.x + node.circle.r > cvsDim.width) {
            node.circle.x = cvsDim.width - node.circle.r;
            node.circle.sx *= -1;
        }
        else if (node.circle.x - node.circle.r < 0) {
            node.circle.x = node.circle.r;
            node.circle.sx *= -1;
        }
        if (node.circle.y + node.circle.r > cvsDim.height) {
            node.circle.y = cvsDim.height - node.circle.r;
            node.circle.sy *= -1;
        }
        else if (node.circle.y - node.circle.r < 0) {
            node.circle.y = node.circle.r;
            node.circle.sy *= -1;
        }

    }

    static nodeDraw(node, ctx){
        ctx.beginPath();        
        ctx.globalAlpha = 0.5;
        ctx.arc(node.circle.x, node.circle.y,node.circle.r, 0, 2 * Math.PI);
        ctx.fillStyle = node.info.infect? '#C70039' : "#000000";
        ctx.fill();
        ctx.closePath(); 
    }

    static drawLine(c1x,c1y,c2x,c2y, ctx){
        ctx.beginPath();
        ctx.moveTo(c2x,c2y);
        ctx.lineTo(c1x,c1y);
        ctx.stroke();
        ctx.closePath(); 
    }

    static getIdxById(id, nodesArray){
        return nodesArray.findIndex((element)=> 
            JSON.stringify(id) === JSON.stringify(element.id))
    }

    static nodeDrawLine(idx,nodes, ctx,win){
        const winFinal = win.length === 1;
        if (!winFinal){
            nodes[idx].info.view.forEach(element => {
                if (element.id === nodes[idx].id)
                    return;
                
                const mx = nodes[idx].circle.x;
                const my= nodes[idx].circle.y;
                ctx.globalAlpha = 0.1;
                this.drawLine(mx,my,element.circle.x,element.circle.y,ctx); 
            });
        }
        else{
            nodes[idx].info.view.forEach(element => {
                const eleIdx = this.getIdxById(element.nodeID,nodes)
                
                if (eleIdx < win[0].firstId || eleIdx >= win[0].firstId+win[0].nodeNbInGrp)
                    return;
                const mx = nodes[eleIdx].circle.x;
                const my= nodes[eleIdx].circle.y;
                const nodeIdx = eleIdx - win[0].firstId;
                ctx.globalAlpha = 0.1;
                this.drawLine(mx,my,nodes[nodeIdx].circle.x,nodes[nodeIdx].circle.y,ctx);  
            });
        }
    }

    static nodeHighlightLine(idx,nodes, ctx, win){
        nodes[idx].info.view.forEach(element => {
            if (element < win[0].firstId || element >= win[0].firstId+win[0].nodeNbInGrp)
                return;
            console.log(element)
            const mx = nodes[idx].circle.x;
            const my= nodes[idx].circle.y;
            const nodeIdx = element - win[0].firstId;
            console.log('in : ', nodes[nodeIdx].id)
            ctx.beginPath();
            ctx.moveTo(nodes[nodeIdx].circle.x,nodes[nodeIdx].circle.y);
            ctx.lineTo(mx,my);
            //ctx.strokeStyle = "blue";
            ctx.stroke();
            ctx.closePath();   
        });
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
