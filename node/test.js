var FileClassificator = require("./FileClassificator");

var movedFiles = FileClassificator.Classificate(
  "../wallpaper",
  "../newDir",
  function (x, y, z) {
    console.log("Current State: %s, Progress %f%, Info: %s", x, y * 100, z);
  }
);

console.log(movedFiles);
