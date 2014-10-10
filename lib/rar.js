/*
 * 
 * https://github.com/pingjiang/node-rar
 *
 * Copyright (c) 2014 pingjiang
 * Licensed under the MIT license.
 */

'use strict';

var unrar = require('bindings')('unrar');

exports.hello = function() {
  return unrar.hello();
};
