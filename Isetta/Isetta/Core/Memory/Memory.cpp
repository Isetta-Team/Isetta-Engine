#include "Core/Memory/Memory.h"
#include <sstream>
#include "Core/Debug/Assert.h"
#include "Core/Debug/Logger.h"

namespace Isetta {

std::string HexFromPtr(const PtrInt rawAddress) {
  std::stringstream str;
  str << "Allocated Memory Address: " << rawAddress << " = " << std::hex
      << std::uppercase << rawAddress;
  return str.str();
}

void* MemoryManager::AllocateUnaligned(const SizeInt size) {
  // looks like std::malloc always return 16 byte aligned memory
  return std::malloc(size);
}

void MemoryManager::FreeUnaligned(void* mem) { std::free(mem); }

void* MemoryManager::AllocateAligned(const SizeInt size, const U8 alignment) {
  const bool isValid = alignment >= 2 && alignment <= 128 &&
                       (alignment & (alignment - 1)) == 0;  // power of 2
  if (!isValid) {
    throw std::invalid_argument("Illegal alignment in allocator");
  }

  const SizeInt expandedSize = size + alignment;

  const PtrInt rawAddress =
      reinterpret_cast<PtrInt>(AllocateUnaligned(expandedSize));
  // std::string add = HexFromPtr(rawAddress);
  const PtrInt misAlignment = rawAddress & (alignment - 1);
  const U8 adjustment = alignment - static_cast<U8>(misAlignment);
  const PtrInt alignedAddress = rawAddress + adjustment;

  U8* alignedMemory = reinterpret_cast<U8*>(alignedAddress);
  alignedMemory[-1] = adjustment;

  ASSERT(reinterpret_cast<PtrInt>(alignedMemory) % alignment == 0);
  return static_cast<void*>(alignedMemory);
}

void MemoryManager::FreeAligned(void* memoryPtr) {
  const U8* alignedMemory = reinterpret_cast<U8*>(memoryPtr);
  const PtrDiff adjustment = static_cast<PtrDiff>(alignedMemory[-1]);
  const PtrInt alignedAddress = reinterpret_cast<PtrInt>(memoryPtr);
  const PtrInt rawAddress = alignedAddress - adjustment;
  void* rawMem = reinterpret_cast<void*>(rawAddress);
  FreeUnaligned(rawMem);
}

StackAllocator::StackAllocator(const SizeInt stackSize)
    : top(0), length(stackSize) {
  bottom = MemoryManager::AllocateUnaligned(stackSize);
  bottomAddress = reinterpret_cast<PtrInt>(bottom);
}

void* StackAllocator::AllocAligned(const SizeInt size, const U8 alignment) {
  PtrInt rawAddress = bottomAddress + top;
  PtrInt misAlignment = rawAddress & (alignment - 1);
  PtrDiff adjustment = alignment - misAlignment;
  PtrInt alignedAddress = rawAddress + adjustment;
  Marker newTop = top + size + adjustment;

  if (newTop > length) {
    // TODO: should I throw an exception here?
    throw std::overflow_error("Not enough memory in stack allocator");
  }

  top = newTop;

  return reinterpret_cast<void*>(alignedAddress);
}

void* StackAllocator::AllocUnaligned(SizeInt size) {
  Marker newTop = top + size;

  if (newTop > length) {
    // TODO: should I throw an exception here?
    throw std::overflow_error("Not enough memory in stack allocator");
  }

  void* mem = reinterpret_cast<void*>(bottomAddress + top);
  top = newTop;
  return mem;
}

void StackAllocator::Erase() {
  Clear();
  std::free(bottom);
}

}  // namespace Isetta
