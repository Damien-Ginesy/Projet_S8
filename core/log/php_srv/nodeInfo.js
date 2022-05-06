function updateNodesInfo(){
  for (let n of [IdTarget]) {
    if(document.getElementById("node-" + n) == null) buildNodeInfo(n);
  }
  for (let elem of document.getElementsByClassName("nodeInfo")) {
    updateNodeInfo(parseInt(elem.id.substring(5)));
  }
}


function buildNodeInfo(n){
  let nodeInfo = document.createElement("div");
  nodeInfo.classList.add("nodeInfo");
  nodeInfo.id = "node-" + n;
  node = nodeArray[n];

  loopID = "";
  loopSeed = "";
  loopHit = "";

  for (var v of node.vue) {
    loopID += "<td>"+v.nodeID.adresseVirtuelle+"</td>";
    loopSeed += "<td>"+v.seed.toString().substring(0,3)+"</td>";
    loopHit += "<td>"+v.hitCount+"</td>";
  }

  nodeInfo.innerHTML = `
  <h3>Noeud ${n}</h3> <p>(${node.nodeID.adresseReelle}:${node.nodeID.port}, ${node.malicieux == 0 ? "non" : ""} malicieux, non sgx, inf=?%)</p>
  <div class="closeButton" onclick="this.parentElement.remove()">x</div>
  <table>
    <tr class="loopID">
     <th>ID</th>
     ${loopID}
   </tr>
   <tr class="loopSeed">
    <th>Seed</th>
    ${loopSeed}
    </tr>
    <tr class="loopHit">
     <th>Hit</th>
     ${loopHit}
   </tr>
    </table>
  `;

  document.getElementById("nodesInfo").appendChild(nodeInfo);
}


function updateNodeInfo(n){
  elem = document.getElementById("node-" + n);
  node = nodeArray[n];

  for (let i in node.vue) {
    v = node.vue[i];
    let tdID = elem.getElementsByClassName("loopID")[0].getElementsByTagName('td')[i];
    let tdSeed = elem.getElementsByClassName("loopSeed")[0].getElementsByTagName('td')[i];
    let tdHit = elem.getElementsByClassName("loopHit")[0].getElementsByTagName('td')[i];

    let valID = v.nodeID.adresseVirtuelle;
    let valSeed = v.seed.toString().substring(0,3);
    let valHit = v.hitCount;

    if(tdID.innerText != valID){tdID.classList.add("newContent");}
    else {tdID.classList.remove("newContent");}
    if(tdSeed.innerText != valSeed){tdSeed.classList.add("newContent");}
    else {tdSeed.classList.remove("newContent");}
    if(tdHit.innerText != valHit){tdHit.classList.add("newContent");}
    else {tdHit.classList.remove("newContent");}

    tdID.innerText = valID;
    tdSeed.innerText = valSeed;
    tdHit.innerText = valHit;
  }

}

function ajouterNoeud(){
  let n = prompt("Noeuds à ajouter ?");
  try {
    buildNodeInfo(parseInt(n));
  } catch (e) {

  }
}

function infectionRate(vue){


}
