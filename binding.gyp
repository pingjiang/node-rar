{
  'targets': [
    {
      'target_name': 'unrar',
      'include_dirs': [
          "<!(node -e \"require('nan')\")"
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

