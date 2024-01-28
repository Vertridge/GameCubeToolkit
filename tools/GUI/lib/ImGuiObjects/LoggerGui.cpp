#include "ImGuiObjects/LoggerGui.h"

// stl
#include <sstream>

namespace UIObjects {

void LoggerGui::OnInit() {}

void LoggerGui::OnBeginDraw() {
  ImGui::Begin("Log Console");
  // Options menu
  if (ImGui::BeginPopup("Options")) {
    ImGui::Checkbox("Auto-scroll", &mAutoScroll);
    ImGui::EndPopup();
  }

  // Main window
  if (ImGui::Button("Options"))
    ImGui::OpenPopup("Options");
  ImGui::SameLine();
  bool clear = ImGui::Button("Clear");
  ImGui::SameLine();
  bool copy = ImGui::Button("Copy");
  ImGui::SameLine();
  // Filter.Draw("Filter", -100.0f);

  ImGui::Separator();
  ImGui::BeginChild("scrolling", ImVec2(0, 0), false,
                    ImGuiWindowFlags_HorizontalScrollbar);
  if (clear) {
    Clear();
  }
  if (copy) {
    ImGui::LogToClipboard();
  }
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  const char *buf = mBuf.begin();
  const char *buf_end = mBuf.end();

  // Start drawning the log
  ImGuiListClipper clipper;
  clipper.Begin(mLineOffsets.Size);
  while (clipper.Step()) {
    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd;
         line_no++) {
      const char *line_start = buf + mLineOffsets[line_no];
      const char *line_end = (line_no + 1 < mLineOffsets.Size)
                                 ? (buf + mLineOffsets[line_no + 1] - 1)
                                 : buf_end;
      ImGui::TextUnformatted(line_start, line_end);
    }
  }
  clipper.End();
  ImGui::PopStyleVar();

  if (mAutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
    ImGui::SetScrollHereY(1.0f);
  }
  ImGui::EndChild();
}

void LoggerGui::OnEndDraw() { ImGui::End(); }

void LoggerGui::AddToBuffer(const std::string &str) {
  int old_size = mBuf.size();
  mBuf.append(str.c_str());
  for (int new_size = mBuf.size(); old_size < new_size; old_size++) {
    if (mBuf[old_size] == '\n') {
      mLineOffsets.push_back(old_size + 1);
    }
  }
}

void LoggerGui::Log(Verbosity verbosity, const std::string &str) {
  std::stringstream ss;
  switch (verbosity) {
  case Verbosity::ERROR:
    ss << "[ERROR] " << str << "\n";
    break;
  case Verbosity::INFO:
    ss << "[INFO] " << str << "\n";
    break;
  case Verbosity::WARN:
    ss << "\033[1;33m"
       << "[WARNING] " << str << "\n";
    break;
  case Verbosity::TRACE:
    ss << "[TRACE] " << str << "\n";
    break;
  }

  AddToBuffer(ss.str());
}

void LoggerGui::Clear() {
  mBuf.clear();
  mLineOffsets.clear();
  mLineOffsets.push_back(0);
}

} // namespace UIObjects
