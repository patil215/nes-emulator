#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "nes.h"


class Inst {
  public:
    enum Type { LSR };
    enum Admode { ABSOLUTE };
    void execute();
    Inst (Type, Admode);
    Type type;
    Admode admode;
};

Inst::Inst(Type type_t, Admode admode_t) {
  type = type_t;
  admode = admode_t;
}




void emulate(unsigned char * prg, unsigned char * chr) {

}


int main(int argc, char * argv[]) {
  // Open file
  char * filename = argv[1];
  FILE * file = fopen(filename, "rb");

  // Read file
  char * buffer;
  long lSize;
  fseek (file, 0, SEEK_END);
  lSize = ftell (file);
  rewind (file);
  buffer = (char * ) malloc(sizeof(char) * lSize);
  fread(buffer, 1, lSize, file);

  Nes_Hdr * header = (Nes_Hdr *) (buffer);

  // Convert sizes to # bytes
  int Prg_Size = header->Prg_Size * 16384;
  int Chr_Size = header->Chr_Size * 8192;


  // TODO: Account for trainer
  unsigned char * prg = (unsigned char *) malloc(sizeof(unsigned char) * Prg_Size);
  unsigned char * chr = (unsigned char *) malloc(sizeof(unsigned char) * Chr_Size);

  // Read into PRG and CHR rom
  for (int i = 0; i < Prg_Size; i++) {
    prg[i] = buffer[i + Nes_HeaderSize];
  }
  for (int i = 0; i < Chr_Size; i++) {
    chr[i] = buffer[i + Nes_HeaderSize + Prg_Size];
  }

  emulate(prg, chr);
}

