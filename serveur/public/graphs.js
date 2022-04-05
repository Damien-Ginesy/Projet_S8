


let ctx = document.querySelector('#graph1')

const data = {
    labels: ['noeuds malicieux', 'noeuds bons'],
    datasets: [{
        data: [10, 5], // mettre les données du serveur
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


let prev = document.getElementById("prev");
let next = document.getElementById("next");
let global = document.getElementById("global");

let screen = document.getElementById("screen");
let screen2 = document.getElementById("screen2");


next.addEventListener("click", () => {
  if(getComputedStyle(ctx).display != "none"){
    screen.style.display = "none";
    screen2.style.display = "none";

  } 
})

prev.addEventListener("click", () => {
    if(getComputedStyle(ctx).display != "none"){
      screen.style.display = "none";
  
    } 
  })
  

global.addEventListener("click", () => {
    if(getComputedStyle(ctx).display != "none"){
        screen.style.display = "block";
    }else{
        screen.style.display = "block";
    }
  })