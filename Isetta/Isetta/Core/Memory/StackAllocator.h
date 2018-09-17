#pragma once
#include "Core/IsettaAlias.h"

namespace Isetta {
class StackAllocator {
 public:
  typedef SizeInt Marker;

  StackAllocator() = default;
  explicit StackAllocator(SizeInt stackSize);
  ~StackAllocator() = default;

  /**
   * \brief You probably want to manually call constructor after getting this
   * memory otherwise the new object may not function properly \param
   * sizeInBytes Number of bytes you want \return pointer to the allocated
   * memory
   */
  void* Alloc(SizeInt size, U8 alignment = 16);
  void* AllocUnaligned(SizeInt size);

  /**
   * \brief If you are using this, you probably need to call the destructor on
   * your own
   * \tparam T
   * \return
   */
  template <typename T>
  T* New() {
    void* mem = Alloc(sizeof(T));
    return new (mem) T();
  };

  template <typename T>
  T* New(Marker& outMarker) {
    outMarker = top;
    return New<T>();
  };

  void FreeToMarker(const Marker marker) { top = marker; };
  void Clear() { top = 0; };
  void Erase();
  Marker GetMarker() const { return top; };

 private:
  Marker top;
  SizeInt length;
  void* bottom;
  PtrInt bottomAddress;
};
}  // namespace Isetta
