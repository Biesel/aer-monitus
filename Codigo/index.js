  // Function to generate chart parameters based on given inputs
  function chartParameters(name, title, color, text) {
    return {
      chart: {
        renderTo: name
      },
      title: {
        text: title
      },
      series: [{
        showInLegend: false,
        data: []
      }],
      plotOptions: {
        line: {
          animation: false,
          dataLabels: {
            enabled: true
          }
        },
        series: {
          color: color
        }
      },
      xAxis: {
        type: 'datetime',
        dateTimeLabelFormats: {
          second: '%H:%M:%S'
        }
      },
      yAxis: {
        title: {
          text: text
        }
      },
      credits: {
        enabled: false
      }
    };
  }

  // Function to set up chart data via AJAX requests at regular intervals
  function setUpChart(readings, path) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
        var x = (new Date()).getTime(),
          y = parseFloat(this.responseText);
        if (readings.series[0].data.length > 40) {
          readings.series[0].addPoint([x, y], true, true, true);
        } else {
          readings.series[0].addPoint([x, y], true, false, true);
        }
      }
    };
    xhttp.open("GET", path, true);
    xhttp.send();
  }

  // Create instances of Highcharts charts with specified parameters
  var chartT = new Highcharts.Chart(chartParameters('chart-temperature', 'DHT 11 Temperature', 'yellow', 'Temperature (Celsius)'));
  var chartH = new Highcharts.Chart(chartParameters('chart-humidity', 'DHT 11 Humidity', 'yellow', 'Humidity (%)'));
  var chartPPM = new Highcharts.Chart(chartParameters('chart-ppm', 'MQ 135 PPM', 'red', 'PPM'));

  // Set up regular intervals for updating chart data via AJAX requests
  setInterval(function() {
    setUpChart(chartH, "temperature");
  }, 30000);

  setInterval(function() {
    setUpChart(chartT, "temperature");
  }, 30000);

  setInterval(function() {
    setUpChart(chartPPM, "temperature");
  }, 30000);