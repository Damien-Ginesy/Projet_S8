
/*-------------------------------------*/
              /* params */
/*-------------------------------------*/


let data = {
  labels: ['noeuds malicieux', 'noeuds bons'],
  datasets: [{
      data: [5,6], // mettre les données du serveur
      backgroundColor: ['#6b2d5c', '#E2C290']
  }]
};


/* db.recupTotalNoeud().then(async(totalNoeud)=>{
    const totalMalicieux = await db.recupTotalMalicieux();
    const nbBon = totalNoeud-totalMalicieux;
    data.datasets.data = [nbBon,totalMalicieux];
});  */





/*-------------------------------------*/
            /* pie chart */
/*-------------------------------------*/


/* graphe 1 pour les statistiques globales */

const ctx = document.querySelector('#graph1')



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


const mainDiv = document.getElementById("chart");


nodes.style.display = 'none';

next.addEventListener("click", () => {
  if(getComputedStyle(ctx).display != "none"){
    screen.style.display = "none";
    mainDiv.style.display = "none";
    nodes.style.display = 'block';
    buttonBar.style.margin = "105px";
  } 
})

prev.addEventListener("click", () => {
    if(getComputedStyle(ctx).display != "none"){
      screen.style.display = "none";
      mainDiv.style.display = "none";
      nodes.style.display = 'block';
      buttonBar.style.marginTop = "105px";
  
    } 
  })
  

global.addEventListener("click", () => {
    if(getComputedStyle(ctx).display != "none"){
        screen.style.display = "block";
        nodes.style.display = 'none';
        mainDiv.style.display = "none";
        buttonBar.style.marginTop = "0px";
    }else{
        screen.style.display = "block";
        mainDiv.style.display = "none";
        buttonBar.style.marginTop = "0px";
    }
  })


/*-------------------------------------*/
            /* header nav */
/*-------------------------------------*/

  
function openNav() {
  document.getElementById("myNav").style.width = "100%";
 }
 
 
function closeNav() {
  document.getElementById("myNav").style.width = "0%";
}


/*-------------------------------------*/
            /* line chart */
/*-------------------------------------*/

mainDiv.style.display = "none";

let tab = document.createElement("table");
tab.className = 'charts-css line show-4-secondary-axes show-heading';
tab.id = "my-chart";
mainDiv.append(tab);  
let tbody = document.createElement('tbody');
tab.append(tbody);

let caption = document.createElement('caption');
caption.textContent = "Graphique Basalt"
tab.append(caption)


let tr1 = document.createElement('tr');
tbody.append(tr1);
let td1 = document.createElement('td');
td1.style =  "--start: 0.0; --size: 0.4";
tr1.append(td1);
let span = document.createElement('span');
span.className = "data"
td1.append(span);


let tr2 = document.createElement('tr');
tbody.append(tr2);
let td2 = document.createElement('td');
td2.style = "--start: 0.4; --size: 0.4" ;
tr2.append(td2);
let span2 = document.createElement('span');
span2.className = "data"
td2.append(span2);


//console.log(mainDiv.childNodes)

function openChart(){
  screen.style.display = "none";
  nodes.style.display = "none";
  mainDiv.style.display = "block";
}

async function init(){
  try{
    const query = await fetch('/stats');
    const res = await query.text();
    console.log(res);
  }catch(err){
    console.log(err)
  }
}
init();