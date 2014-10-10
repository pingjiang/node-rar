describe('test 001', function() {
  it('must be object', function() {
    var archive = rar.open('linux_rar.rar');
    assert.ok(archive);
  });
  it('must be object', function() {
    var archive = rar.open('latest_winrar.rar');
    assert.ok(archive);
  });
  it('must be object', function() {
    var archive = rar.open('no_such_file.rar');
    assert(archive, null);
  });
});
