addEventListener("message",received);

function received(event) {
    var startTimer = new Date();
    var start = event.data[0];
    var end = event.data[1];
    var pattern=/\.7/i;

    var sqroot, sum=0;

    for(var i = start; i< end; i++){
        sqroot = Math.sqrt(i);
        if(pattern.test(sqroot) === true)
            sum++;
    }
    console.log("end")
    const endTimer = new Date();
    var time = ((endTimer - startTimer)/1000);

    postMessage([sum, time]);
}
