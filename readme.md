# Iniziare

## Visualizzare 10 particelle posizionate casualmente sullo stage.

testApp.cpp

```c++
void testApp::setup(){
    ofBackground(0); // sfondo nero
    particleSystem.init(10); // crea 10 particelle
    particleSystem.initGoofyNoise(); // setup the movement with noise
    particleSystem.moveNoise = true; // start the movement
}

void testApp:update(){
    particleSystem.update();
}

void testApp:draw(){
    particleSystem.draw();
    particleSystem.goofyPerlinNoise.draw(); // show the noise movement
}
```
