// Copyright 2021
// Author: Ayvar Aleksiev

#pragma once
#include "UndoRedoInterface.h"
#include <deque>
#include <mutex>

namespace Base
{

// Manages classes implemeting undo/redo feature.
class UndoRedoCommandManager {
public:
   UndoRedoCommandManager() = default;

   // Access to the singleton instance of this class.
   static UndoRedoCommandManager* getInstance();

   // Revert the last changed object to previous state.
   // Does undo operation.
   static void undo();

   // Reverse undo operation.
   // Note: If other changes are done after an undo operation, redo is reset and unavailable.
   static void redo();

   // Add object to undo list.
   // Note: Resets redo list.
   //
   // @param[in] instance The instace that have been last changed.
   void addToUndoList(UndoRedoInterface* instance);

   // Removes all occurances of instance from container.
   //
   // @param[in] instance The instace to be removed.
   void removeFromUndoList(UndoRedoInterface* instance);

   // Removes all occurances of instance from container.
   //
   // @param[in] instance The instace to be removed.
   void removeFromRedoList(UndoRedoInterface* instance);

   UndoRedoCommandManager(UndoRedoCommandManager const&) = delete;
   UndoRedoCommandManager& operator=(UndoRedoCommandManager const&) = delete;
private:
   // Add to redo list the last object that was affected by undo operation
   //
   // @param[in] instance The instace that have been last changed.
   static void addToRedoListUnlocked(UndoRedoInterface* instance);

   // Clears all objects from redo list.
   void ClearRedoListUnlocked();

   /// Contains instances of previous use.
   static std::deque<UndoRedoInterface*> _previousUse;

   /// Contains instances that can be used for redo operation.
   static std::deque<UndoRedoInterface*> _futureUse;

   /// Mutex.
   static std::mutex _mutex;
};

} // namespace Base