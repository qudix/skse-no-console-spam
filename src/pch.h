#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace WinAPI = SKSE::WinAPI;
namespace logs = SKSE::log;
namespace stl = SKSE::stl;
namespace fs = std::filesystem;
using namespace std::literals;

#define REL_ID(se, ae) REL::RelocationID(se, ae)
#define REL_OF(se, ae, vr) REL::VariantOffset(se, ae, vr)
