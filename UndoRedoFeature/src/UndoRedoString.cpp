// Copyright 2020 - 2021
// Author: Ayvar Aleksiev

#include "UndoRedoString.h"

namespace Common {

UndoRedoString::UndoRedoString(const std::string& str)
{
   _state->addUndoState(str);
}

UndoRedoString::UndoRedoString(const char* cstr)
{
   _state->addUndoState(cstr);
}

UndoRedoString::UndoRedoString(std::string&& str)
{
   _state->addUndoState(std::move(str));
}

UndoRedoString::~UndoRedoString()
{
   _state->clear();
}

UndoRedoString::UndoRedoString(UndoRedoString const& other)
{
   std::shared_lock readLock(other._mutex);
   _state = std::make_shared<Base::UndoRedoState<std::string>>(*other._state);
}

UndoRedoString&
UndoRedoString::operator=(UndoRedoString const& other)
{
   std::unique_lock writeLock(_mutex, std::defer_lock);
   std::shared_lock readLock(other._mutex, std::defer_lock);
   std::lock(writeLock, readLock);
   if (this != &other) {
      _state = std::make_shared<Base::UndoRedoState<std::string>>(*other._state);
   }
   return *this;
}

UndoRedoString::UndoRedoString(UndoRedoString&& other) noexcept
{
   std::shared_lock readLock(other._mutex);
   _state = other._state;
}

UndoRedoString&
UndoRedoString::operator=(UndoRedoString&& other) noexcept
{
   std::unique_lock writeLock(_mutex, std::defer_lock);
   std::shared_lock readLock(other._mutex, std::defer_lock);
   std::lock(writeLock, readLock);
   if (this != &other) {
      _state = other._state;
   }
   return *this;
}

void
UndoRedoString::insert(size_t position, const std::string& str)
{

   std::scoped_lock writeLock(_mutex);
   auto currentString = _state->getCurrentState();
   currentString.insert(position, str);
   _state->addUndoState(std::move(currentString));
}

void
UndoRedoString::replace(size_t position, size_t len, const std::string& str)
{
   std::scoped_lock writeLock(_mutex);
   auto currentString = _state->getCurrentState();
   currentString.replace(position, len, str);
   _state->addUndoState(std::move(currentString));
}

void
UndoRedoString::clear()
{
   std::scoped_lock writeLock(_mutex);
   auto currentString = _state->getCurrentState();
   currentString.clear();
   _state->addUndoState(std::move(currentString));
}

bool
UndoRedoString::isEmpty() const
{
   std::shared_lock readLock(_mutex);
   auto currentString = _state->getCurrentState();
   return currentString.empty();
}

bool
UndoRedoString::compare(std::string const& str) const
{
   std::shared_lock readLock(_mutex);
   auto currentString = _state->getCurrentState();
   return currentString == str;
}

std::string UndoRedoString::getString() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return _state->getCurrentState();
}

} // namespace Common
