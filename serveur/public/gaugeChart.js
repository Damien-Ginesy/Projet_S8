function getGaugeOptions(){
    return ({
        chart:{
            type:'solidgauge'
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
                [0.9, '#DF5353'] // red
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
            data: [80],
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


function gaugeUpdate(chart){
    var point,
        newVal,
        inc;
    
    if (chart) {
        point = chart.series[0].points[0];
        inc = Math.round((Math.random() - 0.5) * 100);
        newVal = point.y + inc;
    
        if (newVal < 0 || newVal > 100) {
        newVal = point.y - inc;
        }
    
        point.update(newVal);
    }
}

