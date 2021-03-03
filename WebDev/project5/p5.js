var element, canvas, output;
function initiate() {
  element = document.getElementById("canvas");
  canvas = element.getContext("2d");
  output = document.getElementById("pibox");

  var run = document.getElementById("run");
  run.addEventListener("click", create);
}

function create() {
  var size = document.getElementById("dimmensions").value;
  var darts = document.getElementById("darts").value;
  element.width = size;
  element.height = size;
  canvas.beginPath();

  //outline
  canvas.strokeRect(0, 0, size, size);
  //fill color
  canvas.fillStyle = "#C40C2A";
  //fill size
  canvas.fillRect(0, 0, size, size);

  // circle
  canvas.beginPath();
  //X, Y, R, angle, angle, counterclockwise
  //Set R to size/2 for edges to be the same as rectangle
  var startCoords = size / 2;
  var bigRadius = size / 2;
  canvas.arc(startCoords, startCoords, bigRadius, 0, Math.PI * 2, false);
  //outline of the circle
  // canvas.lineWidth = 5;
  // canvas.strokeStyle = "#492F0E";
  // canvas.stroke();
  //filled color of the circle
  canvas.fillStyle = "green";
  canvas.fill();
  //through the darts
  var inOrOut;
  var inCount = 0,
    outCount = 0;
  for (var i = 1; i <= darts; i++) {
    var xPos = Math.random() * element.width;
    var yPos = Math.random() * element.height;
    var radius = 2;
    inOrOut = distance(startCoords, bigRadius, xPos, yPos);
    if (inOrOut === true) {
      canvas.beginPath();
      canvas.arc(xPos, yPos, radius, 0, Math.PI * 2, false);
      canvas.fillStyle = "yellow";
      canvas.fill();
      inCount++;
    } else {
      canvas.beginPath();
      canvas.arc(xPos, yPos, radius, 0, Math.PI * 2, false);
      canvas.fillStyle = "black";
      canvas.fill();
      outCount++;
    }
  }
  output.innerHTML = "<p>Value of PI: " + inCount+ " "+((inCount / darts) * 4) + "</p>";
}

//distance formula to determine if the point lies in or on(the border) of the board
//params are the boards x&y and radius, small x&y
function distance(lgCoord, lgR, smX, smY) {
  var dist_points =
    ((smX - lgCoord) * (smX - lgCoord)) + ((smY - lgCoord) * (smY - lgCoord));
  lgR *= lgR ;
  if (dist_points <= lgR) {
    return true;
  } else if (dist_points > lgR) {
    return false;
  }
}
window.addEventListener("load", initiate);
