// Copyright 2020
// Author: Ayvar Aleksiev

#pragma once
#include "UndoRedoState.h"
#include <list>
#include <memory>
#include <shared_mutex>

namespace Common {

// Synchronized undo/redo string.
class UndoRedoString
{
public:
   // Constructors
   explicit UndoRedoString(std::string const& str = "");
   explicit UndoRedoString(const char* cstr);
   explicit UndoRedoString(std::string&& str);

   // Destructor.
   ~UndoRedoString();
   
   // Copy constructor
   UndoRedoString(UndoRedoString const& other);

   // Copy assignment operator.
   UndoRedoString& operator=(UndoRedoString const& other);

   // Move constructor
   UndoRedoString(UndoRedoString&& other) noexcept;

   // Move assignment operator
   UndoRedoString& operator=(UndoRedoString&& other) noexcept;

   // @throws out_range exception if position is greater than the string's length.
   void insert(size_t pos, const std::string& str);
   
   // @throws out_range exception if position is greater than the string's length.
   void replace(size_t position, size_t len, const std::string& str);

   // Erases the contents of UndoRedoString which becomes an empty UndoRedoString.
   void clear();
   
   bool isEmpty() const;
   
   bool compare(const std::string& str) const;

   std::string getString() const;
private:
   mutable std::shared_mutex _mutex;

   std::shared_ptr<Base::UndoRedoState<std::string>> _state {
      std::make_shared<Base::UndoRedoState<std::string>>() };
};

} // namespace Common

