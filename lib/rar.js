/*
 * 
 * https://github.com/pingjiang/node-rar
 *
 * Copyright (c) 2014 pingjiang
 * Licensed under the MIT license.
 */

'use strict';

var fs = require('fs');
var path = require('path');
var util = require('util');

var unrar = require('bindings')('unrar');

exports.hello = function() {
  return unrar.hello();
};

exports.list = function(filepath) {
  var realpath = path.resolve(filepath);
  if (!fs.existsSync(realpath)) {
    return false;
  }
  var entries = [];
  unrar.listArchive(realpath, function(entry) {
    entries.push(entry);
    // console.log(util.inspect(entry));
  });
  // console.log(util.inspect(archive));
  return entries;
};

exports.extract = function(filepath, toDir) {
  var realpath = path.resolve(filepath);
  var realDir = path.resolve(toDir);
  if (!fs.existsSync(realpath)) {
    return false;
  }
  var entries = [];
  unrar.extractArchive(realpath, realDir, function(entry) {
    entries.push(entry);
    // console.log(util.inspect(entry));
  });
  // console.log(util.inspect(archive));
  return entries;
};
