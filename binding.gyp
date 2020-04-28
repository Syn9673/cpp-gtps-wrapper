{
  'conditions': [
    ['OS=="win"', {
      "targets": [
        {
          "target_name": "enet",
          "sources": [ "enet.cpp",
            "<!@(node -p \"const fs = require('fs');let files = fs.readdirSync(`${__dirname}/lib/others`).filter(f=>fs.statSync(`${__dirname}/lib/others/${f}`).isFile() && f.endsWith('.cpp')).map(e => `${__dirname}/lib/others/${e}`);files.join(' ');\")",
            "<!@(node -p \"const fs = require('fs');let files = fs.readdirSync(`${__dirname}/lib`).filter(f=>fs.statSync(`${__dirname}/lib/${f}`).isFile() && f.endsWith('.cpp')).map(e => `${__dirname}/lib/${e}`);files.join(' ');\")"
          ],
          "libraries": [ "-lenet64", "winmm.lib", "ws2_32.lib"  ],
          'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
          #'cflags!': [ '-fno-exceptions' ],
          #'cflags_cc!': [ '-fno-exceptions' ],
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