{
  "targets": [
    {
      "target_name": "wiringPi",
      "sources": [ "wiringPi.h" ],
      "dependencies": []
    },
    {
      "target_name": "PiLed",
      "sources": [ "pi-led.cpp" ],
      "dependencies": ["bit_array", "wiringPi", "wiringPiSPI", "LedMatrix"]
    },
    {
      "target_name": "LedMatrix",
      "type": "static_library",
      "sources": [ "LedMatrix.cpp" ],
      "dependencies": ["font"]
    },
    {
      "target_name": "font",
      "type": "static_library",
      "sources": [ "font.c" ]
    },
    {
      "target_name": "bit_array",
      "type": "static_library",
      "sources": [ "bit_array.cpp" ]
    }
  ],
  'include_dirs': [
      'wiringpi/wiringPi',
      'wiringpi/devLib',
      "wiringpi/wiringPiSPI"
   ],
  'libraries': [
      '<!(pwd)/wiringpi/wiringPi/libwiringPi.a',
      '<!(pwd)/wiringpi/devLib/libwiringPiDev.a'
  ]
}
