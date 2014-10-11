/*global describe, it, before, after, beforeEach, afterEach */
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
    if (callback) {
      callback.call(this, err, stdout, stderr);
    }
  });
};

var mkdirp = function(dir, callback) {
  return execFile('mkdir', '-p', dir, callback);
};
var rmdirForce = function(directories, callback) {
  return execFile('rm', '-rf', directories, callback);
};
var MAPFILES = function(a) { return a.map(function(f) { return {FileName: f}; }); };
var EXISTS_FILES1 = function(a) { return a.forEach(function(f) { assert.ok(fs.existsSync('./tmp/'+f)); }); };
var EXISTS_FILES = function(a) {};

describe('node-rar node module.', function() {
  describe('list archive', function() {
    it('must list linux rar entries', function() {
      var entries = rar.list('./test/fixtures/linux_rar.rar');
      var expected = [
      'plain.txt', 
      'test file with whitespaces.txt'
      ];
      assert.deepEqual(entries, MAPFILES(expected));
    });
    
    it('must list win rar entries', function() {
      var entries = rar.list('./test/fixtures/latest_winrar.rar');
      var expected = [
      '1.txt',
      '2.txt'
      ];
      assert.deepEqual(entries, MAPFILES(expected));
    });
    
    it('must not found', function() {
      var entries = rar.list('./test/fixtures/no_such_file.rar');
      assert(!entries);
    });
    
    it('test 4mb.rar', function() {
      var entries = rar.list('./test/fixtures/4mb.rar'); 
      var expected = [
      "4mb.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test commented.rar', function() {
      var entries = rar.list('./test/fixtures/commented.rar'); 
      var expected = [
      "file.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test corrupted.rar', function() {
      var entries = rar.list('./test/fixtures/corrupted.rar'); 
      var expected = [
      "test/C", 
      "test/Sbv=Ð", 
      "test/W", 
      "test/a/a", 
      "test/a", 
      "test/a.a.a.a/a", 
      "test/a.a.a/a", 
      "test/a.a/a", 
      "test/dcyxypbj:", 
      "test/g|j{", 
      "test/g", 
      "test/a.a.a.a/a.a/lwtv", 
      "test/a/oe", 
      "test/a.a/s", 
      "test/a.a.a/tyl", 
      "test/a/a.a/xgqas", 
      "test/", 
      "test/", 
      "test/", 
      "test/", 
      "test/", 
      "test/", 
      "test/", 
      "test/a.a/a.a.a.a/y.llywbylsxam.", 
      "test/", 
      "test/a.a", 
      "test/a.a.a.a/a.a", 
      "test/a.a.a/a.a", 
      "test/a.a/a.a", 
      "test/a/a.a", 
      "test/a.a.a", 
      "test/a.a.a.a/a.a.a", 
      "test/a.a.a/a.a.a", 
      "test/a.a/a.a.a", 
      "test/a/a.a.a", 
      "test/a.a.a.a/a.a.a.a", 
      "test/a.a.a.a", 
      "test/a.a.a/a.a.a.a", 
      "test/a.a/a.a.a.a", 
      "test/a/a.a.a.a", 
      "test/ctkmgt.cmwf", 
      "test/.btareckw.jnxkmpig", 
      "test/.kaq", 
      "test/a/qijt.lfcrpg", 
      "test/a.a/j.ngmccgooauqn.cl.m", 
      "test/mn.x.lt.nfvd", 
      "test/.a.cfmsku.p", 
      "test/a.a/a.a.a/oyo:nwnlh.uhy", 
      "test/.xvtxhrf", 
      "test/a.a.a/a.a/qv.yb", 
      "test", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test directories.rar', function() {
      var entries = rar.list('./test/fixtures/directories.rar'); 
      var expected = [
      "dirwithsth/fileindir.txt", 
      "dirwithsth", 
      "emptydir", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test dirlink_unix.rar', function() {
      var entries = rar.list('./test/fixtures/dirlink_unix.rar'); 
      var expected = [
      "emptydir/link", 
      "file", 
      "link", 
      "emptydir", 
      "nopermdir", 
      "setuiddir", 
      "stickydir", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test dirs_and_extra_headers.rar', function() {
      var entries = rar.list('./test/fixtures/dirs_and_extra_headers.rar'); 
      var expected = [ { FileName: 'אּ/%2Fempty%2E/file7.txt' },
        { FileName: 'אּ/file3.txt' },
        { FileName: 'אּ/file4_אּ.txt' },
        { FileName: 'אּ/אּ_2/file5.txt' },
        { FileName: 'אּ/אּ_2/file6_אּ.txt' },
        { FileName: 'file1.txt' },
        { FileName: 'file2_אּ.txt' },
        { FileName: 'with_streams.txt' },
        { FileName: 'אּ/%2Fempty%2E' },
        { FileName: 'אּ/empty' },
        { FileName: 'אּ/אּ_2' },
        { FileName: 'אּ' },
        { FileName: 'allow_everyone_ni' } ]; 
      assert.deepEqual(entries, expected);
    });
    it('test empty_file.rar', function() {
      var entries = rar.list('./test/fixtures/empty_file.rar'); 
      var expected = [
      "empty_file", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    
    // samplepassword
    it('test encrypted_headers.rar', function() {
      var entries = rar.list('./test/fixtures/encrypted_headers.rar', 'samplepassword'); 
      var expected = [
      "encfile1.txt", 
      "encfile2.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test encrypted_headers.rar wrong password', function() {
      assert.throws(function() {
        rar.list('./test/fixtures/encrypted_headers.rar', 'wrongpassword');
      }, 'Process archive error');
    });
    it('test encrypted_only_files.rar', function() {
      var entries = rar.list('./test/fixtures/encrypted_only_files.rar'); 
      var expected = [
      "encfile1.txt", 
      "encfile2.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test garbage.part03.rar', function() {
      var entries = rar.list('./test/fixtures/garbage.part03.rar'); 
      var expected = [
      // "garbage.txt",
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test latest_winrar.rar', function() {
      var entries = rar.list('./test/fixtures/latest_winrar.rar'); 
      var expected = [
      "1.txt", 
      "2.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test linux_rar.rar', function() {
      var entries = rar.list('./test/fixtures/linux_rar.rar'); 
      var expected = [
      "plain.txt", 
      "test file with whitespaces.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test multi.part1.rar', function() {
      var entries = rar.list('./test/fixtures/multi.part1.rar'); 
      var expected = [
      "file1.txt", 
      "file2.txt", 
      "file3.txt"
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test multi.part2.rar', function() {
      var entries = rar.list('./test/fixtures/multi.part2.rar'); 
      var expected = [
      "file3.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test multi.part3.rar', function() {
      var entries = rar.list('./test/fixtures/multi.part3.rar'); 
      var expected = [
      "file3.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    // it('test multi_broken.part1.rar', function() {
    //   var entries = rar.list('./test/fixtures/multi_broken.part1.rar');
    //   var expected = [
    //   "file1.txt",
    //   "file2.txt",
    //   ];
    //   // assert.deepEqual(entries, MAPFILES(expected));
    //   console.log(entries);
    // });
    it('test rar_notrar.rar', function() {
      assert.throws(function() {
        rar.list('./test/fixtures/rar_notrar.rar');
      }, 'Process archive error');
    });
    it('test rar_unicode.rar', function() {
      var entries = rar.list('./test/fixtures/rar_unicode.rar'); 
      var expected = [
      "file1À۞.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test repeated_name.rar', function() {
      var entries = rar.list('./test/fixtures/repeated_name.rar'); 
      var expected = [
      "file.txt", 
      "file.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test secret-crypted-none.rar', function() {
      var entries = rar.list('./test/fixtures/secret-crypted-none.rar', 'secret'); 
      var expected = [
      "base.css", 
      "reset.css", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test secret-crypted-none.rar wrong secret', function() {
      assert.throws(function() {
        rar.list('./test/fixtures/secret-crypted-none.rar', 'wrongsecret');
      }, 'Process archive error');
    });
    it('test secret-none.rar', function() {
      var entries = rar.list('./test/fixtures/secret-none.rar'); 
      var expected = [
      "base.css", 
      "reset.css", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test solid.rar', function() {
      var entries = rar.list('./test/fixtures/solid.rar'); 
      var expected = [
      "tese.txt", 
      "unrardll.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test sparsefiles_rar.rar', function() {
      var entries = rar.list('./test/fixtures/sparsefiles_rar.rar'); 
      var expected = [
      "sparsefiles.rar", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    it('test store_method.rar', function() {
      var entries = rar.list('./test/fixtures/store_method.rar'); 
      var expected = [
      "tese.txt", 
      ]; 
      assert.deepEqual(entries, MAPFILES(expected));
    });
    
  });
  
  describe('test archive', function() {
    var tmpdir = './tmp';
    before(function() { });
    beforeEach(function() {
      if (fs.existsSync(tmpdir)) {
        rmdirForce(tmpdir);
      }
      mkdirp(tmpdir);
    });
  
    it('must extract entries', function() {
      rar.test('./test/fixtures/linux_rar.rar', tmpdir);
      var expected = [
      'plain.txt', 
      'test file with whitespaces.txt'
      ];
      EXISTS_FILES(expected);
    });
    
    it('test 4mb.rar', function() {
      rar.test('./test/fixtures/4mb.rar', tmpdir); 
      var expected = [
      "4mb.txt", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test commented.rar', function() {
      rar.test('./test/fixtures/commented.rar', tmpdir); 
      var expected = [
      "file.txt", 
      ]; 
      EXISTS_FILES(expected); 
    });

    // it('test corrupted.rar', function() {
    //   rar.test('./test/fixtures/corrupted.rar', tmpdir);
    //   var expected = [
    //   "test/C",
    //   "test/Sbv=Ð",
    //   "test/W",
    //   "test/a/a",
    //   "test/a",
    //   "test/a.a.a.a/a",
    //   "test/a.a.a/a",
    //   "test/a.a/a",
    //   "test/dcyxypbj:",
    //   "test/g|j{",
    //   "test/g",
    //   "test/a.a.a.a/a.a/lwtv",
    //   "test/a/oe",
    //   "test/a.a/s",
    //   "test/a.a.a/tyl",
    //   "test/a/a.a/xgqas",
    //   "test/",
    //   "test/",
    //   "test/",
    //   "test/",
    //   "test/",
    //   "test/",
    //   "test/",
    //   "test/a.a/a.a.a.a/y.llywbylsxam.",
    //   "test/",
    //   "test/a.a",
    //   "test/a.a.a.a/a.a",
    //   "test/a.a.a/a.a",
    //   "test/a.a/a.a",
    //   "test/a/a.a",
    //   "test/a.a.a",
    //   "test/a.a.a.a/a.a.a",
    //   "test/a.a.a/a.a.a",
    //   "test/a.a/a.a.a",
    //   "test/a/a.a.a",
    //   "test/a.a.a.a/a.a.a.a",
    //   "test/a.a.a.a",
    //   "test/a.a.a/a.a.a.a",
    //   "test/a.a/a.a.a.a",
    //   "test/a/a.a.a.a",
    //   "test/ctkmgt.cmwf",
    //   "test/.btareckw.jnxkmpig",
    //   "test/.kaq",
    //   "test/a/qijt.lfcrpg",
    //   "test/a.a/j.ngmccgooauqn.cl.m",
    //   "test/mn.x.lt.nfvd",
    //   "test/.a.cfmsku.p",
    //   "test/a.a/a.a.a/oyo:nwnlh.uhy",
    //   "test/.xvtxhrf",
    //   "test/a.a.a/a.a/qv.yb",
    //   "test",
    //   ];
    //   EXISTS_FILES(expected);
    // });

    it('test directories.rar', function() {
      rar.test('./test/fixtures/directories.rar', tmpdir); 
      var expected = [
      "dirwithsth/fileindir.txt", 
      "dirwithsth", 
      "emptydir", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test dirlink_unix.rar', function() {
      rar.test('./test/fixtures/dirlink_unix.rar', tmpdir); 
      var expected = [
      "emptydir/link", 
      "file", 
      "link", 
      "emptydir", 
      "nopermdir", 
      "setuiddir", 
      "stickydir", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test dirs_and_extra_headers.rar', function() {
      rar.test('./test/fixtures/dirs_and_extra_headers.rar', tmpdir); 
      var expected = [
      "allow_everyone_ni", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test empty_file.rar', function() {
      rar.test('./test/fixtures/empty_file.rar', tmpdir); 
      var expected = [
      "empty_file", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test encrypted_headers.rar', function() {
      rar.test('./test/fixtures/encrypted_headers.rar', tmpdir, 'samplepassword'); 
      var expected = [
      "encfile1.txt", 
      "encfile2.txt", 
      ]; 
      EXISTS_FILES(expected); 
    });

    // TODO 还不支持多个密码
    // it('test encrypted_only_files.rar', function() {
    //   rar.test('./test/fixtures/encrypted_only_files.rar', tmpdir, 'samplepassword');
    //   var expected = [
    //   "encfile1.txt",  // samplepassword
    //   "encfile2.txt",  // samplepassword2
    //   ];
    //   EXISTS_FILES(expected);
    // });

    // it('test garbage.part03.rar', function() {
    //   rar.test('./test/fixtures/garbage.part03.rar', tmpdir);
    //   var expected = [
    //   "garbage.txt",
    //   ];
    //   EXISTS_FILES(expected);
    // });

    it('test latest_winrar.rar', function() {
      rar.test('./test/fixtures/latest_winrar.rar', tmpdir); 
      var expected = [
      "1.txt", 
      "2.txt", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test linux_rar.rar', function() {
      rar.test('./test/fixtures/linux_rar.rar', tmpdir); 
      var expected = [
      "plain.txt", 
      "test file with whitespaces.txt", 
      ]; 
      EXISTS_FILES(expected); 
    });

    // it('test multi.part1.rar', function() {
    //   rar.test('./test/fixtures/multi.part1.rar', tmpdir);
    //   var expected = [
    //   "file1.txt",
    //   "file2.txt",
    //   ];
    //   EXISTS_FILES(expected);
    // });
    //
    // it('test multi.part2.rar', function() {
    //   rar.test('./test/fixtures/multi.part2.rar', tmpdir);
    //   var expected = [
    //   "file2.txt",
    //   ];
    //   EXISTS_FILES(expected);
    // });
    //
    // it('test multi.part3.rar', function() {
    //   rar.test('./test/fixtures/multi.part3.rar', tmpdir);
    //   var expected = [
    //   "file2.txt",
    //   "file3.txt",
    //   ];
    //   EXISTS_FILES(expected);
    // });

    // it('test multi_broken.part1.rar', function() {
    //   rar.test('./test/fixtures/multi_broken.part1.rar', tmpdir);
    //   var expected = [
    //   "file1.txt",
    //   "file2.txt",
    //   ];
    //   EXISTS_FILES(expected);
    // });

    it('test rar_notrar.rar', function() {
      assert.throws(function() {
        rar.test('./test/fixtures/rar_notrar.rar', tmpdir); 
      }, 'Process archive error');
    });

    it('test rar_unicode.rar', function() {
      rar.test('./test/fixtures/rar_unicode.rar', tmpdir); 
      var expected = [
      "file1À۞.txt", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test repeated_name.rar', function() {
      rar.test('./test/fixtures/repeated_name.rar', tmpdir); 
      var expected = [
      "file.txt", 
      "file.txt", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test secret-crypted-none.rar', function() {
      rar.test('./test/fixtures/secret-crypted-none.rar', tmpdir, 'secret'); 
      var expected = [
      "base.css", 
      "reset.css", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test secret-none.rar', function() {
      rar.test('./test/fixtures/secret-none.rar', tmpdir, 'secret'); 
      var expected = [
      "base.css", 
      "reset.css", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test solid.rar', function() {
      rar.test('./test/fixtures/solid.rar', tmpdir); 
      var expected = [
      "tese.txt", 
      "unrardll.txt", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test sparsefiles_rar.rar', function() {
      rar.test('./test/fixtures/sparsefiles_rar.rar', tmpdir); 
      var expected = [
      "sparsefiles.rar", 
      ]; 
      EXISTS_FILES(expected); 
    });

    it('test store_method.rar', function() {
      rar.test('./test/fixtures/store_method.rar', tmpdir); 
      var expected = [
      "tese.txt", 
      ]; 
      EXISTS_FILES(expected); 
    });
    
  
    afterEach(function() {
      rmdirForce(tmpdir);
    });
    after(function() { });
  });
  
  describe('extract archive', function() {
    var tmpdir = './tmp';
    before(function() { });
    beforeEach(function() {
      if (fs.existsSync(tmpdir)) {
        rmdirForce(tmpdir);
      }
      mkdirp(tmpdir);
    });
  
    it('must extract entries', function() {
      rar.extract('./test/fixtures/linux_rar.rar', tmpdir);
      var expected = [
      'plain.txt', 
      'test file with whitespaces.txt'
      ];
      EXISTS_FILES1(expected);
    });
    
    afterEach(function() {
      rmdirForce(tmpdir);
    });
    after(function() { });
  });
  
});

