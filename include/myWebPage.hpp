#ifndef MYWEBPAGE_HPP
#define MYWEBPAGE_HPP
const char myPage[] PROGMEM = R"===(
<!DOCTYPE html>
<html lang="en">

<head>
  <meta charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Cycles</title>
</head>

<body>
  <h1>Cycles</h1>
  <p>nbCycles cycle <span id="nbCycles">--:--:--</span> </p>
  <p>Start cycle <span id="starTime">--:--:--</span> </p>
  <p>End cycle <span id="endTime">--:--:--</span> </p>
  <p>currentCycle cycle <span id="currentCycle">--:--:--</span> </p>
  <p>minCycle cycle <span id="minCycle">--:--:--</span> </p>
  <p>maxCycle cycle <span id="maxCycle">--:--:--</span> </p>
  <p>totalWorkingTime cycle <span id="totalWorkingTime">--:--:--</span> </p>
  <p>totalSleepingTime cycle <span id="totalSleepingTime">--:--:--</span> </p>
  <script>
    var Socket;
    function init() {
      Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
      Socket.onmessage = function (event) {
        processCommand(event);
      };
    }
    function processCommand(event) {
      var obj = JSON.parse(event.data);
      document.getElementById('nbCycles').innerHTML = obj.nbCycles;
      document.getElementById('starTime').innerHTML = obj.starTime;
      document.getElementById('endTime').innerHTML = obj.endTime;
      document.getElementById('currentCycle').innerHTML = obj.currentCycle;
      document.getElementById('minCycle').innerHTML = obj.minCycle;
      document.getElementById('maxCycle').innerHTML = obj.maxCycle;
      document.getElementById('totalWorkingTime').innerHTML = obj.totalWorkingTime;
      document.getElementById('totalSleepingTime').innerHTML = obj.totalSleepingTime;
      console.log(event.data);
    }
    window.onload = function (event) {
      init();
    }
  </script>
</body>
</html>
)===";
#endif