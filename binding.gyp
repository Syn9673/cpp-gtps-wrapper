{
  "targets": [
    {
      "target_name": "gtps",
      "sources": [
      	"main.cc",
        "lib/HandleFunc.cc",
        "lib/Functions/Packets/Packets.cc",
        "lib/Functions/Host/Host.cc",
        "lib/Structs/GamePacket.hpp",
        "<!@(node -p \"const fs = require('fs');let files = fs.readdirSync(`${__dirname}/lib/Utils`).filter(f=>fs.statSync(`${__dirname}/lib/Utils/${f}`).isFile() && f.endsWith('.cc')).map(e => `${__dirname}/lib/Utils/${e}`);files.join(' ');\")"
      ],
     	'include_dirs': [
     		"<!@(node -p \"require('node-addon-api').include\")",
     	],
     	'libraries': [
     		"-lenet64",
     		"winmm.lib",
     		"ws2_32.lib"
      ],
     	'msvs_settings': {
    		'VCCLCompilerTool': {
          'ExceptionHandling': 1,
          'AdditionalOptions': ['/std:c++latest']
        }
  		} 
    }
  ]
}