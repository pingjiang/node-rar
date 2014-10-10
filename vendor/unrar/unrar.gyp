{
  'variables': {
      'STRIP': 'strip'
  },
  'targets': [
    {
      'target_name': 'unrar',
      'type': 'static_library',
      'defines': [
          '_FILE_OFFSET_BITS=64',
          '_LARGEFILE_SOURCE',
          'RAR_SMP',
          'RARDLL'
      ],
      'cflags!': [ '-fexceptions' ],
      'cflags_cc!': [ '-fexceptions' ],
      'conditions': [
          ['OS=="mac"', {
            'xcode_settings': {
              'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
            }
          }],
          ['OS!="win"', {
            'ldflags': [
              '-pthread',
            ],
            'cflags': [
                '-O2'
            ]
          }]
      ],
      'sources': [
        'rar.cpp', 
        'strlist.cpp', 
        'strfn.cpp', 
        'pathfn.cpp', 
        'smallfn.cpp', 
        'global.cpp', 
        'file.cpp', 
        'filefn.cpp', 
        'filcreat.cpp', 
        'archive.cpp', 
        'arcread.cpp', 
        'unicode.cpp', 
        'system.cpp', 
        'isnt.cpp', 
        'crypt.cpp', 
        'crc.cpp', 
        'rawread.cpp', 
        'encname.cpp', 
        'resource.cpp', 
        'match.cpp', 
        'timefn.cpp', 
        'rdwrfn.cpp', 
        'consio.cpp', 
        'options.cpp', 
        'errhnd.cpp', 
        'rarvm.cpp', 
        'secpassword.cpp', 
        'rijndael.cpp', 
        'getbits.cpp', 
        'sha1.cpp', 
        'sha256.cpp', 
        'blake2s.cpp', 
        'hash.cpp', 
        'extinfo.cpp', 
        'extract.cpp', 
        'volume.cpp', 
        'list.cpp', 
        'find.cpp', 
        'unpack.cpp', 
        'headers.cpp', 
        'threadpool.cpp', 
        'rs16.cpp', 
        'cmddata.cpp', 
        'ui.cpp', 
        'filestr.cpp', 
        'scantree.cpp', 
        'dll.cpp', 
        'qopen.cpp'
      ],
      'include_dirs': [
        './',
      ],
      'direct_dependent_settings': {
        'include_dirs': [
          './',
        ],
      },
    }
  ]
}