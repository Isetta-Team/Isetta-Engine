/*
* Copyright (c) 2018 Isetta
*/
#include "Core/Memory/DoubleBufferedAllocator.h"

namespace Isetta {

DoubleBufferedAllocator::DoubleBufferedAllocator(const SizeInt size)
    : stacks{StackAllocator{size}, StackAllocator{size}}, curStackIndex(0) {
}

DoubleBufferedAllocator::~DoubleBufferedAllocator() {
  // TODO(YIDI): Not sure if this is done automatically when then go out of scope
  // stacks[0].Erase();
  // stacks[1].Erase();
}

void* DoubleBufferedAllocator::Alloc(const SizeInt size, const U8 alignment) {
  return stacks[curStackIndex].AllocAligned(size, alignment);
}

void DoubleBufferedAllocator::SwapBuffer() {
  curStackIndex = !curStackIndex;
}

void DoubleBufferedAllocator::ClearCurrentBuffer() {
  stacks[curStackIndex].Clear();
}

void DoubleBufferedAllocator::Erase() {
  stacks[0].Erase();
  stacks[1].Erase();
}

}  // namespace Isetta
