{
  'targets': [
    {
      'target_name': 'caffeine',
      'conditions': [
        ['OS=="mac"', {
          'sources': [ 'caffeine_mac.cc' ],
          'libraries': [
            '-framework IOKit',
          ]
        }]
      ]
    }
  ]
}
