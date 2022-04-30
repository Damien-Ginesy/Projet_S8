const popup =  document.querySelector(".popup");
const canvas = document.querySelector("#particles-js");

function nodeHover(node, finalWin){
    popup.style.display = "block";

    if(!finalWin){
        const popupTitle =  document.getElementById('popupTitle');
        popupTitle.textContent = "Group " + node.gprId + " : ";

        const popupInfo = document.getElementById('popupInfo');
        popupInfo.textContent = "Has " + node.nodeNbInGrp + " nodes in this group.";
    }
    else{
        popupInfo.textContent = "In circle"
    }
    /* if (circleLength<nbNodeSub){
        const popupTitle =  document.getElementById('popupTitle');
        popupTitle.textContent = "Group " + node.id + " : ";

        const popupInfo = document.getElementById('popupInfo');
        popupInfo.textContent = "Has " + nbNodeSub + " nodes in this group.";
        popupInfo.style.display = "flex";
    }   
    else {
        const popupTitle =  document.getElementById('popupTitle');
        popupTitle.textContent = "Node " + node.id + " : ";

        const popupInfo = document.getElementById('popupInfo');
        popupInfo.style.display = "none";     
        
        /*********View Table **************************/
        
        /* const viewDiv = document.createElement("div");
        viewDiv.setAttribute('id', 'view');
        popup.appendChild(viewDiv); */
        /* const popupView = document.getElementById('view');
        const table = document.createElement('table');
        const thead = document.createElement('thead');
        const tbody = document.createElement('tbody'); 
        const table = document.getElementsByTagName('table');
        const thead = document.getElementsByTagName('thead');
        const tbody = document.getElementsByTagName('tbody');

        /* table.appendChild(thead);
        table.appendChild(tbody); 

        // Adding the entire table to the body tag
        //popupView.appendChild(table);

        const row_1 = document.createElement('tr');
        const heading_1 = document.createElement('th');
        heading_1.textContent = "View";
        row_1.appendChild(heading_1);
        thead.appendChild(row_1);


        // Creating and adding data to second row of the table
        const row_2 = document.createElement('tr');
        const row_2_data_1 = document.createElement('td');
        row_2_data_1.innerHTML = node.view[0];
        const row_2_data_2 = document.createElement('td');
        row_2_data_2.innerHTML = node.view[1];
        const row_2_data_3 = document.createElement('td');
        row_2_data_3.innerHTML = node.view[2];
        const row_2_data_4 = document.createElement('td');
        row_2_data_4.innerHTML = node.view[3];
        const row_2_data_5 = document.createElement('td');
        row_2_data_5.innerHTML = node.view[4];
        row_2.appendChild(row_2_data_1);
        row_2.appendChild(row_2_data_2);
        row_2.appendChild(row_2_data_3);
        row_2.appendChild(row_2_data_4);
        row_2.appendChild(row_2_data_5);
        tbody.appendChild(row_2);
    }*/
}

function nodeClearHover(){
    popup.style.display = "none";
}