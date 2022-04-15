const popup =  document.getElementsByClassName('popup')[0];

function nodeHover(node, circleLength,nbNodeSub){
    popup.style.display = "block";
    if (circleLength<nbNodeSub){
        popup.innerHTML = "<p id='title'> Group " + node.id + " : ";
        const popupTitle =  document.getElementById('title');
        popupTitle.style.display = "flex";
        popupTitle.style.justifyContent = "center";
        popupTitle.style.fontSize = "200%";
        popupTitle.style.textDecoration = "underline";

        popup.innerHTML += "<p id='info'> Has " + nbNodeSub + " nodes in this group.";        
        const popupInfo = document.getElementById('info');
        popupInfo.style.fontSize = "100%";
        popupInfo.style.marginLeft = "50px";
        popupInfo.style.justifyContent = "left";
    }   
    else {
        popup.innerHTML = "<p id='title'> Node " + node.id + " : ";
        const popupTitle =  document.getElementById('title');
        popupTitle.style.display = "flex";
        popupTitle.style.justifyContent = "center";
        popupTitle.style.fontSize = "200%";
        popupTitle.style.textDecoration = "underline";

        popup.innerHTML += "<div id='view'> </div>";        
        
        /*********View Table **************************/
        const popupInfo = document.getElementById('view');
        const table = document.createElement('table');
        const thead = document.createElement('thead');
        const tbody = document.createElement('tbody');

        table.appendChild(thead);
        table.appendChild(tbody);

        // Adding the entire table to the body tag
        popupInfo.appendChild(table);

        const row_1 = document.createElement('tr');
        const heading_1 = document.createElement('th');
        heading_1.innerHTML = "View";
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
    }
}

function nodeClearHover(){
    popup.style.display = "none";
}