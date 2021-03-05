// Copyright 2020 - 2021
// Author: Ayvar Aleksiev

#include "UndoRedoCommandManager.h"

namespace Base {

//define static members
std::deque<UndoRedoInterface*> UndoRedoCommandManager::_previousUse;
std::deque<UndoRedoInterface*> UndoRedoCommandManager::_futureUse;
std::mutex UndoRedoCommandManager::_mutex;

UndoRedoCommandManager*
UndoRedoCommandManager::getInstance()
{
   static std::unique_ptr<Base::UndoRedoCommandManager> sManagerPtr(
      std::make_unique<UndoRedoCommandManager>());
   return sManagerPtr.get();
}

void UndoRedoCommandManager::undo()
{
   std::scoped_lock lock(_mutex);
   if (_previousUse.empty()) {
      return;
   }
   auto const& undoInstance = _previousUse.back();
   (undoInstance)->undo();
   addToRedoListUnlocked(undoInstance);
   _previousUse.pop_back();
}

void UndoRedoCommandManager::redo()
{
   std::scoped_lock lock(_mutex);
   if (_futureUse.empty()) {
      return;
   }
   auto const& redoInstance = _futureUse.back();
   (redoInstance)->redo();
   // Add to undo list.
   _previousUse.push_back(redoInstance);
   _futureUse.pop_back();
}

void UndoRedoCommandManager::addToUndoList(UndoRedoInterface* instance)
{
   std::scoped_lock lock(_mutex);
   _previousUse.push_back(instance);
   ClearRedoListUnlocked();
}

void UndoRedoCommandManager::removeFromUndoList(UndoRedoInterface* instance)
{
   std::scoped_lock lock(_mutex);
   // Removes all occurrences of instance in undo list. 
   _previousUse.erase(
      std::remove(
         _previousUse.begin(),
         _previousUse.end(),
         instance),
      _previousUse.end());
}

void UndoRedoCommandManager::removeFromRedoList(UndoRedoInterface* instance)
{
   std::scoped_lock lock(_mutex);
   // Removes all occurrences of instance in redo list. 
   _futureUse.erase(
      std::remove(
         _futureUse.begin(),
         _futureUse.end(),
         instance),
      _futureUse.end());
}

void UndoRedoCommandManager::addToRedoListUnlocked(UndoRedoInterface* instance)
{
   _futureUse.push_back(instance);
}

void UndoRedoCommandManager::ClearRedoListUnlocked()
{
   if (!_futureUse.empty()) {
      _futureUse.clear();
   }
}

} // namespace Base