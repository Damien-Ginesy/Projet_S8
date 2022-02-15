function preset(x){
  switch (x) {
    case 0:
      nodeNumber = 100;
      maliciousRatio = 0.1;
      maliciousNumber = parseInt(maliciousRatio*nodeNumber);
      viewSize = 24;
      resetCooldown = 6;
      cyclesPerSecond = 4;
      resetNumber = 3;
      break;
    case 1:
      nodeNumber = 1000;
      maliciousRatio = 0.1;
      maliciousNumber = parseInt(maliciousRatio*nodeNumber);
      viewSize = 50;
      resetCooldown = 22;
      cyclesPerSecond = 4;
      resetNumber = 22;
      break;
  }
  updateParam();
}
