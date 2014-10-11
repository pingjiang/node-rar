#! /usr/bin/env node

'use strict';

var util = require('util');
var rar;
try {
  rar = require('node-rar');
} catch(e) {
  rar = require('./lib/rar');
}

var userArgs = process.argv;
var len = userArgs.length;
var searchParam = userArgs[2];

function showUsage() {
  console.log('Usage: node-rar [l|t|x] filepath [todir] password');
  process.exit(-1);
}
function showVersion() {
  console.log(require('./package').version);
}
if (userArgs.indexOf('-h') !== -1 || userArgs.indexOf('--help') !== -1 || searchParam === undefined) {
  return showUsage();
}

if (userArgs.indexOf('-v') !== -1 || userArgs.indexOf('--version') !== -1) {
    return showVersion();
}

if (len < 3) {
  return showUsage();
}

var entries;
if (searchParam === 'l') {
  entries = rar.list(userArgs[3], userArgs[4]);
} else if (searchParam === 't') {
  entries = rar.test(userArgs[3], userArgs[4], userArgs[5]);
} else if (searchParam === 'x') {
  entries = rar.extract(userArgs[3], userArgs[4], userArgs[5]);
} else {
  return showUsage();
}

if (entries) {
  console.log(util.inspect(entries));
}
