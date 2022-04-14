const popup =  document.getElementsByClassName('popup');

function nodeHover(node){
    popup[0].style.display = "block";
    showPopup = 1;
}

function nodeClearHover(){
    popup[0].style.display = "none";
}