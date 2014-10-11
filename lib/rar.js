/*
 * 
 * https://github.com/pingjiang/node-rar
 *
 * Copyright (c) 2014 pingjiang
 * Licensed under the MIT license.
 */

'use strict';

var util = require('util');
var extend = util._extend;
var fs = require('fs');
var path = require('path');
var unrar = require('bindings')('unrar');

var processArchive = exports.processArchive = function(filepath, options) {
  var realpath = path.resolve(filepath);
  if (!fs.existsSync(realpath)) {
    return false;
  }
  var defaults = {
    openMode: 0,
    filepath: realpath
  };
  var opts = extend(defaults, options);
  // console.log('process archive with %s', util.inspect(opts));
  var entries = [];
  unrar.processArchive(opts, function(entry) {
    entries.push(entry);
  });
  return entries;
};

exports.list = function(filepath, password) {
  return processArchive(filepath, {
    password: password
  });
};

exports.test = function(filepath, toDir, password) {
  return processArchive(filepath, {
    openMode: 1,
    test: true,
    toDir: toDir ? path.resolve(toDir) : null,
    password: password
  });
};

exports.extract = function(filepath, toDir, password) {
  return processArchive(filepath, {
    openMode: 1,
    toDir: toDir ? path.resolve(toDir) : null,
    password: password
  });
};
