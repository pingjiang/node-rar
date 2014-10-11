{
  'targets': [
    {
      'target_name': 'unrar',
      'include_dirs': [
          "<!(node -e \"require('nan')\")"
      ],
      'defines': [
          '_FILE_OFFSET_BITS=64',
          '_LARGEFILE_SOURCE',
          'RAR_SMP',
          'RARDLL'
      ],
      'sources': [ 
          'src/binding.cc' 
      ],
      'dependencies': [
        './vendor/unrar/unrar.gyp:unrar'
      ]
    }
  ]
}

