{
  'conditions': [
    ['OS=="win"', {
      "targets": [
        {
          "target_name": "enet",
          "sources": [ "enet.cpp", "lib/Utils.cpp", "lib/Gtps.cpp", "lib/Packet.cpp", "lib/Methods.cpp" ],
          "libraries": [ "-lenet64", "winmm.lib", "ws2_32.lib"  ],
          'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
          'cflags!': [ '-fno-exceptions' ],
          'cflags_cc!': [ '-fno-exceptions' ],
          'msvs_settings': {
            'VCCLCompilerTool': {
              'ExceptionHandling': 1,
              'AdditionalOptions': ['/MT', '/openmp']
            }
          }
        }
      ]
    }]
  ]
}