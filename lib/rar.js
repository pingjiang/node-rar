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
var unrar = require('bindings')('unrar');

exports.list = function(filepath) {
  var realpath = path.resolve(filepath);
  if (!fs.existsSync(realpath)) {
    return false;
  }
  var entries = [];
  unrar.listArchive(realpath, function(entry) {
    entries.push(entry);
  });
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
  });
  return entries;
};
