var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var alarmAudio = document.getElementById('alarmAudio');

function initWebSocket() {
  console.log('Trying to open a WebSocket connection...');
  websocket = new WebSocket(gateway);
  websocket.onmessage = onMessage;
}

function onMessage(event) {
  var date = new Date();
  document.getElementById('fecha').innerHTML = date.toLocaleDateString();
  document.getElementById('hora').innerHTML = event.data;

  if (event.data === "ON") {
    activateAlarm();
  } else if (event.data === "OFF") {
    deactivateAlarm();
  }
}

function updateAlarmStatus(status) {
  const statusElement = document.getElementById('alarmStatus');
  statusElement.innerHTML = status;
}

function activateAlarm() {
  fetch('/setAlarm?state=1')
    .then(response => response.text())
    .then(data => {
      console.log(data);
      document.getElementById('alarmAudio').play();
      alarmAudio.loop = true;
    });
}

function deactivateAlarm() {
  fetch('/setAlarm?state=0')
    .then(response => response.text())
    .then(data => {
      console.log(data);
      document.getElementById('alarmAudio').pause();
      document.getElementById('alarmAudio').currentTime = 0;
      updateAlarmStatus('Alarm deactivated');
    });
}

document.getElementById('repetitiveBtn').addEventListener('click', function() {
  var menu = document.getElementById('repetitiveAlarm');
  menu.style.display = menu.style.display === "none" ? "block" : "none";
});

document.getElementById('durationBtn').addEventListener('click', function() {
  var menu = document.getElementById('durationAlarm');
  menu.style.display = menu.style.display === "none" ? "block" : "none";
});

document.getElementById('intervalBtn').addEventListener('click', function() {
  var menu = document.getElementById('durationInterval');
  menu.style.display = menu.style.display === "none" ? "block" : "none";
});

document.addEventListener('DOMContentLoaded', (event) => {
  // Obtener elementos del DOM
  const popup = document.getElementById('popup');
  const openPopupBtn = document.getElementById('openPopupBtn');
  const closeBtn = document.querySelector('.close-btn');

  // Abrir el popup al hacer clic en el botón
  openPopupBtn.addEventListener('click', () => {
    popup.style.display = 'block';
  });

  // Cerrar el popup al hacer clic en el botón de cerrar
  closeBtn.addEventListener('click', () => {
    popup.style.display = 'none';
  });

  // Cerrar el popup si se hace clic fuera del contenido del popup
  window.addEventListener('click', (event) => {
    if (event.target === popup) {
      popup.style.display = 'none';
    }
  });
});

function setTargetTime() {
  var targetTime = document.getElementById('targetTime').value;
  var xhttp = new XMLHttpRequest();
  xhttp.open("POST", "/set-time", true);
  xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  xhttp.send("time=" + targetTime);
}

function sendAlarmDuration() {
  var durationValue = document.getElementById('durationTime').value;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/setAlarmDuration?duration=" + durationValue, true);
  xhr.send();
}

function sendAlarmRepetitive() {
  var repetitiveValue = document.getElementById('repeatCount').value;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/setAlarmRepeat?repeat=" + repetitiveValue, true);
  xhr.send();
}

function sendAlarmInterval() {
  var intervalValue = document.getElementById('durationRepeatInterval').value;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/setAlarmInterval?interval=" + intervalValue, true);
  xhr.send();
}

function setConfigAlarm() {
  setTargetTime();
  sendAlarmDuration();
  sendAlarmRepetitive();
  sendAlarmInterval();
}

function stopAlarm() {
  fetch('/stopAlarm', {method: 'POST'})
    .then(response => response.text())
    .then(() => {
      if (alarmAudio) {
        alarmAudio.pause();
        alarmAudio.currentTime = 0; // Reinicia el audio
      }
    });
}

window.onload = function(event) {
  initWebSocket();
}

const api = {
  key: '965af2672919c9a96bec53314dd4f4fd',
  url: `https://api.openweathermap.org/data/2.5/weather`
}

const card = document.getElementById('card');
const city = document.getElementById('city');
const tempImg = document.getElementById('temp-img');
const weatherImg = document.getElementById('info-Wth');
const temp = document.getElementById('temp');
const hum = document.getElementById('hum');
const weather = document.getElementById('weather');
const feelsL = document.getElementById('feels-like');



function updateImageIcon(description){
  let src = 'https://cdn-icons-png.freepik.com/256/6968/6968724.png';
  if(description.includes('sol')){
    src = 'https://cdn-icons-png.freepik.com/256/6968/6968724.png';
  }else if(description.includes('nubes')|| description.includes('nublado') || description.includes('niebla') || description.includes('nuboso')){
    src = 'https://cdn-icons-png.freepik.com/256/1116/1116486.png';
  }else if(description.includes('lluvia')){
    src = 'https://cdn-icons-png.freepik.com/256/6236/6236336.png';
  }else if(description.includes('tormenta')){
    src = 'https://cdn-icons-png.freepik.com/256/8324/8324739.png';
  }
  tempImg.src = src;
}

function updateBackgroundImage(description) {
  
  let backgroundImage = 'https://img.freepik.com/foto-gratis/naturaleza-esponjosa-espacio-fondo-aire_1122-2337.jpg?t=st=1722972678~exp=1722976278~hmac=7f7ac752ccafdcbe481d82b6e875c4e563b0dddb02ee9ee1e8a99bae6e264f8a&w=740'; // Imagen por defecto
  if (description.includes('nublado') || description.includes('nuboso')) {
      backgroundImage = 'https://img.freepik.com/foto-gratis/lluvia-negro-abstracto-oscuro-poder_1127-2380.jpg?t=st=1722970318~exp=1722973918~hmac=1e89f9b1afa7a91a9b554b2daa272057406e2221701ec73941a24314ff89cdf3&w=740';
  } else if (description.includes('lluvia')) {
      backgroundImage = 'https://t1.uc.ltmcdn.com/es/posts/4/6/1/que_significa_sonar_con_lluvia_51164_600.webp';
  } else if (description.includes('cielo claro') || description.includes('soleado')) {
      backgroundImage = 'https://img.freepik.com/foto-gratis/nube-cielo-azul_1232-3108.jpg?t=st=1722971506~exp=1722975106~hmac=cf14f77a20cb4a2d76246189f68b14286bf89615e5fa88dd176ded00c35580a4&w=740';
  } else if (description.includes('niebla')){
    backgroundImage = 'https://img.freepik.com/foto-gratis/silueta-arboles-desnudos-amanecer_198169-375.jpg?t=st=1723041459~exp=1723045059~hmac=f7e622629d40b3fa0530067c56780386d60421de2bc6c061fd077571c2c2423c&w=740';
  }

  card.style.backgroundImage = `url(${backgroundImage})`;
  card.style.backgroundSize = 'cover';
  card.style.backgroundPosition = 'center';
  card.style.backgroundRepeat = 'no-repeat';
}
function updateTextColor(description) {
  let textColor = '#383737'; // Color de texto por defecto (gris)

  if (description.includes('nublado') || description.includes('lluvia') || description.includes('tormenta') || description.includes('niebla')) {
    textColor = '#fff'; // Texto blanco para fondos oscuros
  } else if (description.includes('cielo claro') || description.includes('soleado')) {
    textColor = '#383737'; // Texto negro para fondos claros
  }

  // Aplicar el color de texto a los elementos deseados
  city.style.color = textColor;
  temp.style.color = textColor;
  hum.style.color = textColor;
  weather.style.color = textColor;
  feelsL.style.color = textColor;
}
async function search(query) {
  try {
  const response = await fetch(`${api.url}?q=${query}&appid=${api.key}&lang=es`);
  const data = await response.json();
  const description = data.weather[0].description.toLowerCase();

  card.style.display = 'block';
  city.innerHTML = `${data.name}, ${data.sys.country}`;   
  temp.innerHTML = `${toCelsius(data.main.temp)}&deg;C`;
  hum.innerHTML = `${data.main.humidity}%`;
  
  weather.innerHTML = description;
  feelsL.innerHTML = `ST: ${toCelsius(data.main.feels_like)} &deg;C`;
  
  updateBackgroundImage(description); 
  updateImageIcon(description);
  updateTextColor(description);
  
  } catch (err) {
  console.log(err);
  alert('No se encontro el lugar buscado');

  }
}

function toCelsius(kelvin) {
  return Math.round(kelvin - 273.15).toFixed(1);
}

function onSubmit(event) {
  event.preventDefault();
  search(searchbox.value);
}
const searchform = document.getElementById('search-form');
const searchbox = document.getElementById('searchbox');
searchform.addEventListener('submit', onSubmit, true);

