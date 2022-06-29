function getGaugeOptions(){
    return ({
        chart:{
            type:'solidgauge',
            backgroundColor: 'rgba(0,0,0,0)'
        },
        title:null,
        pane: {
            center: ['50%', '85%'],
            size: '140%',
            startAngle: -90,
            endAngle: 90,
            background: {
              backgroundColor:
                Highcharts.defaultOptions.legend.backgroundColor || '#EEE',
              innerRadius: '60%',
              outerRadius: '100%',
              shape: 'arc'
            }
        },
        exporting: {
            enabled: false
        },
          // the value axis
        yAxis: {
            stops: [
                [0.3, '#55BF3B'], // green
                [0.5, '#DDDF0D'], // yellow
                [0.8, '#DF5353'] // red
            ],
            lineWidth: 0,
            tickWidth: 0,
            minorTickInterval: null,
            tickAmount: 2,
            title: {
                y: -160,
                style:{
                    fontSize:'150%',
                }
            },
            labels: {
                y: 16
            }
        },
        plotOptions: {
            solidgauge: {
                dataLabels: {
                y: 5,
                borderWidth: 0,
                useHTML: true
                }
            }
        }
          
    })
}

function setGaugeData(){
    return({
        yAxis: {
            min: 0,
            max: 100,
            title: {
              text: 'Taux d\'infection'
            }
        },
        credits: {
            enabled: false
        },
        series: [{
            name: 'Taux',
            data: [0],
            dataLabels: {
              format:
                '<div style="text-align:center">' +
                '<span style="font-size:25px">{y}</span><br/>' +
                '<span style="font-size:12px;opacity:0.4">%</span>' +
                '</div>'
            },
            tooltip: {
              valueSuffix: ' %'
            }
        }]
    })
}

const gaugeChart = Highcharts.chart('infectionRate', Highcharts.merge(getGaugeOptions(),setGaugeData()));

setInterval(async function () {
    // Speed
    let point,
      newVal    

    if (!gaugeChart)
        return;
        
    point = gaugeChart.series[0].points[0];
    const data = await fetch('/nodeStat');
    const dataJson = await data.json();
    const taux = Math.round(dataJson.noeudMalicieux/(dataJson.noeudSain+dataJson.noeudMalicieux)*100);
    newVal = taux;

    if (newVal < 0 || newVal > 100) {
        newVal = point.y;
    }

    point.update(newVal);

}, 1000);