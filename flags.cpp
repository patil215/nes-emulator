#include "flags.h"

bool Flags::getC() {
  return flags[0];
}

bool Flags::getZ() {
  return flags[1];
}

bool Flags::getI() {
  return flags[2];
}

bool Flags::getD() {
  return flags[3];
}

bool Flags::getB() {
  return flags[4];
}

bool Flags::getV() {
  return flags[6];
}

bool Flags::getS() {
  return flags[7];
}

u_int8_t Flags::getCB() {
  return flags[0] ? 1 : 0;
}

u_int8_t Flags::getZB() {
  return flags[1] ? 1 : 0;
}

u_int8_t Flags::getIB() {
  return flags[2] ? 1 : 0;
}

u_int8_t Flags::getDB() {
  return flags[3] ? 1 : 0;
}

u_int8_t Flags::getBB() {
  return flags[4] ? 1 : 0;
}

u_int8_t Flags::getVB() {
  return flags[6] ? 1 : 0;
}

u_int8_t Flags::getSB() {
  return flags[7] ? 1 : 0;
}

void Flags::setC(bool result) {
  flags[0] = result;
}

void Flags::setZ(bool result) {
  flags[1] = result;
}

void Flags::setI(bool result) {
  flags[2] = result;
}

void Flags::setD(bool result) {
  flags[3] = result;
}

void Flags::setB(bool result) {
  flags[4] = result;
}

void Flags::setV(bool result) {
  flags[6] = result;
}

void Flags::setS(bool result) {
  flags[7] = result;
}

void Flags::setCB(u_int8_t result) {
  flags[0] = (result != 0);
}

void Flags::setZB(u_int8_t result) {
  flags[1] = (result != 0);
}

void Flags::setIB(u_int8_t result) {
  flags[2] = (result != 0);
}

void Flags::setDB(u_int8_t result) {
  flags[3] = (result != 0);
}

void Flags::setBB(u_int8_t result) {
  flags[4] = (result != 0);
}

void Flags::setVB(u_int8_t result) {
  flags[6] = (result != 0);
}

void Flags::setSB(u_int8_t result) {
  flags[7] = (result != 0);
}

void Flags::autoSetS(u_int8_t result) {
  if(((int8_t) result) < 0) {
    flags[7] = true;
  } else {
    flags[7] = false;
  }
}

void Flags::autoSetZ(u_int8_t result) {
  if(result == 0) {
    flags[1] = true;
  } else {
    flags[1] = false;
  }
}