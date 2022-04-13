
anychart.onDocumentReady(function () {
    anychart.data.loadJsonFile("./sample.json", function (data) {
  
        const chart = anychart.graph(data);
        chart.title("Network Graph showing Basalt protocol");
        chart.container("container").draw();
  
    });
  });