var modelViewMatrix = mat4(); // identity
var modelViewMatrixLoc;
var projectionMatrix;
var projectionMatrixLoc;
var modelViewStack = [];

var points = [];
var colors = [];

var cmtStack = [];

function main() {
  canvas = document.getElementById("gl-canvas");

  gl = WebGLUtils.setupWebGL(canvas);
  if (!gl) {
    alert("WebGL isn't available");
  }

  GeneratePoints();

  modelViewMatrix = mat4();
  projectionMatrix = ortho(-8, 8, -8, 8, -1, 1);

  initWebGL();

  render();
}

function initWebGL() {
  gl.viewport(0, 0, canvas.width, canvas.height);
  gl.clearColor(0.0, 0.0, 0.0, 1.0);

  //  Load shaders and initialize attribute buffers
  var program = initShaders(gl, "vertex-shader", "fragment-shader");
  gl.useProgram(program);

  var cBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, cBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(colors), gl.STATIC_DRAW);

  var vColor = gl.getAttribLocation(program, "vColor");
  gl.vertexAttribPointer(vColor, 4, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vColor);

  var vBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, vBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, flatten(points), gl.STATIC_DRAW);

  var vPosition = gl.getAttribLocation(program, "vPosition");
  gl.vertexAttribPointer(vPosition, 2, gl.FLOAT, false, 0, 0);
  gl.enableVertexAttribArray(vPosition);

  modelViewMatrixLoc = gl.getUniformLocation(program, "modelViewMatrix");
  projectionMatrixLoc = gl.getUniformLocation(program, "projectionMatrix");
}

function scale4(a, b, c) {
  var result = mat4();
  result[0][0] = a;
  result[1][1] = b;
  result[2][2] = c;
  return result;
}

function GeneratePoints() {
  GenerateSky(); //add sky
  GenerateGround(); //add ground
  GenerateStars(); //add stars
  GenerateMountains();
  GeneratePlanet();
  GenerateRings();
  GenerateGhost();
  GenerateTrees();
  GenerateBow();
  GenerateArrow();
}

function GenerateSky() {
  points.push(vec2(-8, 8)); //sky top left
  colors.push(vec4(0.051, 0.0, 0.15, 1));
  points.push(vec2(8, 8)); //top right
  colors.push(vec4(0.051, 0.0, 0.15, 1));
  points.push(vec2(8, -0.5)); //bottom right
  colors.push(vec4(0.45, 0.0, 0.375, 1));
  points.push(vec2(-8, -0.5)); //bottom left
  colors.push(vec4(0.45, 0.0, 0.375, 1));
}
function GenerateGround() {
  points.push(vec2(-8, -8)); //ground bottom left
  colors.push(vec4(0.251, 0.41, 0.1, 1));
  points.push(vec2(8, -8)); //bottom right
  colors.push(vec4(0.251, 0.41, 0.1, 1));
  points.push(vec2(8, -0.5)); // top right
  colors.push(vec4(0.1105, 0.1, 0.05, 1));
  points.push(vec2(-8, -0.5)); //top left
  colors.push(vec4(0.1105, 0.1, 0.05, 1));
}
function GenerateStars() {
  points.push(vec2(0.3, 0.5));
  colors.push(vec4(1.0, 1.0, 1.0, 1));

  points.push(vec2(0.0, 1.1));
  colors.push(vec4(0.1, 1.0, 1.0, 1));

  points.push(vec2(1.0, 1.0));
  colors.push(vec4(1.0, 1.0, 1.0, 1));

  points.push(vec2(0.8, 4.5));
  colors.push(vec4(0.1, 1.0, 1.0, 1));

  points.push(vec2(-1.5, -0.1));
  colors.push(vec4(1.0, 1.0, 1.0, 1));

  points.push(vec2(-0.2, -0.3));
  colors.push(vec4(0.1, 1.0, 1.0, 1));
}
function GenerateMountains() {
  points.push(vec2(-7.6, -1.5)); //1st mountain
  //colors.push(vec4(0.2, 0.2784, 0.2784, 1));
  colors.push(vec4(0.15, 0.1, 0.09, 1));

  points.push(vec2(-2.3, -1.9));
  colors.push(vec4(0.0, 0.01, 0.01, 1));
  points.push(vec2(-5.5, 4));
  colors.push(vec4(0.2747, 0.3649, 0.41, 1));

  points.push(vec2(-4.5, -1.5)); //second mountain
  colors.push(vec4(0.02, 0.02784, 0.02784, 1));
  points.push(vec2(0, -1.5));
  colors.push(vec4(0.0, 0.01, 0.01, 1));
  points.push(vec2(-1.5, 3));
  colors.push(vec4(0.2747, 0.3649, 0.41, 1));

  points.push(vec2(-4, -1.85)); //3th
  colors.push(vec4(0.11, 0.1, 0.09, 1));
  points.push(vec2(2, -1.975));
  colors.push(vec4(0.02, 0.02784, 0.02784, 1));
  points.push(vec2(0.5, 1.51));
  colors.push(vec4(0.02, 0.02784, 0.02784, 1));

  points.push(vec2(-2, -2)); //4th
  colors.push(vec4(0.2, 0.25, 0.25, 1));
  points.push(vec2(2, -2));
  colors.push(vec4(0.02, 0.02784, 0.02784, 1));
  points.push(vec2(-1, 1));
  colors.push(vec4(0.11, 0.11, 0.09, 1));

  points.push(vec2(8, -1)); //5th
  colors.push(vec4(0.2, 0.2784, 0.2784, 1));
  points.push(vec2(6, -1));
  colors.push(vec4(0.0, 0.01, 0.01, 1));
  points.push(vec2(8, 6));
  colors.push(vec4(0.2, 0.2784, 0.2784, 1));

  points.push(vec2(7, -1)); //6th mountain
  colors.push(vec4(0.2, 0.2784, 0.2784, 1));
  points.push(vec2(3.0, -1));
  colors.push(vec4(0.0, 0.01, 0.01, 1));
  points.push(vec2(5, 4));
  colors.push(vec4(0.27, 0.369, 0.41, 1));

  points.push(vec2(8, -2)); //7th mountain
  colors.push(vec4(0.0, 0.01, 0.01, 1));
  points.push(vec2(6, -2));
  colors.push(vec4(0.0, 0.01, 0.01, 1));
  points.push(vec2(7.3, 1));
  colors.push(vec4(0.2747, 0.3649, 0.41, 1));
}
function GeneratePlanet() {
  var Radius = 1.0;
  var numPoints = 80;

  // TRIANGLE_FAN : for solid circle
  for (var i = 0; i < numPoints; i++) {
    var Angle = i * ((2.0 * Math.PI) / numPoints);
    var X = Math.cos(Angle) * Radius;
    var Y = Math.sin(Angle) * Radius;
    colors.push(vec4(0.6, 0.6, 0, 1));
    points.push(vec2(X, Y));

    // use 360 instead of 2.0*PI if // you use d_cos and d_sin
  }
}
function GenerateRings() {
  var Radius = 0.985;
  var numPoints = 12;

  // TRIANGLE_FAN : for solid circle
  for (var i = 1; i < numPoints; i++) {
    var Angle = i * ((2 * Math.PI) / numPoints);
    var X = Math.cos(Angle) * Radius;
    var Y = Math.sin(Angle) * Radius;
    colors.push(vec4(0.1, 0.4, 0.2, 1));
    points.push(vec2(X, Y));

    // use 360 instead of 2.0*PI if // you use d_cos and d_sin
  }
}
function GenerateGhost() {
  // begin body  (87 points)
  points.push(vec2(3, 0));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(3.1, 1));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(3.5, 2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(4, 3.6));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(4, 4));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(4.1, 3.3));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(4.5, 3));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(5.5, 3));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(6, 3.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(6.5, 4));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(6.7, 4.2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(6.8, 2.8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(7, 2.4));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(7.5, 2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(8, 2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(8.5, 1.7));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(9, 1.2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(10, 0.8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(10, -2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(10.4, -2.8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(10.5, -3.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(10.7, -1.7));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(11, -1.4));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(11.2, -1.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(12, -2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(12.5, -2.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(13, -3));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(13, -2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(12.8, -0.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(12, 0));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(12.5, 0.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(11, 1));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(10.8, 1.4));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(10.2, 2.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(10, 4));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(9.8, 7.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(7.5, 9.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(6, 11));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(3, 12));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(0.5, 15));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(0, 17));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-1.8, 17.4));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-4, 16.6));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-5, 14));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-6, 10.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-9, 10));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-10.5, 8.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-12, 7.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-12.5, 4.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-13, 3));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-13.5, -1));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-13, -2.3));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-12, 0));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-11.5, 1.8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-11.5, -2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-10.5, 0));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-10, 2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-8.5, 4));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-8, 4.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-8.5, 7));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-8, 5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-6.5, 4.2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-4.5, 6.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-4, 4));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-5.2, 2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-5, 0));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-5.5, -2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-6, -5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-7, -8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-8, -10));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-9, -12.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-10, -14.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-10.5, -15.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-11, -17.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-5, -14));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-4, -11));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-5, -12.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-3, -12.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2, -11.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(0, -11.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(1, -12));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(3, -12));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(3.5, -7));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(3, -4));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(4, -3.8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(4.5, -2.5));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(3, 0));
  colors.push(vec4(1, 1, 1, 1));
  // end body

  // begin mouth (6 points)
  points.push(vec2(-1, 6));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-0.5, 7));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-0.2, 8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-1, 8.6));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2, 7));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-1.5, 5.8));
  colors.push(vec4(1, 1, 1, 1));
  // end mouth

  // begin nose (5 points)
  points.push(vec2(-1.8, 9.2));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-1, 9.8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-1.1, 10.6));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-1.6, 10.8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-1.9, 10));
  colors.push(vec4(1, 1, 1, 1));

  // begin left eye, translate (2.6, 0.2, 0) to draw the right eye
  // outer eye, draw line loop (9 points)
  points.push(vec2(-2.9, 10.8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2.2, 11));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2, 12));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2, 12.8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2.2, 13));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2.5, 13));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2.9, 12));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-3, 11));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2.9, 10.5));
  colors.push(vec4(1, 1, 1, 1));

  // eye ball, draw triangle_fan (7 points)
  points.push(vec2(-2.5, 11.4)); // middle point
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2.9, 10.8));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2.2, 11));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2, 12));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2.9, 12));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-3, 11));
  colors.push(vec4(1, 1, 1, 1));
  points.push(vec2(-2.9, 10.5));
  colors.push(vec4(1, 1, 1, 1));
  // end left eye
}
function GenerateTrees() {
  // colors.push(vec4(0.0,0.0,0.0,1));
  points.push(vec2(0, 0.0)); //starting points
  colors.push(vec4(0.0, 0.0, 0.0, 1));
  points.push(vec2(0, 0.0));
  colors.push(vec4(0.0, 0.0, 0.0, 1));
  points.push(vec2(1, 0.1));
  colors.push(vec4(0.0, 0.0, 0.0, 1));

  points.push(vec2(2.2, 1.0)); //first branch
  colors.push(vec4(0.0, 0.0, 0.0, 1));
  points.push(vec2(2.2, 1.0));
  colors.push(vec4(0.0, 0.0, 0.0, 1));
  points.push(vec2(1.2, 0.3));
  colors.push(vec4(0.0, 0.0, 0.0, 1));

  points.push(vec2(2.8, 1)); //second branch
  colors.push(vec4(0.0, 0.0, 0.0, 1));
  points.push(vec2(1.9, 0.3));
  colors.push(vec4(0.0, 0.0, 0.0, 1));
  points.push(vec2(3.5, 1));
  colors.push(vec4(0.0, 0.0, 0.0, 1));

  points.push(vec2(3, 0.6)); //starting points
  colors.push(vec4(0.0, 0.0, 0.0, 1));
  points.push(vec2(2.2, 0.1));
  colors.push(vec4(0.0, 0.0, 0.0, 1));
  points.push(vec2(1, 0.1));
  colors.push(vec4(0.0, 0.0, 0.0, 1));

  points.push(vec2(4, 0.8)); //third branch
  colors.push(vec4(0.1, 0, 0.0, 1));
  points.push(vec2(2.5, 0));
  colors.push(vec4(0.0, 0.0, 0.0, 1));
  points.push(vec2(3.9, 0.0));
  colors.push(vec4(0.0, 0.0, 0.0, 1));

  points.push(vec2(5, 0.1)); //trunk
  colors.push(vec4(0.21, 0.11, 0.11, 1));
  points.push(vec2(5, 0)); //trunk
  colors.push(vec4(0.0, 0.0, 0.0, 1));
  points.push(vec2(1, 0)); //trunk
  colors.push(vec4(0.21, 0.21, 0.21, 1));
}

function GenerateBow() {
  points.push(vec2(-2, -6.5));
  colors.push(vec4(0.21, 0.11, 0.11, 1));
  points.push(vec2(-1.7, -6));
  colors.push(vec4(0.11, 0.1, 0.1, 1));

  points.push(vec2(-0.8, -5.6));
  colors.push(vec4(0.14, 0.1, 0.1, 1));

  points.push(vec2(-0.9, -5.45));
  colors.push(vec4(0.14, 0.1, 0.1, 1));

  points.push(vec2(0, -5.25));
  colors.push(vec4(0.14, 0.1, 0.1, 1));

  points.push(vec2(0.9, -5.45));
  colors.push(vec4(0.14, 0.1, 0.1, 1));

  points.push(vec2(0.8, -5.6));
  colors.push(vec4(0.14, 0.1, 0.1, 1));

  points.push(vec2(1.7, -6));
  colors.push(vec4(0.1, 0.1, 0.1, 1));
  points.push(vec2(2, -6.5));
  colors.push(vec4(0.21, 0.11, 0.11, 1));

  points.push(vec2(-1.8, -6.3));
  colors.push(vec4(0.9, 0.9, 0.9, 1));
  points.push(vec2(-1.8, -6.2));
  colors.push(vec4(0.5, 0.5, 0.5, 1));

  points.push(vec2(-0.1, -5.38));
  colors.push(vec4(0.7, 0.7, 0.7, 1));
  points.push(vec2(0.1, -5.38));
  colors.push(vec4(0.7, 0.7, 0.7, 1));

  points.push(vec2(1.8, -6.2));
  colors.push(vec4(0.5, 0.5, 0.5, 1));
  points.push(vec2(1.8, -6.3));
  colors.push(vec4(0.9, 0.9, 0.9, 1));
}
function GenerateArrow() {
  points.push(vec2(0, 0));
  colors.push(vec4(0.21, 0.11, 0.11, 1));
  points.push(vec2(1, 0.1));
  colors.push(vec4(1.0, 1.0, 1.0, 1));
  points.push(vec2(1, 0.1));
  colors.push(vec4(1.0, 1.0, 1.0, 1));

  points.push(vec2(3, 1.4));
  colors.push(vec4(1.0, 1.0, 1.0, 1));
  points.push(vec2(3, 1.4));
  colors.push(vec4(1.0, 1.0, 1.0, 1));
  points.push(vec2(2, 0.0));
  colors.push(vec4(1.0, 1.0, 1.0, 1));

  points.push(vec2(2, 0.0));
  colors.push(vec4(1.0, 1.0, 1.0, 1));
  points.push(vec2(2, 0.0));
  colors.push(vec4(0.21, 0.11, 0.11, 1));
  points.push(vec2(4, 1.4));
  colors.push(vec4(1.0, 1.0, 1.0, 1));

  points.push(vec2(4, 1.4));
  colors.push(vec4(1.0, 1.0, 1.0, 1));
  points.push(vec2(4, 1.4));
  colors.push(vec4(1.0, 1.0, 1.0, 1));
  points.push(vec2(3, 0.0));
  colors.push(vec4(0.21, 0.11, 0.11, 1));

  points.push(vec2(4.5, 0));
  colors.push(vec4(0.21, 0.11, 0.11, 1));

  points.push(vec2(-6.4, -0.1));
  colors.push(vec4(0.21, 0.11, 0.11, 1));
  points.push(vec2(-6.4, 0.1));
  colors.push(vec4(0.21, 0.11, 0.11, 1));

  points.push(vec2(4.5, 0.1));
  colors.push(vec4(0.21, 0.11, 0.11, 1));
  points.push(vec2(4.5, -0.1));
  colors.push(vec4(0.21, 0.11, 0.11, 1));
  points.push(vec2(-6.4, -0.1));
  colors.push(vec4(0.21, 0.11, 0.11, 1));
}

function DrawGhost() {
  modelViewMatrix = mult(modelViewMatrix, scale4(1 / 20, 1 / 20, 1));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.LINE_LOOP, 126, 87); // body
  gl.drawArrays(gl.LINE_LOOP, 213, 6); // mouth
  gl.drawArrays(gl.LINE_LOOP, 219, 5); // nose

  gl.drawArrays(gl.LINE_LOOP, 224, 9); // left eye
  gl.drawArrays(gl.TRIANGLE_FAN, 231, 7); // left eye ball

  modelViewMatrix = mult(modelViewMatrix, translate(2.6, 0, 0));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.LINE_STRIP, 224, 9); // right eye
  gl.drawArrays(gl.TRIANGLE_FAN, 231, 7); // right eye ball
}

function DrawFullPlanet() {
  modelViewMatrix = mat4();
  modelViewMatrix = mult(modelViewMatrix, translate(-3.35, 5.5, 0));
  modelViewMatrix = mult(modelViewMatrix, scale4(0.7, 0.7 * 1.618, 1));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  // draw planet circle
  gl.drawArrays(gl.TRIANGLE_FAN, 35, 80);
}

function DrawSkyandGround() {
  gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
  gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
}

function DrawAStar(scalefactor) {
  var r;
  var s;

  modelViewStack.push(modelViewMatrix);
  // draw the full star
  for (var i = 0; i < 3; i++) {
    r = rotate(70, 0, 0, 1);

    modelViewMatrix = mult(modelViewMatrix, r);

    modelViewStack.push(modelViewMatrix); // save the previous MVM
    s = scale4(scalefactor, scalefactor, 0);
    modelViewMatrix = mult(modelViewMatrix, s);
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 6);
    modelViewMatrix = modelViewStack.pop(); // undo the scaling effect

    r = rotate(55, 0, 0, 1);

    modelViewMatrix = mult(modelViewMatrix, r);

    modelViewStack.push(modelViewMatrix); // save the previous MVM
    s = scale4(scalefactor, scalefactor, 0);
    modelViewMatrix = mult(modelViewMatrix, s);
    gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 6);
    modelViewMatrix = modelViewStack.pop(); // undo the scaling effect
  }
  modelViewStack = modelViewStack.pop();
}
function DrawStars() {
  modelViewMatrix = mat4();
  var x, y;
  for (var i = 0; i < 25; i++) {
    if (i % 2 == 0) {
      x = Math.random() * 8; //positive side of x-axis
      y = Math.random() * 6 + 2;
      scaleFactor = 1 / 30;
    } else {
      x = Math.random() * -8; //negative side of x-axis
      y = Math.random() * 6 + 2;
      scaleFactor = 1 / 30;
    }
    if (i % 3 == 0) {
      //change scale for different stars
      scaleFactor = 1 / 20;
    } else if (i % 5 == 0) {
      scaleFactor = 1 / 45;
    }
    t = translate(x, y, 0); //translate random x & y
    modelViewMatrix = t;
    r = rotate(28, 0, 0, 1);
    modelViewMatrix = mult(t, r);
    DrawAStar(scaleFactor);
  }
}

function DrawMountains() {
  modelViewMatrix = mat4();

  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.TRIANGLE_STRIP, 14, 12);
  gl.drawArrays(gl.TRIANGLE_STRIP, 26, 9);
}

function DrawRings() {
  //repeat a new transformation for each off the five rings

  modelViewMatrix = mat4();
  modelViewMatrix = mult(modelViewMatrix, rotate(18, 0, 0, 1));
  modelViewMatrix = mult(modelViewMatrix, translate(-1.5, 6.21, 0));
  modelViewMatrix = mult(modelViewMatrix, scale4(-0.1, 1.3 * 1.618, 1));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));

  gl.drawArrays(gl.LINE_STRIP, 115, 11);

  modelViewMatrix = mat4();
  modelViewMatrix = mult(modelViewMatrix, rotate(-5, 0, 0, 1));
  modelViewMatrix = mult(modelViewMatrix, translate(-3.9, 5.2, 0));
  modelViewMatrix = mult(modelViewMatrix, scale4(-0.1, 1.32 * 1.618, 1));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.LINE_STRIP, 115, 11);

  modelViewMatrix = mat4();
  modelViewMatrix = mult(modelViewMatrix, rotate(18, 0, 0, 1));
  modelViewMatrix = mult(modelViewMatrix, translate(-1.45, 6.21, 0));
  modelViewMatrix = mult(modelViewMatrix, scale4(-0.1, 1.3 * 1.618, 1));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));

  gl.drawArrays(gl.LINE_STRIP, 115, 11);

  modelViewMatrix = mat4();
  modelViewMatrix = mult(modelViewMatrix, rotate(-5.5, 0, 0, 1));
  modelViewMatrix = mult(modelViewMatrix, translate(-3.9, 5.2, 0));
  modelViewMatrix = mult(modelViewMatrix, scale4(-0.1, 1.32 * 1.618, 1));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.LINE_STRIP, 115, 11);

  modelViewMatrix = mat4();
  modelViewMatrix = mult(modelViewMatrix, rotate(4, 0, 0, 1));
  modelViewMatrix = mult(modelViewMatrix, translate(-3, 5.7, 0));
  modelViewMatrix = mult(modelViewMatrix, scale4(-0.055, 1.5 * 1.5, 1));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.LINE_STRIP, 115, 11);
  // modelViewMatrix = mat4();
}

function tree(scale) {
  var s;
  //  modelViewStack.push(modelViewMatrix);
  modelViewStack.push(modelViewMatrix); // save the previous MVM
  s = scale4(scale, scale, 0);
  modelViewMatrix = mult(modelViewMatrix, s);
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.TRIANGLE_FAN, 240, 18);
  modelViewMatrix = modelViewStack.pop(); // undo the scaling effect

  modelViewStack.push(modelViewMatrix); // save the previous MVM
  s = scale4(scale, -scale, 0);
  modelViewMatrix = mult(modelViewMatrix, s);
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.TRIANGLE_FAN, 240, 18);
  modelViewMatrix = modelViewStack.pop(); // undo the scaling effect
  //    modelViewStack=modelViewStack.pop();
}

function DrawTrees() {
  modelViewMatrix = mat4();
  var x, y, scale;
  for (var i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      x = Math.random() * (8 - 3) + 3; //positive side of x-axis
      y = Math.random() * -4;
    } else {
      x = Math.random() * (8 - 0.5) - 0.5; //negative side of x-axis
      y = Math.random() * -1;
    }
    if (i % 4 == 0) {
      //change scale for different stars
      scale = 1 / 3;
      r = rotate(-91, 0, 0, 1); //rotate all trees
    } else if (i % 3 == 0) {
      scale = 1 / 4;
      r = rotate(-89, 0, 0, 1); //rotate all trees
    } else {
      r = rotate(-90, 0, 0, 1); //rotate all trees
      scale = 1 / 2;
    }
    t = translate(x, y, 0); //translate random x & y
    modelViewMatrix = t;
    modelViewMatrix = mult(t, r);
    tree(scale);
  }
  //draw three large trees for background
  r = rotate(-92, 0, 0, 1);
  t = translate(-7.35, 4, 0);
  scale = 1;
  modelViewMatrix = t;
  modelViewMatrix = mult(t, r);
  tree(scale);
  r = rotate(-90, 0, 0, 1);
  t = translate(2, 4, 0);
  scale = 1;
  modelViewMatrix = t;
  modelViewMatrix = mult(t, r);
  tree(scale);
  r = rotate(-90, 0, 0, 1);
  t = translate(3, 2, 0);
  scale = 1 / 1.5;
  modelViewMatrix = t;
  modelViewMatrix = mult(t, r);
  tree(scale);
}

function DrawBow() {
  var r;
  modelViewMatrix = mat4();
  modelViewStack.push(modelViewMatrix);
  modelViewMatrix = mult(modelViewMatrix, translate(0, 2.21, 0));
  modelViewMatrix = mult(modelViewMatrix, scale4(0.7, 1.25, 1));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.TRIANGLE_STRIP, 258, 9);
  modelViewStack.pop();

  r = rotate(180, 0, 0, 1);

  modelViewMatrix = mult(modelViewMatrix, r);

  modelViewStack.push(modelViewMatrix);
  modelViewMatrix = mult(modelViewMatrix, translate(0, 14, 0));
  modelViewMatrix = mult(modelViewMatrix, scale4(1, 1.25, 1));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.LINE_STRIP, 267, 6);
  modelViewStack.pop();

  //gl.drawArrays(gl.LINE_STRIP, 264, )
}

function DrawArrow() {
  modelViewMatrix = mat4();
  var r;
  r = rotate(-90, 0, 0, 1);
  modelViewMatrix = r;

  //first row of feathers
  modelViewStack.push(modelViewMatrix); // save the previous MVM
  s = scale4(1 / 4, 1 / 4, 0);
  modelViewMatrix = mult(modelViewMatrix, translate(5.7, 0, 0));
  modelViewMatrix = mult(modelViewMatrix, s);
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.TRIANGLE_FAN, 273, 13);
  modelViewMatrix = modelViewStack.pop(); // undo the scaling effect

  modelViewStack.push(modelViewMatrix); // save the previous MVM
  s = scale4(1 / 4, -1 / 4, 0);
  modelViewMatrix = mult(modelViewMatrix, translate(5.7, 0, 0));
  modelViewMatrix = mult(modelViewMatrix, s);
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.TRIANGLE_FAN, 273, 13);
  modelViewMatrix = modelViewStack.pop(); // undo the scaling effect

  //body of arrow
  gl.drawArrays(gl.TRIANGLE_FAN, 286, 5);

  //second row of feathers
  modelViewStack.push(modelViewMatrix); // save the previous MVM
  s = scale4(1 / 4, 1 / 4, 0);
  modelViewMatrix = mult(modelViewMatrix, translate(5.3, 0, 0));
  modelViewMatrix = mult(modelViewMatrix, s);
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.TRIANGLE_FAN, 273, 13);
  modelViewMatrix = modelViewStack.pop(); // undo the scaling effect
  modelViewStack.push(modelViewMatrix); // save the previous MVM
  s = scale4(1 / 4, -1 / 4, 0);
  modelViewMatrix = mult(modelViewMatrix, translate(5.3, 0, 0));
  modelViewMatrix = mult(modelViewMatrix, s);
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.TRIANGLE_FAN, 273, 13);
  modelViewMatrix = modelViewStack.pop(); // undo the scaling effect

  modelViewStack.push(modelViewMatrix); // save the previous MVM
  s = scale4(1 / 4, 1 / 4, 0);
  modelViewMatrix = mult(modelViewMatrix, translate(3.3, 0, 0));
  modelViewMatrix = mult(modelViewMatrix, rotate(-22, 0, 0, 1));
  modelViewMatrix = mult(modelViewMatrix, s);
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.TRIANGLE_FAN, 273, 13);
  modelViewMatrix = modelViewStack.pop(); // undo the scaling effect
  modelViewStack.push(modelViewMatrix); // save the previous MVM
  s = scale4(1 / 4, -1 / 4, 0);
  modelViewMatrix = mult(modelViewMatrix, translate(3.3, 0, 0));
  modelViewMatrix = mult(modelViewMatrix, rotate(22, 0, 0, 1));
  modelViewMatrix = mult(modelViewMatrix, s);
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  gl.drawArrays(gl.TRIANGLE_FAN, 273, 13);
  modelViewMatrix = modelViewStack.pop(); // undo the scaling effect
}

function render() {
  gl.clear(gl.COLOR_BUFFER_BIT);
  gl.uniformMatrix4fv(projectionMatrixLoc, false, flatten(projectionMatrix));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));

  // draw ground and sky first
  DrawSkyandGround();
  // draw stars and mountains... next
  DrawStars();
  DrawMountains();
  // then, draw planet, add rings too
  DrawFullPlanet();
  DrawRings();

  //   then, draw ghost
  modelViewMatrix = mat4();
  modelViewMatrix = mult(modelViewMatrix, translate(-6, -2, 0));
  modelViewMatrix = mult(modelViewMatrix, scale4(2, 2, 1));
  gl.uniformMatrix4fv(modelViewMatrixLoc, false, flatten(modelViewMatrix));
  DrawGhost();

  // add other things, like bow, arrow, spider, flower, tree ...
  DrawTrees();
  DrawBow();
  DrawArrow();
}
