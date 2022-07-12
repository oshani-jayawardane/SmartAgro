var xValues = ["wet", "dry"];
var yValues = [80, 20];
var barColors = [
    "#00aba9",
    "#b91d47"
];

new Chart("moistureChart", {
    type: "doughnut",
    data: {
        labels: xValues,
        datasets: [{
            // label: '# of votes',
            backgroundColor: barColors,
            data: yValues,
            borderWidth: 2,

        }]
    },
    options: {
        rotation: 1 * Math.PI,
        circumference: 1 * Math.PI,
        title: {
            display: true,
            text: "Soil Moisture Content"
        },
        responsive: true,
        legend: {
            display: true
        },
        cutoutPercentage: 75
    }
});

Chart.pluginService.register({
    beforeDraw: function(chart) {
      var width = chart.chart.width,
          height = chart.chart.height,
          ctx = chart.chart.ctx;
      ctx.restore();
      var fontSize = (height / 114).toFixed(2);
      ctx.font = fontSize + "em sans-serif";
      ctx.textBaseline = "middle";
      var text = "80%",
          textX = Math.round((width - ctx.measureText(text).width) / 2),
          textY = height / 2;
      ctx.fillText(text, textX, textY);
      ctx.save();
    }
  });