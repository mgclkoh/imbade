#ifndef MY_PASS_H
#define MY_PASS_H

// This class only can be apllied to encrypting and decrypting of visible ASCII characters less than 255 bytes long.
// Chip ID and compile TIME is involved in the process.
// Data encrypted in this hardware can be only decrypted on this hardware.

#define BAUD 115200
#include <AESLib.h>

AESLib aesLib;

char cleartext[256];
char ciphertext[512];

// AES Encryption Key

byte aes_key[] = { 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30 };

// General initialization vector (you must use your own IV's in production for full security!!!)
byte aes_iv[N_BLOCK] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

byte enc_iv[N_BLOCK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // iv_block gets written to, provide own fresh copy...
byte dec_iv[N_BLOCK] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

//class MyPass : public AESLib {
class MyPass {
public:
  MyPass();

  void begin();
  
  String encHW(const String& msg);
  String decHW(const String& msg);
  String asciiOnly(String str);

  String encrypt_impl(char * msg, byte iv[]);
  String decrypt_impl(char * msg, byte iv[]);

  bool fst;
  
};

///*
MyPass::MyPass() {
  
}

void MyPass::begin() {  
  
  aesLib.gen_iv(aes_iv);
  //encrypt_impl(strdup(plaintext.c_str()), aes_iv);

  aesLib.set_paddingmode(paddingMode::CMS);

  char b64in[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  char b64out[base64_enc_len(sizeof(aes_iv))];
  base64_encode(b64out, b64in, 16);

  char b64enc[base64_enc_len(10)];
  base64_encode(b64enc, (char*) "0123456789", 10);

  char b64dec[ base64_dec_len(b64enc, sizeof(b64enc))];
  base64_decode(b64dec, b64enc, sizeof(b64enc));

}

String MyPass::encHW(const String& msg) {
  
  if (msg.length() < 1) {
    return msg;
  }

  for (int i = 0; i < 16; i++) {
    enc_iv[i] = 0;
    dec_iv[i] = 0;
  }

  strlcpy(cleartext,msg.c_str(),sizeof(cleartext));
  String encrypted = encrypt_impl(cleartext, enc_iv);

  return encrypted;
}

String MyPass::decHW(const String& msg) {

  if (msg.length() < 1) {
    return msg;
  }

  for (int i = 0; i < 16; i++) {
    enc_iv[i] = 0;
    dec_iv[i] = 0;
  }

  strlcpy(ciphertext,msg.c_str(),sizeof(ciphertext));
  String decrypted = decrypt_impl(ciphertext, dec_iv);
  decrypted = asciiOnly(decrypted);
  
  return decrypted;
}

String MyPass::encrypt_impl(char * msg, byte iv[]) {
  int msgLen = strlen(msg);
  char encrypted[2 * msgLen] = {0};
  aesLib.encrypt64((const byte*)msg, msgLen, encrypted, aes_key, sizeof(aes_key), iv);
  return String(encrypted);
}

String MyPass::decrypt_impl(char * msg, byte iv[]) {
  int msgLen = strlen(msg);
  char decrypted[msgLen] = {0}; // half may be enough
  aesLib.decrypt64(msg, msgLen, (byte*)decrypted, aes_key, sizeof(aes_key), iv);
  return String(decrypted);
}

String MyPass::asciiOnly(String INstr) {
  char buf[256];
  size_t len = INstr.length();
  strlcpy(buf,INstr.c_str(),sizeof(buf));
 
  if (len > sizeof(buf)-1) {
    len = sizeof(buf)-1;
  }

  for (int i = len-1; i >= 0; i--) {
    if (buf[i] > 126 || buf[i] < 32 ) {
      buf[i] = '\0';
    }
    else {
      break;
    }
  } 

  String OUTstr = String(buf);
  return OUTstr;
}
//*/
#endif //#ifndef MY_PASS_H
