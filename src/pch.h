#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace logs = SKSE::log;
using namespace std::literals;

#define REL_ID(se, ae) REL::RelocationID(se, ae)
#define REL_OF(se, ae, vr) REL::VariantOffset(se, ae, vr)
