#ifndef IMAGES_H
#define IMAGES_H

#define start_width 33
#define start_height 24
const  uint8_t start_bits[]   = {
  0xc0, 0x01, 0x00, 0x07, 0x00, 0xc0, 0x01, 0x00, 0x07, 0x00, 0xc0, 0x01, 0x00, 0x07, 0x00,
  0x00, 0x0e, 0xe0, 0x00, 0x00, 0x00, 0x0e, 0xe0, 0x00, 0x00, 0x00, 0x0e, 0xe0, 0x00, 0x00,
  0xc0, 0xff, 0xff, 0x07, 0x00, 0xc0, 0xff, 0xff, 0x07, 0x00, 0xc0, 0xff, 0xff, 0x07, 0x00,
  0xf8, 0xf1, 0x1f, 0x3f, 0x00, 0xf8, 0xf1, 0x1f, 0x3f, 0x00, 0xf8, 0xf1, 0x1f, 0x3f, 0x00,
  0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0x01, 0xff, 0xff, 0xff, 0xff, 0x01,
  0xc7, 0xff, 0xff, 0xc7, 0x01, 0xc7, 0xff, 0xff, 0xc7, 0x01, 0xc7, 0xff, 0xff, 0xc7, 0x01,
  0xc7, 0x01, 0x00, 0xc7, 0x01, 0xc7, 0x01, 0x00, 0xc7, 0x01, 0xc7, 0x01, 0x00, 0xc7, 0x01,
  0x00, 0x7e, 0xfc, 0x00, 0x00, 0x00, 0x7e, 0xfc, 0x00, 0x00, 0x00, 0x7e, 0xfc, 0x00, 0x00
};

#define photo_width 16
#define photo_height 16
const  uint8_t photo_bits[]   = {
  0x00, 0x00, 0x00, 0x00, 0xf6, 0xff, 0xff, 0xc7, 0x9f, 0xc7, 0xff, 0xff, 0xc1, 0x83, 0x61,
  0x86, 0x31, 0x8c, 0x31, 0x8c, 0x61, 0x86, 0xc1, 0x83, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
  0x00, 0x00
};

#define close_width 32
#define close_height 32
const  uint8_t close_bits[]   = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x0f,
  0x00, 0x00, 0xf8, 0x1f, 0x00, 0x00, 0x1c, 0x38, 0x00, 0x00, 0x0e, 0x70, 0x00, 0x00, 0x07,
  0xe0, 0x00, 0x00, 0x03, 0xc0, 0x00, 0x80, 0x03, 0xc0, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
  0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xc0, 0xff, 0xff, 0x03, 0xe0, 0xff, 0xff, 0x07,
  0xf0, 0xff, 0xff, 0x0f, 0xf0, 0xff, 0xff, 0x0f, 0xf0, 0xff, 0xff, 0x0f, 0xf0, 0x7f, 0xfc,
  0x0f, 0xf0, 0x3f, 0xf8, 0x0f, 0xf0, 0x3f, 0xf9, 0x0f, 0xe0, 0x3f, 0xf8, 0x07, 0xe0, 0x7f,
  0xfc, 0x07, 0xe0, 0x7f, 0xfc, 0x03, 0xc0, 0x7f, 0xfc, 0x03, 0xc0, 0x7f, 0xfc, 0x03, 0x80,
  0xff, 0xfe, 0x01, 0x00, 0xff, 0xff, 0x00, 0x00, 0xfc, 0x7f, 0x00, 0x00, 0xf8, 0x3f, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define temper_width 10
#define temper_height 12
const  uint8_t temper_bits[]   = {
  0x38, 0x00, 0x44, 0x00, 0xd4, 0x01, 0x54, 0x00, 0xd4, 0x01, 0x54, 0x00, 0xd4, 0x01, 0x54,
  0x00, 0xba, 0x00, 0xba, 0x00, 0xc6, 0x00, 0x7c, 0x00
};

#define humidity_width 10
#define humidity_height 12
const  uint8_t humidity_bits[]   = {
  0x30, 0x00, 0x30, 0x00, 0x78, 0x00, 0x48, 0x00, 0xcc, 0x00, 0x84, 0x00, 0x92, 0x01, 0x3a,
  0x01, 0x22, 0x01, 0x86, 0x01, 0xfc, 0x00, 0x78, 0x00
};

#define fan_width 16
#define fan_height 16
const  uint8_t fan_bits[]   = {
  0xcf, 0xf3, 0x1f, 0xf8, 0xbf, 0xfd, 0x3f, 0xfc, 0x3e, 0x7c, 0x7c, 0x3e, 0xe1, 0x87, 0xc5,
  0xa3, 0xc5, 0xa3, 0xe1, 0x87, 0x7c, 0x3e, 0x3e, 0x7c, 0x3f, 0xfc, 0xbf, 0xfd, 0x1f, 0xf8,
  0xcf, 0xf3
};

#define fork_width 11
#define fork_height 11
const  uint8_t fork_bits[]   = {
  0x00, 0x00, 0x38, 0x00, 0x78, 0x00, 0xff, 0x00, 0xf8, 0x01, 0xf8, 0x07, 0xf8, 0x01, 0xff,
  0x00, 0x78, 0x00, 0x38, 0x00, 0x00, 0x00
};

#define bat_width 11
#define bat_height 11
const  uint8_t bat_bits[]   = {
  0x00, 0x00, 0x00, 0x00, 0xfe, 0x07, 0xfe, 0x07, 0xf7, 0x07, 0x63, 0x04, 0xf7, 0x07, 0xfe,
  0x07, 0xfe, 0x07, 0x00, 0x00, 0x00, 0x00
};

#define swoff_width 15
#define swoff_height 7
const uint8_t swoff_bits[]   = {
  0x00, 0x02, 0x00, 0x01, 0x80, 0x00, 0x40, 0x00, 0x27, 0x70, 0x3f, 0x7e, 0x07, 0x70
};

#define swon_width 15
#define swon_height 3
const uint8_t swon_bits[]   = {
  0xe7, 0x73, 0x3f, 0x7e, 0x07, 0x70
};

#define sms_width 11
#define sms_height 11
const uint8_t sms_bits[]   = {
 0x00,0x00,0xff,0x07,0x03,0x06,0x05,0x05,0x89,0x04,0x71,0x04,0x89,0x04,0x05,
 0x05,0x03,0x06,0xff,0x07,0x00,0x00 };

#define sms1_width 11
#define sms1_height 11
const uint8_t sms1_bits[]   = {
 0x00,0x00,0xff,0x07,0x03,0x06,0x75,0x05,0x11,0x04,0x77,0x07,0x41,0x04,0x75,
 0x05,0x03,0x06,0xff,0x07,0x00,0x00 };

#define email_width 11
#define email_height 11
const uint8_t email_bits[]   = {
 0x00,0x00,0xff,0x07,0x03,0x06,0x75,0x05,0x11,0x04,0x37,0x07,0x11,0x04,0x75,
 0x05,0x03,0x06,0xff,0x07,0x00,0x00 };

#define preview_width 11
#define preview_height 11
const uint8_t preview_bits[]   = {
 0x00,0x00,0x00,0x00,0xfc,0x00,0x86,0x01,0x33,0x03,0x79,0x06,0x33,0x03,0x86,
 0x01,0xfc,0x00,0x00,0x00,0x00,0x00 };

#define phone_width 11
#define phone_height 11
const uint8_t phone_bits[]   = {
 0xfe,0x03,0x02,0x02,0xfa,0x02,0xfa,0x02,0xfa,0x02,0x02,0x02,0xaa,0x02,0x02,
 0x02,0xaa,0x02,0x02,0x02,0xfe,0x03 };

#define notify_width 11
#define notify_height 11
const uint8_t notify_bits[]   = {
 0x03,0x00,0x01,0x03,0x0c,0x02,0xc4,0x00,0x80,0x00,0x20,0x00,0x08,0x00,0x18,
 0x01,0x82,0x01,0x06,0x04,0x00,0x06 };


#endif // IMAGES_H