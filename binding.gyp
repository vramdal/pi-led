{
  'targets': [
    {
      'target_name': 'piLedAddon',
      'sources': [
        'src/pi-led/font.c',
        'src/pi-led/bit_array.cpp',
        'src/pi-led/font.c',
        'src/pi-led/LedMatrix.cpp',
        'src/pi-led/pi-led.cpp'#,

       # 'src/wiringPi.cc',
       # 'src/wiringPiSPI.cc' #,
        #'src/wpi.cc'
      ],
      'include_dirs': [
        'wiringpi/wiringPi',
        'wiringpi/devLib'
      ],
      'libraries': [
        '<!(pwd)/wiringpi/wiringPi/libwiringPi.a',
        '<!(pwd)/wiringpi/devLib/libwiringPiDev.a'
      ],
      'cflags': [
        '-Wall'
      ]
    }
  ]
}
