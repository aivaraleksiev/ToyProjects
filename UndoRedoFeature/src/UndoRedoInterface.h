// Copyright 2020 - 2021
// Author: Ayvar Aleksiev

#pragma once

namespace Base {

// Interface for classes implementing undo/redo feature.
class UndoRedoInterface
{
public:
   // Revert to previous state.
   virtual void undo() = 0;

   // Reverse undo operation.
   // Redo is available only when an undo operation is called before that.
   virtual void redo() = 0;

   // Virtual destructor.
   virtual ~UndoRedoInterface() = default;
};

} // namespace Base
