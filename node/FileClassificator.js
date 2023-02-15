var fs = require("fs");

/** Custom Error: RuntimeError */
function RuntimeError(message) {
  this.name = "RuntimeError";
  this.message = message || "Error running the current script";
  this.stack = new Error().stack;
}

var _f = function () {};
_f.prototype = Error.prototype;
RuntimeError.prototype = new _f();
RuntimeError.prototype.constructor = RuntimeError;
/** */

if (!fs.existsSync("./FileClassificator.dll")) {
  new RuntimeError('Cannot find runtime library "FileClassificator"');
}

var nativeBinding = require("./FileClassificator.node");

/**
 * Description of the function
 * @name Progresser
 * @function
 * @param {string} info - Current state info
 * @param {number} ratio - Current progress ratio in precentage
 * @param {string} calculateText - Current file calculate text
 */
/**
 * Classificate the duplicate file to the specific directory
 * @param {string} srcDir - Directory which contains files that you want to specific
 * @param {string} moveTo - Directory name you want to move the duplicate file to, it will create a new directory when the path is not exists
 * @param {Progresser=} progresserCallback - (Optimal) get the classifier progress
 * @returns {string[]} - String array contains which files moved to new directory
 */
var Classificate = function (srcDir, moveTo, progresserCallback) {
  if (!fs.existsSync(srcDir)) {
    new Error("Source directory not exists");
  }

  if (!fs.lstatSync(srcDir).isDirectory()) {
    new Error("Source directory not a vailed directory");
  }

  if (typeof progresserCallback == "function") {
    return nativeBinding.Classificate(srcDir, moveTo, progresserCallback);
  }

  return nativeBinding.Classificate(srcDir, moveTo);
};

/**
 * Get the SHA-3 512 string hex value of a file
 * @param {string} srcFile - The source file to calculate SHA-3 512 hex value, must not be a directory
 * @returns {string} - The SHA-3 512 hex value of the file
 */
var GetSHA512StringValue = function (srcFile) {
  if (!fs.existsSync(srcFile)) {
    new Error("Source file not exists");
  }

  if (fs.lstatSync(srcFile).isDirectory()) {
    new Error("Source file cannot be a directory");
  }

  return nativeBinding.GetSHA512StringValue(srcFile);
};

module.exports = {
  Classificate: Classificate,
  GetSHA512StringValue: GetSHA512StringValue,
};
