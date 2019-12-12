#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include "basic.h"
#include "ObjLoader.h"
#include "ScanlineZBuffer.h"

void display(int argc, char ** argv, int _eyePos[3], int _centerPos[3], Object &obj, ScanlineZBufferProcessor &Processor);

#endif // !DISPLAY_H
