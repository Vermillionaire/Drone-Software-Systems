var canvas = document.getElementById("renderCanvas");
var engine = new BABYLON.Engine(canvas, true);

console.log("Testst");
$.getJSON( "json/data1", function( data ) {
  var items = [];
  print("Test1");
  $.each( data, function( key, val ) {
    print("Test");
    items.push( "<li id='" + key + "'>" + val + "</li>" );
  });

  $( "<ul/>", {
    "class": "my-new-list",
    html: items.join( "" )
  }).appendTo( "body" );
});

var createScene = function () {
    var scene = new BABYLON.Scene(engine);

    var camera = new BABYLON.ArcRotateCamera("Camera", 3 * Math.PI / 2, Math.PI / 8, 50, BABYLON.Vector3.Zero(), scene);

    camera.attachControl(canvas, true);

    meters = 1
    sp = 100
    ln = sp * meters
    num = ln/sp
    dash = 10
    space = dash * 10
    dnum = 0
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
