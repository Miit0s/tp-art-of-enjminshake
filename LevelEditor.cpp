#include "LevelEditor.hpp"
#include "imgui.h"

LevelEditor::LevelEditor()
{
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::drawLevelEditorGui() {
	if (ImGui::Begin("Level Editor##Default")) {
		ImGui::Text("Test");
	}
	ImGui::End();
}