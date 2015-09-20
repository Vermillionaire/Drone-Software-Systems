var canvas = document.getElementById("renderCanvas");
var engine = new BABYLON.Engine(canvas, false);

console.log(window.location.href);


function Get(url){
  var Httpreq = new XMLHttpRequest(); // a new request
  Httpreq.open("GET",url,false);
  Httpreq.send(null);
  return Httpreq.responseText;
};

var data = JSON.parse(Get("json/data2.json"));
console.log(data);

console.log(data.data[0].point.x);



var createScene = function () {
    var scene = new BABYLON.Scene(engine);

    var camera = new BABYLON.FreeCamera("FreeCamera", BABYLON.Vector3.Zero(), scene);

    camera.attachControl(canvas, true);

    meters = 1
    sp = 100
    ln = sp * meters
    num = ln/sp
    dash = 10
    space = dash * 10
    dnum = 0

    for (i = 0; i<10000; i++) {
      var box = BABYLON.Mesh.CreateBox("box", 1.0, scene);
      box.position.x = data.data[i].point.x;
      box.position.y = data.data[i].point.y;
      box.position.z = data.data[i].point.z;
    }

    for (i = -num; i <= num; i++){
      for (j = -num; j <= num; j++) {
        BABYLON.Mesh.CreateDashedLines("lines", [
          new BABYLON.Vector3(-ln, i * sp, j * sp),
          new BABYLON.Vector3(ln, i * sp, j * sp)],
                                       dash, space, dnum,
                                       scene);
      }
    }

    for (i = -num; i <= num; i++){
      for (j = -num; j <= num; j++) {
        BABYLON.Mesh.CreateDashedLines("lines", [
          new BABYLON.Vector3(j * sp, i * sp, -ln),
          new BABYLON.Vector3(j * sp, i * sp, ln)],
                                       dash, space, dnum,
                                       scene);
      }
    }

    for (i = -num; i <= num; i++){
      for (j = -num; j <= num; j++) {
        BABYLON.Mesh.CreateDashedLines("lines", [
          new BABYLON.Vector3(i * sp, -ln, j * sp),
          new BABYLON.Vector3(i * sp, ln, j * sp)],
                                       dash, space, dnum,
                                       scene);
      }
    }


    return scene;
}

var scene = createScene();

engine.runRenderLoop(function () {
   scene.render();
});

// Resize
window.addEventListener("resize", function () {
    engine.resize();
});
