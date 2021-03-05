// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once
#include "UndoRedoInterface.h"
#include "UndoRedoCommandManager.h"
#include <list>
#include <shared_mutex>
#include <stdexcept>

namespace Base {

template<typename Type>
class UndoRedoState final : public UndoRedoInterface
{
public:
   // Constructor
   UndoRedoState() = default;

   // Destructor
   ~UndoRedoState() = default;

   // Copy constructor.
   UndoRedoState(UndoRedoState const& other);
   
   // Assignment operator
   UndoRedoState& operator=(UndoRedoState const& other);

   // Move constructor
   UndoRedoState(UndoRedoState&& other) noexcept;

   // Move assignment operator
   UndoRedoState& operator=(UndoRedoState&& other) noexcept;

   // UndoRedoInterface API
   void undo() override;
   void redo() override;

   // Removes all undo/redo states.
   // The object is no loger part of the undo/redo list of objects.
   void clear();

   // Returns true if there are states available for undo.
   bool canUndo() const;

   // Returns true if there are states available for redo.
   bool canRedo() const;

   template<typename... Args>
   void addUndoState(Args&&... args)
   {
      std::scoped_lock writeLock(_mutex);
      _undoStates.emplace_back(std::forward<Args>(args)...);
      if (_undoStates.size() > 1) {
         _undoRedoManager->addToUndoList(this);
      }
      // Clear redo states.
      // Note: If other changes are done after an undo operation, redo is reset and unavailable.
      _redoStates.clear();
   }

   // Returns the last state of the object that is currently being used.
   //
   // @throws std::logic error
   //        If there are no states are available.
   Type getCurrentState() const;

private:
   /// Contains instances of previous use.
   std::list<Type> _undoStates;
   
   /// Contains instances that can be used for redo operation.
   std::list<Type> _redoStates;
   
   /// Mutex
   mutable std::shared_mutex  _mutex;

   UndoRedoCommandManager *_undoRedoManager { UndoRedoCommandManager::getInstance() };
};

//
// UndoRedoState class definition
//

template <typename Type>
UndoRedoState<Type>::UndoRedoState(UndoRedoState const& other)
{
   std::shared_lock<std::shared_mutex> readLock(other._mutex);
   _undoStates = other._undoStates;
   _redoStates = other._redoStates;
   _undoRedoManager = other._undoRedoManager;
}

template <typename Type>
UndoRedoState<Type>&
UndoRedoState<Type>::operator=(UndoRedoState const& other)
{
   std::unique_lock writeLock(_mutex, std::defer_lock);
   std::shared_lock readLock(other._mutex, std::defer_lock);
   std::lock(writeLock, readLock);
   if (this != &other) {
      _undoStates = other._undoStates;
      _redoStates = other._redoStates;
      _undoRedoManager = other._undoRedoManager;
   }
   return *this;
}

template <typename Type>
UndoRedoState<Type>::UndoRedoState(UndoRedoState&& other) noexcept
{
   std::shared_lock<std::shared_mutex> readLock(other._mutex);
   _undoStates = std::move(other._undoStates);
   _redoStates = std::move(other._redoStates);
   _undoRedoManager = other._undoRedoManager;
}

template <typename Type>
UndoRedoState<Type>&
UndoRedoState<Type>::operator=(UndoRedoState&& other) noexcept
{
   std::unique_lock writeLock(_mutex, std::defer_lock);
   std::shared_lock readLock(other._mutex, std::defer_lock);
   std::lock(writeLock, readLock);
   if (this != &other) {
      _undoStates = std::move(other._undoStates);
      _redoStates = std::move(other._redoStates);
      _undoRedoManager = other._undoRedoManager;
   }
   return *this;
}

template <typename Type>
void
UndoRedoState<Type>::undo()
{
   std::scoped_lock writeLock(_mutex);
   if (_undoStates.size() <= 1) {
      return;
   }
   _redoStates.push_back(_undoStates.back());
   _undoStates.pop_back();
}

template<typename Type>
void
UndoRedoState<Type>::redo()
{
   std::scoped_lock writeLock(_mutex);
   if (_redoStates.empty()) {
      return;
   }
   _undoStates.push_back(_redoStates.back());
   _redoStates.pop_back();
}

template<typename Type>
void
UndoRedoState<Type>::clear()
{
   std::scoped_lock writeLock(_mutex);
   if (_undoStates.empty() && _redoStates.empty()) {
      return;
   }
   _undoStates.clear();
   _redoStates.clear();
   // removes all occurances of instance from undo, redo containers.
   _undoRedoManager->removeFromUndoList(this);
   _undoRedoManager->removeFromRedoList(this);
}

template<typename Type>
Type
UndoRedoState<Type>::getCurrentState() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   if (_undoStates.empty()) {
      throw std::logic_error("No state available");
   }
   return _undoStates.back();
}

template<typename Type>
bool
UndoRedoState<Type>::canUndo() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return _undoStates.size() > 1;
}

template<typename Type>
bool
UndoRedoState<Type>::canRedo() const
{
   std::shared_lock<std::shared_mutex> readLock(_mutex);
   return !_redoStates.empty();
}

} // namespace Base