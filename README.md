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
var node-rar = require('node-rar');
node-rar.awesome(); // "awesome"
```

Install with cli command

```sh
$ npm install -g node-rar
$ node-rar --help
$ node-rar --version
```




## Documentation

_(Coming soon)_


## Examples

_(Coming soon)_


## Credits

* Unrar - http://www.rarlab.com/rar_add.htm
* php-rar - https://github.com/cataphract/php-rar

## Contributing

In lieu of a formal styleguide, take care to maintain the existing coding style. Add unit tests for any new or changed functionality. Lint and test your code using [Grunt](http://gruntjs.com).


## License

Copyright (c) 2014 pingjiang  
Licensed under the MIT license.
