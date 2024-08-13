//
// Created by Syl on 12/08/2024.
//
#include <mostly_harmless/gui/mostlyharmless_IEditor.h>
namespace mostly_harmless::gui {
   void IEditor::setGuiToProcQueue(marvin::containers::fifos::SPSC<events::GuiToProcParamEvent>* queue) {
       m_guiToProcQueue = queue;
   }
}