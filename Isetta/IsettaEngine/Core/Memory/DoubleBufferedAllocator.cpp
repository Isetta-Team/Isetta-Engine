/*
 * Copyright (c) 2018 Isetta
 */
#include "Core/Memory/DoubleBufferedAllocator.h"

namespace Isetta {

DoubleBufferedAllocator::DoubleBufferedAllocator(const Size size)
    : stacks{StackAllocator{size}, StackAllocator{size}}, curStackIndex(0) {}

void* DoubleBufferedAllocator::Alloc(const Size size, const U8 alignment) {
  return stacks[curStackIndex].Alloc(size, alignment);
}

void DoubleBufferedAllocator::SwapBuffer() { curStackIndex = !curStackIndex; }

void DoubleBufferedAllocator::ClearCurrentBuffer() {
  stacks[curStackIndex].Clear();
}

}  // namespace Isetta
