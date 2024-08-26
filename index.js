var canvas = document.getElementById("canvas");
canvas.width = window.innerWidth;
canvas.height = window.innerHeight;

// point emscripten to canvas
var Module = {};
Module.canvas = canvas;
