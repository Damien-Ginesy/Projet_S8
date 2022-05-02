class Node {
    constructor(id,circle,info) {
        this.id = id;
        this.circle = circle;
        this.info = info
    }

    static nodeInit(id, cvsDim,nodes, n, winFinal, info){
        let circle;
        if (winFinal){

            /* Si circulaire */ /* modifier la répartition */
            //circle = Node.setPosition(id, cvsDim,'circular', n);
            /* si aléatoire */ /* enlever n + winFinal et le remplacer par r */
            circle = Node.setPosition(id, cvsDim,'default',20);
            while(this.checkBorder(circle, cvsDim) || this.checkOverlap(circle,nodes)){
                // changer l'argument
                circle = Node.setPosition(id, cvsDim,'default',20);
            }
        }
        else{
            circle = Node.setPosition(id, cvsDim,'default',n);
            while(this.checkBorder(circle, cvsDim) || this.checkOverlap(circle,nodes)){
                circle = Node.setPosition(id, cvsDim,'default',n);
            }
        }
        const c=new Node(id,circle, info);
        return c;
    }

    static setPosition(i, cvsDim,shape, n){
        let circle;
        switch(shape){
            case "circular":
                // A modifier
                const baseRadius = Math.min(cvsDim.width,cvsDim.height)/(n/59);
                const angle = Math.PI / n;
                const s = Math.sin(angle);
                const r = baseRadius * s / (1-s);
                const phi = angle * i * 2;
                const cx = cvsDim.width/2+(baseRadius + r) * Math.cos(phi);
                const cy = cvsDim.height/2+(baseRadius + r) * Math.sin(phi);
                circle = {'x' : cx, 'y' : cy, 'r': r, 'sx': 0, 'sy': 0};
                return circle;
            default:
                /* A MODIFIER */
                const rad = n===20 ? n : Math.min(cvsDim.height,cvsDim.width)/10,
                    xspd = n===20 ? 0 : Math.floor((Math.random()*0.2))+0.5,
                    yspd = n===20 ? 0 : Math.floor((Math.random()*0.2))+0.5,
                    x = Math.floor((Math.random()*cvsDim.width)),
                    y = Math.floor((Math.random()*cvsDim.height));
                circle = {'x' : x, 'y' :y, 'r': rad, 'sx': xspd, 'sy': yspd};
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

    static nodeDrawLine(idx,nodes, ctx,win){
        const winFinal = win.length === 1;
        if (!winFinal){
            nodes[idx].info.view.forEach(element => {
                if (element.id === nodes[idx].id)
                    return;
                
                const mx = nodes[idx].circle.x;
                const my= nodes[idx].circle.y;
                ctx.globalAlpha = 0.1;
                ctx.beginPath();
                ctx.moveTo(element.circle.x,element.circle.y);
                ctx.lineTo(mx,my);
                ctx.stroke();
                ctx.closePath();   
            });
        }
        else{
            nodes[idx].info.view.forEach(element => {
                if (element < win[0].firstId || element >= win[0].firstId+win[0].nodeNbInGrp)
                    return;
                const mx = nodes[idx].circle.x;
                const my= nodes[idx].circle.y;
                const nodeIdx = element - win[0].firstId;
                ctx.globalAlpha = 0.1;
                ctx.beginPath();
                ctx.moveTo(nodes[nodeIdx].circle.x,nodes[nodeIdx].circle.y);
                ctx.lineTo(mx,my);
                ctx.stroke();
                ctx.closePath();   
            });
        }
    }

    static nodeHighlight(idx,nodes, ctx, win){
        nodes[idx].info.view.forEach(element => {
            if (element < win[0].firstId || element >= win[0].firstId+win[0].nodeNbInGrp)
                return;
            console.log(element)
            const mx = nodes[idx].circle.x;
            const my= nodes[idx].circle.y;
            const nodeIdx = element - win[0].firstId;
            console.log('in : ', nodeIdx)
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
