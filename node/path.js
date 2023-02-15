var path = require("path");
var FileClassificator = path
  .resolve("../FileClassificator.lib")
  .replace(/\\/gm, "\\\\");

module.exports = {
  FileClassificator: FileClassificator,
};
