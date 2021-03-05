var databox,
  socket,
  count = 0,
  count2 = 0;
function initiate() {
  databox = document.getElementById("data-box");

  var button1 = document.getElementById("cand1");
  button1.addEventListener("click", send);

  var button2 = document.getElementById("cand2");
  button2.addEventListener("click", send);

  var button3 = document.getElementById("counts");
  button3.addEventListener("click", send);

  socket = new WebSocket("http://localhost:3000");

  socket.addEventListener("open", opened);
  socket.addEventListener("message", received);
}

function opened() {
  databox.innerHTML = "CONNECTION OPENED<br>";
  databox.innerHTML += "Status: " + socket.readyState;
}

function received(event) {
  databox.innerHTML = event.data;
}

function send(event) {
  socket.send(event.target.id);
}

window.addEventListener("load", initiate);
