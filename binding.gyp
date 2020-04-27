{
  "targets": [
    {
      "target_name": "enet",
      "sources": [ "enet.cpp", "lib/Utils.cpp", "lib/Gtps.cpp" ],
      "libraries": [ "-lenet64", "winmm.lib", "ws2_32.lib"  ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'cflags': [
        '-std=c++17'
      ],
  		'xcode_settings': {
    		'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
    		'CLANG_CXX_LIBRARY': 'libc++',
    		'MACOSX_DEPLOYMENT_TARGET': '10.7',
  		},
  		'msvs_settings': {
   			'VCCLCompilerTool': { 'ExceptionHandling': 1 },
  		},
    }
  ]
}