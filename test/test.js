/*global describe,it*/
'use strict';
var assert = require('assert');
var fs = require('fs');
var rar = require('../lib/rar.js');
var child = require('child_process');

var execFile = function(cmd, arg, directories, callback) {
  if(typeof directories === 'string') {
    directories = [directories];
  }
  var args = directories;
  args.unshift(arg);
  child.execFile(cmd, args, {env:process.env}, function(err, stdout, stderr) {
    callback && callback.apply(this, arguments);
  });
};

var mkdirp = function(dir, callback) {
  return execFile('mkdir', '-p', dir, callback);
};
var rmdirForce = function(directories, callback) {
  return execFile('rm', '-rf', directories, callback);
};


describe('node-rar node module.', function() {
  it('must be world', function() {
    assert( rar.hello(), 'world');
  });
  
  describe('list archive', function() {
    it('must list entries', function() {
      var entries = rar.list('./test/fixtures/linux_rar.rar');
      var expected = [
        {FileName: 'plain.txt'},
        {FileName: 'test file with whitespaces.txt'}
      ];
      assert.deepEqual(entries, expected);
    });
  });
  
  describe('extract archive', function() {
    before(function() {
      if (fs.existsSync('tmp')) {
        rmdirForce('tmp');
      }
      fs.mkdirSync('tmp');
    });
  
    it('must extract entries', function() {
      rar.extract('./test/fixtures/linux_rar.rar', './tmp/');
      ['plain.txt', 'test file with whitespaces.txt'].forEach(function(f) {
        assert.ok(fs.existsSync('./tmp/'+f));
      });
    });
  
    after(function() {
      rmdirForce('tmp');
    });
  });
  
});

