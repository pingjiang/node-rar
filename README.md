#  Node Rar Addon [![Build Status](https://secure.travis-ci.org/pingjiang/node-rar.png?branch=master)](http://travis-ci.org/pingjiang/node-rar)

Node Rar Addon for reading RAR archives using the bundled UnRAR library.

This extension uses a modified version of the UnRAR library. The UnRAR library
is an official open-source library by RARLabs, an auto generated subset of the
RAR codebase. It is available from http://www.rarlab.com/rar_add.htm
Please note that it has a more restrictive license than the NodeJS bindings,
barring using it to re-create the RAR compression algorithm. See
vendor/unrar/LICENSE.txt for details.

Some modifications have been applied to the UnRAR library, mainly to allow
streaming extraction of files without using threads.

## Getting Started

Install the module with: `npm install node-rar`

```js
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
```

Install with cli command

```sh
$ npm install -g node-rar
$ node-rar --help
$ node-rar --version
```

## Documentation

在开发一个Web字幕的应用的时候需要解压rar字幕文件，但是NodeJS没有rar解压的库。
有几个rar解压的库仅仅是封装了unrar命令行工具实现的，在很多的云环境上都用不起来。
所以我花了几天的时间利用unrar的源码开发了一个NodeJS插件。
插件的实现参考了php-rar的代码，采用了php-rar的测试用例。
插件目前还只是简单封装了unrar的接口，后面如果有需要可以在此基础上扩展。

## Todo

- [x] 实现rar文件查看，测试和解压。
- [ ] 支持分卷解压，多文件密码等。
- [x] 兼容支持NodeJS的云平台。
- [x] 发布到NPM上。
- [ ] 兼容Windows，Linux和MacOSX。


## Credits

* Unrar - http://www.rarlab.com/rar_add.htm
* php-rar - https://github.com/cataphract/php-rar

## Contributing

In lieu of a formal styleguide, take care to maintain the existing coding style. Add unit tests for any new or changed functionality. Lint and test your code using [Grunt](http://gruntjs.com).


## License

Copyright (c) 2014 pingjiang  
Licensed under the MIT license.
