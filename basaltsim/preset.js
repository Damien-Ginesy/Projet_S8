function preset(x){
  stop();
  switch (x) {
    case 'light':
      nodeNumber = 100;
      maliciousRatio = 0.1;
      maliciousNumber = parseInt(maliciousRatio*nodeNumber);
      viewSize = 24;
      resetCooldown = 6;
      cyclesPerSecond = 4;
      resetNumber = 3;
      break;
    case 'heavy':
      nodeNumber = 1000;
      maliciousRatio = 0.1;
      maliciousNumber = parseInt(maliciousRatio*nodeNumber);
      viewSize = 50;
      resetCooldown = 22;
      cyclesPerSecond = 1;
      resetNumber = 22;
      break;
  }
  setParamHTML();
}
