//This program handles two sets of data to create IFS Ferns(a.k.a barnsleys ferns)
//It will toggle between two colors via the 'C' event key and toggle between two differnt ferns via a mouse click

var canvas, gl;
var points = [vec2(0.0, 0.0)];                                    //initial points
var vertices = [];                                                //scaled points
var first = false;                                                //boolean statement toggle event handler states
var fernChange = true;
var x, y, newX, newY, minX, minY, maxX, maxY;                     //declare variables
var color1 = { a: 0.3, b: 1.0, c: 0.3, d: 1.0 };                  //initialize color values

function main() {
  canvas = document.getElementById("gl-canvas");

  gl = WebGLUtils.setupWebGL(canvas);
  if (!gl) {
    console.log("WebGL isn't available");
    return;
  }

  initFernA();                                                    //render image while there is no event
  newFern();                                                     //render image while there is no event
  fernChange = false;                                             //first fern has been created

  //event handler to change color on the same fern with a nested switch statement to handle both cases
  //of the key 

  window.onkeydown = function (event) {
    var key = this.String.fromCharCode(event.keyCode);
    switch (key) {
      case 'C':
        switch (first) {
          case true:
            color1 = { a: 0.3, b: 1.0, c: 0.3, d: 1.0 };
            first = false;
            this.render();
            break;
          case false:
            color1 = { a: 0.2, b: 0.8, c: 0.55, d: 0.8 };       //through trial and error, matching color green/blue
            first = true;
            this.render();
            break;
        }
    }
  }

  canvas.addEventListener("click", function () {
    points = [vec2(0.0, 0.0)];                                   //reset arrays for new fern
    vertices = [];
    if (fernChange) {
      initFernA();
      fernChange = false;                                       //handles event state
    }
    else {
      initFernA();
      fernChange = true;
    }
    newFern();
    render();                                                   //render new fern
  });

  render();                                                     //render image while there is no event
}

//Creates the first fern set
function initFernA() {
  var probablity;

  var Aset1 = { a: 0.0, b: 0.0, c: 0.0, d: 0.16, e: 0.0, f: 0.0 };      //js object
  var Aset2 = { a: 0.2, b: -0.26, c: 0.23, d: 0.22, e: 0.0, f: 1.6 };   //which is a
  var Aset3 = { a: -0.15, b: 0.28, c: 0.26, d: 0.24, e: 0.0, f: 0.44 }; //collection of
  var Aset4 = { a: 0.75, b: 0.04, c: -0.04, d: 0.85, e: 0.0, f: 1.6 };  //values with names

  var Bset1 = { a: 0.0, b: 0.0, c: 0.0, d: 0.16, e: 0.0, f: 0.0 };      
  var Bset2 = { a: 0.2, b: -0.26, c: 0.23, d: 0.22, e: 0.0, f: 1.6 };   
  var Bset3 = { a: -0.15, b: 0.28, c: 0.26, d: 0.24, e: 0.0, f: 0.44 }; 
  var Bset4 = { a: 0.85, b: 0.04, c: -0.04, d: 0.85, e: 0.0, f: 1.6 };  

  var aprob = {a:0.1, b:0.18, c:0.26};                                //probablity values for set A            
  var bprob = {a:0.01, b:0.07, c:0.14};                               //for set B
  var p = points;                                                     //variable representing inital points

  for (var i = 0; i < 100000; i++) {
    p = points[i];
    probablity = Math.random();
    if(fernChange){
      probabilityCoords(probablity, Aset1, p, Aset2, Aset3, Aset4,aprob); //if sets chosen
    }
    else{
      probabilityCoords(probablity, Bset1, p, Bset2, Bset3, Bset4,bprob); //if Bsets chosen
    }

    if (i == 100) {
      minX = maxX = p[0];   //set first min and max X values to the points location at 100 iterations
      minY = maxY = p[1];   //set first min and max Y values to the points location at 100 iterations
    }
    else if (i > 100) {
      getMinMax(x, y);       //update the min and max values during each iteration
    }
    points.push(vec2(x, y));  //push points into beginning array
  }
}

//function parameters are the desired set along with their points occuring at a certain probablility
function probabilityCoords(probablity, set1, p, set2, set3, set4,prob) {
  if (probablity <= prob.a) {
    x = set1.a * p[0] + set1.b * p[1] + set1.e;    //x & y are global variables
    y = set1.c * p[0] + set1.d * p[1] + set1.f;
  }
  else if (probablity <= prob.b) {
    x = set2.a * p[0] + set2.b * p[1] + set2.e;
    y = set2.c * p[0] + set2.d * p[1] + set2.f;
  }
  else if (probablity <= prob.c) {
    x = set3.a * p[0] + set3.b * p[1] + set3.e;
    y = set3.c * p[0] + set3.d * p[1] + set3.f;
  }
  else {
    x = set4.a * p[0] + set4.b * p[1] + set4.e;
    y = set4.c * p[0] + set4.d * p[1] + set4.f;
  }
}


//update the min and max values
function getMinMax(x, y) {
  if (x < minX) {
    minX = x;
  }
  else if (x > maxX) {
    maxX = x;
  }
  if (y < minY) {
    minY = y;
  }
  else if (y > maxY) {
    maxY = y;
  }
}

// a new scaled fern will be created based on the values in the points array which were determined by 
// previous functions and probability sets
function newFern() {

  for (var i = 0; i < points.length; i++)          //add our points until the end of points array
  {
    newX = (((points[i][0] - minX) / (maxX - minX)) * 2 - 1);
    newY = (((points[i][1] - minY) / (maxY - minY)) * 2 - 1);
    vertices.push(vec2(newX, newY));
  }
}

//initialize and execute the program
function render() {

  //  Load shaders and initialize attribute buffers
  var program = initShaders(gl, "vertex-shader", "fragment-shader");
  gl.useProgram(program);

  // Load the data into the GPU
  var bufferId = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, bufferId);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(vertices), gl.STATIC_DRAW);

  // Associate out shader variables with our data buffer
  var vPosition = gl.getAttribLocation(program, "vPosition");
  gl.vertexAttribPointer(vPosition, 2, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vPosition);

  var fColor = gl.getUniformLocation(program, "fColor");
  var t = color1;
  gl.uniform4f(fColor, t.a, t.b, t.c, t.d);

  gl.clearColor(1.0, 1.0, 1.0, 1.0);
  gl.clear(gl.COLOR_BUFFER_BIT);

  //send data to the GPU
  gl.drawArrays(gl.POINTS, 0, vertices.length); //or i

  //window.requestAnimationFrame(render);

}
