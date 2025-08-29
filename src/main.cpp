struct Hook_TESConditionItem
{
    static bool IsTrue(RE::TESConditionItem* a_this, RE::ConditionCheckParams* a_params)
    {
        auto tls = RE::GetStaticTLSData();
        tls->consoleMode = false;

        return _IsTrue1(a_this, a_params);
    }

    static inline REL::Hook _IsTrue1{ "IsTrue1", REL::ID(24633), 0x076, IsTrue };
    static inline REL::Hook _IsTrue2{ "IsTrue2", REL::ID(29895), 0x0DF, IsTrue };
    static inline REL::Hook _IsTrue3{ "IsTrue3", REL::ID(29895), 0x16C, IsTrue };
    static inline REL::Hook _IsTrue4{ "IsTrue4", REL::ID(29923), 0x01E, IsTrue };
};

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
    SKSE::Init(a_skse, { .trampoline = true, .trampolineSize = 14 });

    return true;
}
