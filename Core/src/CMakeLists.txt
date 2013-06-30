cmake_minimum_required(VERSION 2.6)

include_directories("../include")

add_library(CrabGui_Core SHARED CrabGuiEventHandle.cpp CrabGuiScrollBar.cpp CrabGuiButton.cpp CrabGuiFontTexture.cpp CrabGuiScrollWindow.cpp CrabGuiCanvas.cpp CrabGuiFrameWindow.cpp CrabGuiStatic.cpp CrabGuiCombobox.cpp CrabGuiListbox.cpp CrabGuiSystem.cpp CrabGuiDefine.cpp CrabGuiQueue.cpp CrabGuiTreebox.cpp CrabGuiEditbox.cpp CrabGuiRBTree.cpp CrabGuiWindow.cpp)

