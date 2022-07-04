#include "AES.h"

int main(){
    string text = "00000000000000000000000000020418", key = "40b64fe0da0c2fa1b85f5eb6587e5863b2c51e58";
    aes(text, key);
    aes(text, key);
    text = "2129f46fd8f67aceaa92ee17e187a29a";
    in_aes(text, key);
}