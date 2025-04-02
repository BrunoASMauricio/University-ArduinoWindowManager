// interrupt pins for known boards

// Teensy and maybe others automatically define this info
#if !defined(CORE_INT0_PIN) && !defined(CORE_INT1_PIN) && !defined(CORE_INT2_PIN)&& !defined(CORE_INT3_PIN)

// Chipkit Uno32 (untested)
#elif defined(__PIC32MX__) && defined(_BOARD_UNO_)
  #define CORE_INT0_PIN  38
  #define CORE_INT1_PIN  2
  #define CORE_INT2_PIN  7
  #define CORE_INT3_PIN  8
  #define CORE_INT4_PIN  35

  #endif
 
