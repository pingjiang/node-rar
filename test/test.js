/*global describe,it*/
'use strict';
var assert = require('assert');
var nodeRar = require('../lib/rar.js');

describe('node-rar node module.', function() {
  it('must be world', function() {
    assert( nodeRar.hello(), 'world');
  });
});
