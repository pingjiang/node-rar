/*global describe,it*/
'use strict';
var assert = require('assert');
var rar = require('../lib/rar.js');

describe('node-rar node module.', function() {
  it('must be world', function() {
    assert( rar.hello(), 'world');
  });
});
