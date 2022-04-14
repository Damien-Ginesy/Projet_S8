
/* récupération params url */

const queryString = window.location.search;
const urlParams = new URLSearchParams (queryString);


let nbNoeuds = 100;
let nbMalicieux = 5; 



if(urlParams .get ('nbNoeuds')){
  nbNoeuds = urlParams .get ('nbNoeuds');
}

if(urlParams .get ('nbMalicieux')){
  nbMalicieux= urlParams .get ('nbMalicieux');
}


let nbBon = nbNoeuds-nbMalicieux;



/* graphe 1 pour les statistiques globales */

const ctx = document.querySelector('#graph1')

const data = {
    labels: ['noeuds malicieux', 'noeuds bons'],
    datasets: [{
        data: [nbMalicieux, nbBon], // mettre les données du serveur
        backgroundColor: ['#6b2d5c', '#E2C290']
    }]
};

const config = {
    type: 'doughnut',
    data: data,
    options: {
        maintainAspectRatio : true, // make the pie small
         labels: {
            display: true,
            labels: {
                  fontSize: 500 //change the size of the labels
            }
        }
    }
  };

const graph1 = new Chart(ctx, config);


/* graphe 2 pour les statistiques d'un noeud en particulier */


const ctx2 = document.querySelector('#graph2')

const data2 = {
    labels: ['noeuds malicieux', 'noeuds bons'],
    datasets: [{
        data: [10, 5], // mettre les données du serveur
        backgroundColor: ['#6b2d5c', '#E2C290']
    }]
};

const config2 = {
    type: 'doughnut',
    data: data,
    options: {
      maintainAspectRatio : true, // make the pie small
      labels: {
        display: true,
        labels: {
              fontSize: 500 //change the size of the labels
        }
      },
      plugins: {
        legend : {
          display: false,
        }
      }
      
    }
  };

const graph2 = new Chart(ctx2, config2);


const prev = document.getElementById("prev");
const next = document.getElementById("next");
const global = document.getElementById("global");

const screen = document.getElementById("screen");
const nodes = document.getElementById("nodes");

const buttonBar = document.getElementById("cont");

nodes.style.display = 'none';

next.addEventListener("click", () => {
  if(getComputedStyle(ctx).display != "none"){
    screen.style.display = "none";
    nodes.style.display = 'block';
    buttonBar.style.margin = "105px";
  } 
})

prev.addEventListener("click", () => {
    if(getComputedStyle(ctx).display != "none"){
      screen.style.display = "none";
      nodes.style.display = 'block';
      buttonBar.style.marginTop = "105px";
  
    } 
  })
  

global.addEventListener("click", () => {
    if(getComputedStyle(ctx).display != "none"){
        screen.style.display = "block";
        nodes.style.display = 'none';
        buttonBar.style.marginTop = "0px";
    }else{
        screen.style.display = "block";
        buttonBar.style.marginTop = "0px";
    }
  })



  /* header nav */ 

  
function openNav() {
  document.getElementById("myNav").style.width = "100%";
 }
 
 
function closeNav() {
  document.getElementById("myNav").style.width = "0%";
}



