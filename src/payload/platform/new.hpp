#pragma once
#include <common/Common.h>

// Declare Heap class pre-emptively
namespace EGG { class Heap; }
void* operator new(size_t size, int align);
void* operator new(size_t size, EGG::Heap* heap, int align);
void* operator new[](size_t size, int align);
void* operator new[](size_t size, EGG::Heap* heap, int align);

// Implement placement new
inline void* operator new(size_t size, void* buffer) { return buffer; }
inline void* operator new[](size_t size, void* buffer) { return buffer; }
