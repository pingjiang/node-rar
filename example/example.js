'use strict';

var rar = require('node-rar');

/// list archive entries
rar.list('path/to/file.rar', 'optional password');
// => [entries]

/// test archive entries
rar.test('path/to/file.rar', 'dir/to/test', 'optional password');
// => [entries]

/// extract archive entries
rar.extract('path/to/file.rar', 'dir/to/extract', 'optional password');
// => [entries]