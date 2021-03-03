var worker = 0,
  databox,
  result = [],
  total = 0,
  time = 0;

function initiate() {
  databox = document.getElementById("databox");
  var start = document.getElementById("start");
  start.addEventListener("click", send);
}

function send() {
  var numThreads = document.getElementById("inNum").value;
  var low = document.getElementById("thread1").value;
  var high = document.getElementById("thread2").value;

  var numPerWorker = (high - low) / numThreads;
  var isround = false;

  if (numPerWorker % 2 != 0) {
    numPerWorker = Math.round(numPerWorker);
    isround = true;
  }
  var start = Number(low);
  var end = numPerWorker + Number(low);

  for (var index = 0; index < numThreads; index++) {
    var threadWorker = new Worker("p4worker.js");
    threadWorker.addEventListener("message", received);
    if (isround === true) {
      end++;
      isround = false;
    }
    threadWorker.postMessage([start, end]);
    start = end;
    end = end + numPerWorker;
    worker++;
  }
}

function received(event) {
  console.log(event.data)
  result.push(event.data);
  if (result.length === worker) {
    displayData();
  }
}

function displayData() {
  var display = "<hr/>";
  for (var i = 0; i < result.length; i++) {
    total = total + result[i][0];
    time = time + result[i][1];
    display += "THREAD:\t" + result[i][0] + "<br/>";
  }
  databox.innerHTML = display + "TOTAL:\t" + total + "\nTIME " + time + " seconds";
}
window.addEventListener("load", initiate);
