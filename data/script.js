var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

function initWebSocket() {
  console.log('Trying to open a WebSocket connection...');
  websocket = new WebSocket(gateway);
  websocket.onopen = onOpen;
  websocket.onclose = onClose;
  websocket.onmessage = onMessage;
}

function onOpen(event) {
  console.log('Connection opened');
  websocket.send('toggle');
}

function onClose(event) {
  console.log('Connection closed');
  setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
  var date = new Date();
  document.getElementById('fecha').innerHTML = date.toLocaleDateString();
  document.getElementById('hora').innerHTML = event.data;
}

function setTargetTime() {
  var targetTime = document.getElementById('targetTime').value;
  var xhttp = new XMLHttpRequest();
  xhttp.open("POST", "/set-time", true);
  xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  xhttp.send("time=" + targetTime);
}

window.onload = function(event) {
  initWebSocket();
}
