struct Hook_TESConditionItem
{
    static bool IsTrue(RE::TESConditionItem* a_this, RE::ConditionCheckParams* a_params)
    {
        auto tls = RE::GetStaticTLSData();
        tls->consoleMode = false;

        return _IsTrue(a_this, a_params);
    }

    static inline REL::Relocation<decltype(IsTrue)> _IsTrue;

    static void Install()
    {
        REL::Relocation target1{ REL_ID(24131, 24633), REL_OF(0x76, 0x76, 0) };
        _IsTrue = target1.write_call<5>(IsTrue);
        REL::Relocation target2{ REL_ID(29078, 29895), REL_OF(0xDF, 0xDF, 0) };
        target2.write_call<5>(IsTrue);
        REL::Relocation target3{ REL_ID(29078, 29895), REL_OF(0x16C, 0x16C, 0) };
        target3.write_call<5>(IsTrue);
        REL::Relocation target4{ REL_ID(29089, 29923), REL_OF(0x1E, 0x1E, 0) };
        target4.write_call<5>(IsTrue);
    }
};

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse);
    SKSE::AllocTrampoline(14);

    Hook_TESConditionItem::Install();

    return true;
}
