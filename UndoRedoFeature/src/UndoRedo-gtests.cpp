// Copyright 2021
// Author: Ayvar Aleksiev

#include "gtest/gtest.h"
#include "UndoRedoState.h"
#include "UndoRedoString.h"


/// Tests covering the basic functionallity of undo/redo feature.

namespace {
   std::string const kStringName("String");
   std::string const kInsert = "Insert";
   std::string const kReplace = "Replace";
} 

TEST(UndoRedoTests, UndoRedoStateTest)
{
   // Test usage 1
   Base::UndoRedoState<std::string> testObj1;
   testObj1.addUndoState(kStringName);
   testObj1.addUndoState(kInsert);
   EXPECT_TRUE(testObj1.canUndo());
   testObj1.undo();
   EXPECT_EQ(testObj1.getCurrentState(), kStringName);
   EXPECT_TRUE(testObj1.canRedo());
   testObj1.redo();
   EXPECT_EQ(testObj1.getCurrentState(), kInsert);
   testObj1.clear();
   EXPECT_FALSE(testObj1.canUndo());
   EXPECT_FALSE(testObj1.canRedo());

   // Test usage 2
   Base::UndoRedoState<std::string> testObj2;
   testObj2.addUndoState(kStringName);
   testObj2.addUndoState(kInsert);
   EXPECT_TRUE(testObj2.canUndo());
   Base::UndoRedoCommandManager::undo();
   EXPECT_EQ(testObj2.getCurrentState(), kStringName);
   EXPECT_TRUE(testObj2.canRedo());
   Base::UndoRedoCommandManager::redo();
   EXPECT_EQ(testObj2.getCurrentState(), kInsert);
   testObj2.clear();
   EXPECT_FALSE(testObj2.canUndo());
   EXPECT_FALSE(testObj2.canRedo());

   // Test usage 3
   Base::UndoRedoState<std::string> testObj3;
   testObj3.addUndoState(kStringName);
   testObj3.clear();
   EXPECT_THROW(testObj3.getCurrentState(), std::logic_error);
}

TEST(UndoRedoTests, UndoRedoString_UndoTest)
{
   // Test constructor
   Common::UndoRedoString testObj(kStringName);
   EXPECT_EQ(testObj.getString(), kStringName);

   // Test copy constructor
   Common::UndoRedoString testObj2(testObj);
   EXPECT_EQ(testObj2.getString(), kStringName);

   // Test copy assignment operator
   Common::UndoRedoString testObj3;
   testObj3 = testObj;
   EXPECT_EQ(testObj3.getString(), kStringName);

   // Test insert operation
   testObj.insert(0, kInsert);
   EXPECT_EQ(testObj.getString(), kInsert + kStringName);

   // Test replace operation
   std::string contentAfterReplace = kStringName;
   contentAfterReplace.replace(0, 0, kReplace);
   testObj2.replace(0, 0, kReplace);
   EXPECT_EQ(testObj2.getString(), contentAfterReplace);

   // Test clear operation
   testObj3.clear();
   EXPECT_TRUE(testObj3.isEmpty());

   // Test Undo operation
   Base::UndoRedoCommandManager::undo();
   EXPECT_EQ(testObj3.getString(), kStringName);

   Base::UndoRedoCommandManager::undo();
   EXPECT_EQ(testObj2.getString(), kStringName);
}

TEST(UndoRedoTests, UndoRedoString_RedoTest)
{
   // Test usage 1
   Common::UndoRedoString testObj(kStringName);
   testObj.insert(0, kInsert);
   Base::UndoRedoCommandManager::undo();
   Base::UndoRedoCommandManager::redo();
   EXPECT_EQ(testObj.getString(), kInsert + kStringName);

   // Test usage 2
   Common::UndoRedoString testObj2(kStringName);
   std::string insert1 = "INSERT1";
   testObj2.insert(0, insert1);
   Base::UndoRedoCommandManager::undo();
   std::string insert2 = "INSERT2";
   testObj2.insert(0, insert2);
   // Changes done after an undo operation should reset redo states.
   // There should be no redo states available - all cleared after insert operation.
   Base::UndoRedoCommandManager::redo();
   EXPECT_EQ(testObj2.getString(), insert2 + kStringName);
}