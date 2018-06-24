{
  'targets': [
    {
      'target_name': 'piLedAddon',
      'sources': [
        'src/pi-led/font.c',
        'src/pi-led/bit_array.cpp',
        'src/pi-led/LedMatrix.cpp',
        'src/pi-led/pi-led.cpp',
        'wiringpi/wiringPi/softPwm.c',
        'wiringpi/wiringPi/softTone.c'

       # 'src/wiringPi.cc',
       # 'src/wiringPiSPI.cc' #,
        #'src/wpi.cc'
      ],
      'include_dirs': [
        'wiringpi/wiringPi',
        'wiringpi/devLib'
      ],
      'cflags': [
        '-Wno-narrowing'
      ]
    }
  ]
}
