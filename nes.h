#define NI_IDENT    4
int Nes_HeaderSize = 16;

typedef u_int8_t Nes_Byte;

typedef struct nes_hdr {
  unsigned char n_ident[NI_IDENT];
  Nes_Byte Prg_Size;
  Nes_Byte Chr_Size;
  // TODO: Account for flags 6, 7, etc being nonzero
  // TODO: Account for trainer
  Nes_Byte Flags_6;
  Nes_Byte Flags_7;
  Nes_Byte Prg_Ram_Size;
  Nes_Byte Flags_9;
  Nes_Byte Flags_10;
  Nes_Byte zeros[5];
} Nes_Hdr;
