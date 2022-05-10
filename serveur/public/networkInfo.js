function nodeHover(node, finalWin){
    const popup =  document.querySelector(".popup");
    const canvas = document.querySelector("#particles-js");
    const view = document.getElementById('view');
    popup.style.display = "block";

    if(!finalWin){
        const popupTitle =  document.getElementById('popupTitle');
        popupTitle.textContent = "Group " + node.gprId + " : ";

        view.style.display = 'none';
        const popupInfo = document.getElementById('popupInfo');
        popupInfo.style.display = 'block';

        const endId = node.firstId+node.nodeNbInGrp-1;
        popupInfo.innerHTML = "Has " + node.nodeNbInGrp + " nodes in this group."
                        + "<br> Range : " + node.firstId + "-"  + endId +".";
    }
    else{
        popupTitle.textContent = "Node " + node.info.idx + " : ";

        view.style.display = 'block';
        const popupInfo = document.getElementById('popupInfo');
        popupInfo.style.display='none';

        const tbody = document.getElementById('viewInfo');
        while (tbody.firstChild) {
            tbody.removeChild(tbody.lastChild);
        }

        let row;
        const viewArray = node.info.view;
        for (let i=0; i<node.info.view.length;i++){
            if (!(i%3))
                row = document.createElement('tr');
            const rowData = document.createElement('td');    
            rowData.innerHTML = viewArray[i].nodeID.adresseReelle +
                "<br>" + viewArray[i].nodeID.port;
            row.appendChild(rowData);
            if (!(i%3) || i<node.info.view.length)
                tbody.appendChild(row);
        }

    }
}

function nodeClearHover(){
    const popup =  document.querySelector(".popup");
    popup.style.display = "none";
}

function showData(){
    document.getElementById("overlay").style.display = "block";
    document.querySelector(".lessData").style.display = "block";
    document.querySelector(".popup").style.display = "none";
    document.getElementById("topNavLeft").style.display = "none";
    document.getElementById("moreData").style.display = "none";
}

function hideData(){
    document.getElementById("overlay").style.display = "none";
    document.querySelector(".lessData").style.display = "none";
    document.getElementById("topNavLeft").style.display = "block";
    document.getElementById("moreData").style.display = "block";
}

const chart = Highcharts.chart('infectionRate', Highcharts.merge(getGaugeOptions(),setGaugeData()));

setInterval(function () {
    // Speed
    let point,
      newVal,
      inc;
  
    if (!chart) 
        return;

    point = chart.series[0].points[0];
    inc = Math.round((Math.random() - 0.5) * 100);
    newVal = point.y + inc;

    if (newVal < 0 || newVal > 100) {
        newVal = point.y - inc;
    }

    point.update(newVal);

}, 1000);