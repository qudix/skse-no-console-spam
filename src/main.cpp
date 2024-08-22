#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

void InitLogging()
{
    auto path = logs::log_directory();
    if (!path)
        return;

    const auto plugin = SKSE::PluginDeclaration::GetSingleton();
    *path /= std::format("{}.log", plugin->GetName());

    std::vector<spdlog::sink_ptr> sinks{ 
        std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true), 
        std::make_shared<spdlog::sinks::msvc_sink_mt>() 
    };

    auto logger = std::make_shared<spdlog::logger>("global", sinks.begin(), sinks.end());
    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);

    spdlog::set_default_logger(std::move(logger));
    spdlog::set_pattern("[%^%L%$] %v");
}

struct ConsoleLogHook
{
    static void Print(RE::ConsoleLog* a_log, const char* a_fmt, ...)
    {
        std::va_list args;
        va_start(args, a_fmt);

        const auto ui = RE::UI::GetSingleton();
        const auto player = RE::PlayerCharacter::GetSingleton();
        if (ui->IsMenuOpen(RE::Console::MENU_NAME) && player->Is3DLoaded()) {
            a_log->VPrint(a_fmt, args);
        }

        va_end(args);
    }

    static void Hook(REL::RelocationID a_id, REL::VariantOffset a_offset)
    {
        REL::Relocation target{ a_id, a_offset };
        target.write_call<5>(Print);
    }

    static void Install()
    {
        //        [SE] [AE]         [SE] [AE] [VR]      [FID] [N] [Name]
        Hook(REL_ID(21121, 21573), REL_OF(0x0CE, 0x0D4, 0)); // (000) [ ] (GetWantBlocking)
        Hook(REL_ID(20955, 21405), REL_OF(0x077, 0x077, 0)); // (001) [ ] (GetDistance)
        Hook(REL_ID(21083, 21533), REL_OF(0x08C, 0x08C, 0)); // (005) [ ] (GetLocked)
        Hook(REL_ID(20957, 21407), REL_OF(0x076, 0x076, 0)); // (006) [ ] (GetPos)
        Hook(REL_ID(20958, 21408), REL_OF(0x092, 0x092, 0)); // (008) [ ] (GetAngle)
        Hook(REL_ID(20959, 21409), REL_OF(0x089, 0x089, 0)); // (010) [ ] (GetStartingPos)
        Hook(REL_ID(20963, 21413), REL_OF(0x065, 0x062, 0)); // (012) [ ] (GetSecondsPassed)
        Hook(REL_ID(20966, 21416), REL_OF(0x089, 0x089, 0)); // (014) [ ] (GetActorValue)
        Hook(REL_ID(20970, 21420), REL_OF(0x051, 0x051, 0)); // (018) [ ] (GetCurrentTime)
        Hook(REL_ID(20971, 21421), REL_OF(0x07C, 0x07C, 0)); // (024) [ ] (GetScale)
        Hook(REL_ID(21996, 22479), REL_OF(0x050, 0x050, 0)); // (025) [ ] (IsMoving)
        Hook(REL_ID(21997, 22480), REL_OF(0x050, 0x050, 0)); // (026) [ ] (IsTurning)
        Hook(REL_ID(20972, 21422), REL_OF(0x0FF, 0x107, 0)); // (027) [ ] (GetLineOfSight)
        Hook(REL_ID(21026, 21476), REL_OF(0x089, 0x08D, 0)); // (032) [ ] (GetInSameCell)
        Hook(REL_ID(20973, 21423), REL_OF(0x09A, 0x09A, 0)); // (035) [ ] (GetDisabled)
        Hook(REL_ID(20974, 21424), REL_OF(0x076, 0x076, 0)); // (039) [ ] (GetDisease)
        Hook(REL_ID(20976, 21426), REL_OF(0x05D, 0x05D, 0)); // (041) [ ] (GetClothingValue)
        Hook(REL_ID(20977, 21427), REL_OF(0x13C, 0x144, 0)); // (042) [ ] (SameFaction)
        Hook(REL_ID(20978, 21428), REL_OF(0x08C, 0x092, 0)); // (043) [ ] (SameRace)
        Hook(REL_ID(20979, 21429), REL_OF(0x0A8, 0x0A8, 0)); // (044) [ ] (SameSex)
        Hook(REL_ID(20983, 21433), REL_OF(0x0C5, 0x0C5, 0)); // (045) [ ] (GetDetected)
        Hook(REL_ID(20984, 21434), REL_OF(0x074, 0x074, 0)); // (046) [ ] (GetDead)
        Hook(REL_ID(20985, 21435), REL_OF(0x05C, 0x18F, 0)); // (047) [ ] (GetItemCount)
        Hook(REL_ID(20989, 21439), REL_OF(0x099, 0x099, 0)); // (048) [ ] (GetGold)
        Hook(REL_ID(20990, 21440), REL_OF(0x0A3, 0x0CA, 0)); // (049) [ ] (GetSleeping)
        Hook(REL_ID(20994, 21444), REL_OF(0x06D, 0x073, 0)); // (050) [ ] (GetTalkedToPC)
        Hook(REL_ID(20997, 21447), REL_OF(0x074, 0x07A, 0)); // (056) [ ] (GetQuestRunning)
        Hook(REL_ID(20999, 21449), REL_OF(0x063, 0x062, 0)); // (058) [ ] (GetStage)
        Hook(REL_ID(21000, 21450), REL_OF(0x06A, 0x06A, 0)); // (059) [ ] (GetStageDone)
        Hook(REL_ID(21001, 21451), REL_OF(0x0F8, 0x0F8, 0)); // (060) [ ] (GetFactionRankDifference)
        Hook(REL_ID(21002, 21452), REL_OF(0x081, 0x081, 0)); // (061) [ ] (GetAlarmed)
        Hook(REL_ID(21005, 21455), REL_OF(0x05F, 0x0BD, 0)); // (062) [ ] (IsRaining)
        Hook(REL_ID(21019, 21469), REL_OF(0x06D, 0x06D, 0)); // (063) [ ] (GetAttacked)
        Hook(REL_ID(21020, 21470), REL_OF(0x03F, 0x03F, 0)); // (064) [ ] (GetIsCreature)
        Hook(REL_ID(21084, 21534), REL_OF(0x074, 0x074, 0)); // (065) [ ] (GetLockLevel)
        Hook(REL_ID(21021, 21471), REL_OF(0x0C7, 0x0C7, 0)); // (066) [ ] (GetShouldAttack)
        Hook(REL_ID(21024, 21474), REL_OF(0x07F, 0x083, 0)); // (067) [ ] (GetInCell[Param]) ?
        Hook(REL_ID(21027, 21477), REL_OF(0x081, 0x087, 0)); // (068) [ ] (GetIsClass)
        Hook(REL_ID(21028, 21478), REL_OF(0x081, 0x087, 0)); // (069) [ ] (GetIsRace)
        Hook(REL_ID(21030, 21480), REL_OF(0x076, 0x076, 0)); // (070) [ ] (GetIsSex)
        Hook(REL_ID(21032, 21482), REL_OF(0x0D3, 0x0D9, 0)); // (071) [ ] (GetInFaction)
        Hook(REL_ID(21037, 21487), REL_OF(0x0BD, 0x0BD, 0)); // (072) [ ] (GetIsID)
        Hook(REL_ID(21043, 21493), REL_OF(0x089, 0x09A, 0)); // (073) [ ] (GetFactionRank)
        Hook(REL_ID(21044, 21494), REL_OF(0x05F, 0x06B, 0)); // (074) [ ] (GetGlobalValue)
        Hook(REL_ID(21006, 21456), REL_OF(0x05F, 0x0BD, 0)); // (075) [ ] (IsSnowing)
        Hook(REL_ID(21045, 21495), REL_OF(0x05A, 0x077, 0)); // (077) [ ] (GetRandomPercent)
        Hook(REL_ID(21046, 21496), REL_OF(0x06C, 0x06B, 0)); // (080) [ ] (GetLevel)
        Hook(REL_ID(22051, 22534), REL_OF(0x123, 0x126, 0)); // (083) [ ] (GetLeveledEncounterValue)
        Hook(REL_ID(21047, 21497), REL_OF(0x061, 0x061, 0)); // (084) [ ] (GetDeadCount)
        Hook(REL_ID(21048, 21498), REL_OF(0x068, 0x068, 0)); // (091) [ ] (GetIsAlerted)
        Hook(REL_ID(21049, 21499), REL_OF(0x102, 0x108, 0)); // (098) [A] (GetPlayerControlsDisabled)
        Hook(REL_ID(21049, 21499), REL_OF(0x14D, 0x15D, 0)); // (098) [B]
        Hook(REL_ID(21049, 21499), REL_OF(0x1A4, 0x1B4, 0)); // (098) [C]
        Hook(REL_ID(21049, 21499), REL_OF(0x1EB, 0x1FB, 0)); // (098) [D]
        Hook(REL_ID(21049, 21499), REL_OF(0x20C, 0x21C, 0)); // (098) [E]
        Hook(REL_ID(21050, 21500), REL_OF(0x138, 0x139, 0)); // (099) [ ] (GetHeadingAngle)
        Hook(REL_ID(21051, 21501), REL_OF(0x074, 0x07E, 0)); // (101) [ ] (IsWeaponMagicOut)
        Hook(REL_ID(21053, 21503), REL_OF(0x0E0, 0x0E2, 0)); // (102) [ ] (IsTorchOut)
        Hook(REL_ID(21054, 21504), REL_OF(0x0D4, 0x0D6, 0)); // (103) [ ] (IsShieldOut)
        Hook(REL_ID(21055, 21505), REL_OF(0x0BD, 0x0BD, 0)); // (106) [ ] (IsFacingUp)
        Hook(REL_ID(21057, 21507), REL_OF(0x073, 0x096, 0)); // (107) [ ] (GetKnockedState)
        Hook(REL_ID(21060, 21510), REL_OF(0x0B1, 0x0B3, 0)); // (108) [ ] (GetWeaponAnimType)
        Hook(REL_ID(21061, 21511), REL_OF(0x069, 0x075, 0)); // (109) [ ] (IsWeaponSkillType)
        Hook(REL_ID(21062, 21512), REL_OF(0x252, 0x252, 0)); // (110) [ ] (GetCurrentAIPackage)
        Hook(REL_ID(21063, 21513), REL_OF(0x06E, 0x06E, 0)); // (111) [ ] (IsWaiting)
        Hook(REL_ID(21064, 21514), REL_OF(0x03F, 0x03F, 0)); // (112) [ ] (IsIdlePlaying)
        Hook(REL_ID(21320, 21779), REL_OF(0x06E, 0x06E, 0)); // (116) [ ] (IsIntimidatedByPlayer)
        Hook(REL_ID(21322, 21781), REL_OF(0x0B0, 0x0B0, 0)); // (117) [ ] (IsPlayerInRegion)
        Hook(REL_ID(21171, 21626), REL_OF(0x0BA, 0x0BA, 0)); // (118) [ ] (GetActorAggroRadiusViolated)
        Hook(REL_ID(21065, 21515), REL_OF(0x0B6, 0x0B4, 0)); // (122) [ ] (GetCrime)
        Hook(REL_ID(21174, 21629), REL_OF(0x106, 0x108, 0)); // (123) [ ] (IsGreetingPlayer)
        Hook(REL_ID(21066, 21516), REL_OF(0x06B, 0x071, 0)); // (125) [ ] (IsGuard)
        Hook(REL_ID(20944, 21394), REL_OF(0x069, 0x069, 0)); // (127) [ ] (HasBeenEaten)
        Hook(REL_ID(21067, 21517), REL_OF(0x061, 0x061, 0)); // (128) [ ] (GetStaminaPercentage)
        Hook(REL_ID(21027, 21483), REL_OF(0x081, 0x08D, 0)); // (129) [ ] (GetPCIsClass)
        Hook(REL_ID(21028, 21484), REL_OF(0x081, 0x08D, 0)); // (130) [ ] (GetPCIsRace)
        Hook(REL_ID(21030, 21485), REL_OF(0x076, 0x07D, 0)); // (131) [ ] (GetPCIsSex)
        Hook(REL_ID(21032, 21486), REL_OF(0x0D3, 0x0DD, 0)); // (132) [ ] (GetPCInFaction)
        Hook(REL_ID(20977, 21430), REL_OF(0x13C, 0x14F, 0)); // (133) [ ] (SameFactionAsPC)
        Hook(REL_ID(20978, 21431), REL_OF(0x08C, 0x098, 0)); // (134) [ ] (SameRaceAsPC)
        Hook(REL_ID(20979, 21432), REL_OF(0x0A8, 0x0AF, 0)); // (135) [ ] (SameSexAsPC)
        Hook(REL_ID(21042, 21492), REL_OF(0x06F, 0x073, 0)); // (136) [ ] (GetIsRef[erence])
        Hook(REL_ID(21069, 21519), REL_OF(0x06E, 0x06E, 0)); // (141) [ ] (IsTalking)
        Hook(REL_ID(21070, 21520), REL_OF(0x061, 0x061, 0)); // (142) [ ] (GetWalkSpeed)
        Hook(REL_ID(21071, 21521), REL_OF(0x3DC, 0x3DB, 0)); // (143) [ ] (GetCurrentAIProcedure)
        Hook(REL_ID(21072, 21522), REL_OF(0x0B1, 0x0B1, 0)); // (144) [ ] (GetTrespassWarningLevel)
        Hook(REL_ID(21073, 21523), REL_OF(0x06B, 0x071, 0)); // (145) [ ] (IsTrespassing)
        Hook(REL_ID(21074, 21524), REL_OF(0x074, 0x074, 0)); // (146) [ ] (IsInMyOwnedCell)
        Hook(REL_ID(21007, 21457), REL_OF(0x064, 0x064, 0)); // (147) [ ] (GetWindSpeed)
        Hook(REL_ID(21008, 21458), REL_OF(0x04E, 0x04E, 0)); // (148) [ ] (GetCurrentWeatherPercent)
        Hook(REL_ID(21009, 21459), REL_OF(0x08C, 0x08C, 0)); // (149) [ ] (GetIsCurrentWeather)
        Hook(REL_ID(21075, 21525), REL_OF(0x06C, 0x06B, 0)); // (150) [ ] (IsContinuingPackagePCNear)
        Hook(REL_ID(21262, 21721), REL_OF(0x076, 0x076, 0)); // (152) [ ] (GetIsCrimeFaction)
        Hook(REL_ID(21076, 21526), REL_OF(0x0C9, 0x0C6, 0)); // (153) [ ] (CanHaveFlames)
        Hook(REL_ID(21077, 21527), REL_OF(0x099, 0x096, 0)); // (154) [ ] (HasFlames)
        Hook(REL_ID(21078, 21528), REL_OF(0x05D, 0x05C, 0)); // (157) [ ] (GetOpenState)
        Hook(REL_ID(20991, 21441), REL_OF(0x0A0, 0x0C4, 0)); // (159) [ ] (GetSitting)
        Hook(REL_ID(21079, 21529), REL_OF(0x0F0, 0x0F0, 0)); // (161) [ ] (GetIsCurrentPackage)
        Hook(REL_ID(20992, 21442), REL_OF(0x0EC, 0x0EE, 0)); // (162) [ ] (IsCurrentFurnitureRef)
        Hook(REL_ID(20993, 21443), REL_OF(0x101, 0x103, 0)); // (163) [ ] (IsCurrentFurnitureObj)
        Hook(REL_ID(21080, 21530), REL_OF(0x059, 0x059, 0)); // (170) [ ] (GetDayOfWeek)
        Hook(REL_ID(20994, 21444), REL_OF(0x06D, 0x073, 0)); // (172) [ ] (GetTalkedToPCParam)
        Hook(REL_ID(21081, 21531), REL_OF(0x061, 0x05E, 0)); // (175) [ ] (IsPCSleeping)
        Hook(REL_ID(21082, 21532), REL_OF(0x065, 0x062, 0)); // (176) [ ] (IsPCAMurderer)
        Hook(REL_ID(21200, 21657), REL_OF(0x05B, 0x05B, 0)); // (180) [ ] (HasSameEditorLocAsRef)
        Hook(REL_ID(21201, 21658), REL_OF(0x147, 0x149, 0)); // (181) [ ] (HasSameEditorLocAsRefAlias)
        Hook(REL_ID(21086, 21536), REL_OF(0x0E2, 0x17F, 0)); // (182) [ ] (GetEquipped)
        Hook(REL_ID(21087, 21537), REL_OF(0x07A, 0x07A, 0)); // (185) [ ] (IsSwimming)
        Hook(REL_ID(21088, 21538), REL_OF(0x054, 0x054, 0)); // (190) [ ] (GetAmountSoldStolen)
        Hook(REL_ID(21090, 21540), REL_OF(0x070, 0x076, 0)); // (193) [ ] (GetPCExpelled)
        Hook(REL_ID(21091, 21541), REL_OF(0x070, 0x076, 0)); // (195) [ ] (GetPCFactionMurder)
        Hook(REL_ID(21092, 21542), REL_OF(0x076, 0x076, 0)); // (197) [ ] (GetPCEnemyofFaction)
        Hook(REL_ID(21093, 21543), REL_OF(0x070, 0x076, 0)); // (199) [ ] (GetPCFactionAttack)
        Hook(REL_ID(21094, 21544), REL_OF(0x05D, 0x063, 0)); // (203) [ ] (GetDestroyed)
        Hook(REL_ID(21095, 21545), REL_OF(0x089, 0x089, 0)); // (214) [ ] (HasMagicEffect)
        Hook(REL_ID(21097, 21547), REL_OF(0x089, 0x089, 0)); // (223) [ ] (IsSpellTarget)
        Hook(REL_ID(21099, 21549), REL_OF(0x051, 0x051, 0)); // (224) [ ] (GetVATSMode)
        Hook(REL_ID(20975, 21425), REL_OF(0x06C, 0x06B, 0)); // (226) [ ] (GetVampireFeed)
        Hook(REL_ID(21107, 21559), REL_OF(0x06C, 0x06B, 0)); // (227) [ ] (GetCannibal)
        Hook(REL_ID(21108, 21560), REL_OF(0x07A, 0x07A, 0)); // (228) [ ] (GetIsClassDefault)
        Hook(REL_ID(21024, 21474), REL_OF(0x07F, 0x083, 0)); // (230) [ ] (GetInCellParam)
        Hook(REL_ID(21100, 21550), REL_OF(0x13B, 0x128, 0)); // (235) [ ] (GetVATSTargetHeight)
        Hook(REL_ID(21110, 21562), REL_OF(0x06E, 0x06E, 0)); // (237) [ ] (GetIsGhost)
        Hook(REL_ID(21111, 21563), REL_OF(0x0A7, 0x0AD, 0)); // (242) [ ] (GetUnconscious)
        Hook(REL_ID(21112, 21564), REL_OF(0x0A7, 0x0AD, 0)); // (244) [ ] (GetRestrained)
        Hook(REL_ID(21040, 21490), REL_OF(0x0AD, 0x0AD, 0)); // (246) [ ] (GetIsUsedItem)
        Hook(REL_ID(21041, 21491), REL_OF(0x07D, 0x07E, 0)); // (247) [ ] (GetIsUsedItemType)
        Hook(REL_ID(21223, 21680), REL_OF(0x0A7, 0x0AD, 0)); // (248) [ ] (IsScenePlaying)
        Hook(REL_ID(21235, 21692), REL_OF(0x16D, 0x167, 0)); // (249) [ ] (IsInDialogueWithPlayer)
        Hook(REL_ID(21231, 21688), REL_OF(0x05C, 0x062, 0)); // (250) [ ] (GetLocationCleared)
        Hook(REL_ID(21031, 21481), REL_OF(0x0B8, 0x0B4, 0)); // (254) [ ] (GetIsPlayableRace)
        Hook(REL_ID(21113, 21565), REL_OF(0x09A, 0x09A, 0)); // (255) [ ] (GetOffersServicesNow)
        Hook(REL_ID(21242, 21699), REL_OF(0x0E9, 0x0E9, 0)); // (258) [ ] (HasAssociationType)
        Hook(REL_ID(21243, 21700), REL_OF(0x0E3, 0x0E3, 0)); // (259) [ ] (HasFamilyRelationship)
        Hook(REL_ID(21244, 21701), REL_OF(0x104, 0x104, 0)); // (261) [ ] (HasParentRelationship)
        Hook(REL_ID(21305, 21764), REL_OF(0x0D1, 0x0CC, 0)); // (263) [ ] (IsWeaponOut)
        Hook(REL_ID(21306, 21765), REL_OF(0x0C1, 0x0BE, 0)); // (264) [ ] (HasSpell)
        Hook(REL_ID(21114, 21566), REL_OF(0x06E, 0x06E, 0)); // (265) [ ] (IsTimePassing)
        Hook(REL_ID(21003, 21453), REL_OF(0x099, 0x099, 0)); // (266) [ ] (IsPleasant)
        Hook(REL_ID(21004, 21454), REL_OF(0x099, 0x099, 0)); // (267) [ ] (IsCloudy)
        Hook(REL_ID(20969, 21419), REL_OF(0x089, 0x089, 0)); // (277) [ ] (GetBaseActorValue)
        Hook(REL_ID(21115, 21567), REL_OF(0x0A2, 0x0A2, 0)); // (278) [ ] (IsOwner)
        Hook(REL_ID(21116, 21568), REL_OF(0x087, 0x087, 0)); // (280) [ ] (IsCellOwner)
        Hook(REL_ID(21056, 21506), REL_OF(0x0BD, 0x0BD, 0)); // (285) [ ] (IsLeftUp)
        Hook(REL_ID(21118, 21570), REL_OF(0x0C4, 0x0C4, 0)); // (286) [ ] (IsSneaking)
        Hook(REL_ID(21119, 21571), REL_OF(0x095, 0x095, 0)); // (287) [ ] (IsRunning)
        Hook(REL_ID(21123, 21575), REL_OF(0x0A0, 0x0A0, 0)); // (288) [ ] (GetFriendHit)
        Hook(REL_ID(21124, 21576), REL_OF(0x0B7, 0x0B7, 0)); // (289) [ ] (IsInCombat)
        Hook(REL_ID(21125, 21577), REL_OF(0x094, 0x09A, 0)); // (300) [ ] (IsInInterior)
        Hook(REL_ID(20954, 21404), REL_OF(0x069, 0x069, 0)); // (304) [ ] (IsWaterObject)
        Hook(REL_ID(21260, 21717), REL_OF(0x062, 0x062, 0)); // (305) [ ] (GetPlayerAction)
        Hook(REL_ID(21136, 21589), REL_OF(0x03F, 0x03F, 0)); // (309) [ ] (IsXBox)
        Hook(REL_ID(21025, 21475), REL_OF(0x0B0, 0x0AE, 0)); // (310) [ ] (GetInWorldspace)
        Hook(REL_ID(21126, 21579), REL_OF(0x0A2, 0x0A2, 0)); // (312) [A] (GetPCMiscStat)
        Hook(REL_ID(21126, 21579), REL_OF(0x0D8, 0x0D8, 0)); // (312) [B]
        Hook(REL_ID(21127, 21580), REL_OF(0x069, 0x069, 0)); // (313) [ ] (GetPairedAnimation)
        Hook(REL_ID(21128, 21581), REL_OF(0x0C7, 0x0CD, 0)); // (314) [ ] (IsActorAVictim)
        Hook(REL_ID(21252, 21709), REL_OF(0x0DA, 0x0DA, 0)); // (325) [ ] (GetWithinPackageLocation)
        Hook(REL_ID(21300, 21759), REL_OF(0x06E, 0x06E, 0)); // (329) [ ] (IsFleeing)
        Hook(REL_ID(21195, 21652), REL_OF(0x071, 0x071, 0)); // (359) [ ] (GetInCurrentLocation)
        Hook(REL_ID(21197, 21654), REL_OF(0x08E, 0x08E, 0)); // (360) [ ] (GetInCurrentLocAlias[ConditionFunction])
        Hook(REL_ID(21288, 21747), REL_OF(0x065, 0x065, 0)); // (362) [ ] (HasLinkedRef)
        Hook(REL_ID(21039, 21489), REL_OF(0x064, 0x064, 0)); // (365) [ ] (IsChild)
        Hook(REL_ID(21301, 21760), REL_OF(0x064, 0x063, 0)); // (366) [ ] (GetStolenItemValueNoCrime)
        Hook(REL_ID(21144, 21597), REL_OF(0x05C, 0x05C, 0)); // (368) [ ] (IsPlayerActionActive)
        Hook(REL_ID(21089, 21539), REL_OF(0x091, 0x091, 0)); // (372) [ ] (IsInList)
        Hook(REL_ID(21302, 21761), REL_OF(0x064, 0x063, 0)); // (373) [ ] (GetStolenItemValue)
        Hook(REL_ID(21248, 21705), REL_OF(0x0A6, 0x099, 0)); // (375) [ ] (GetCrimeGoldViolent)
        Hook(REL_ID(21249, 21706), REL_OF(0x0A6, 0x099, 0)); // (376) [ ] (GetCrimeGoldNonViolent)
        Hook(REL_ID(21307, 21766), REL_OF(0x0C1, 0x0BE, 0)); // (378) [ ] (HasShout)
        Hook(REL_ID(21149, 21602), REL_OF(0x070, 0x074, 0)); // (396) [ ] (GetCauseOfDeath)
        Hook(REL_ID(21052, 21502), REL_OF(0x0A0, 0x0A0, 0)); // (398) [ ] (IsWeaponInList)
        Hook(REL_ID(21320, 21779), REL_OF(0x06E, 0x06E, 0)); // (402) [ ] (IsBribedbyPlayer)
        Hook(REL_ID(21222, 21679), REL_OF(0x084, 0x087, 0)); // (403) [ ] (GetRelationshipRank)
        Hook(REL_ID(21152, 21605), REL_OF(0x0A0, 0x0A0, 0)); // (408) [ ] (IsKiller)
        Hook(REL_ID(21153, 21606), REL_OF(0x0B5, 0x0B5, 0)); // (409) [ ] (IsKillerObject)
        Hook(REL_ID(21038, 21488), REL_OF(0x0BC, 0x0BC, 0)); // (426) [ ] (GetIsVoiceType)
        Hook(REL_ID(21309, 21768), REL_OF(0x09E, 0x09B, 0)); // (429) [ ] (IsScenePackageRunning)
        Hook(REL_ID(21068, 21518), REL_OF(0x061, 0x061, 0)); // (430) [ ] (GetHealthPercentage)
        Hook(REL_ID(21162, 21617), REL_OF(0x07D, 0x07E, 0)); // (432) [ ] (GetIsObjectType)
        Hook(REL_ID(21163, 21618), REL_OF(0x08B, 0x092, 0)); // (434) [ ] (GetDialogueEmotion)
        Hook(REL_ID(21164, 21619), REL_OF(0x06D, 0x06C, 0)); // (435) [ ] (GetDialogueEmotionValue)
        Hook(REL_ID(21196, 21653), REL_OF(0x107, 0x102, 0)); // (444) [ ] (GetInCurrentLocFormList)
        Hook(REL_ID(20952, 21402), REL_OF(0x09D, 0x09D, 0)); // (445) [ ] (GetInZone)
        Hook(REL_ID(20953, 21403), REL_OF(0x0E3, 0x0CE, 0)); // (446) [ ] (GetVelocity)
        Hook(REL_ID(21167, 21622), REL_OF(0x05F, 0x05F, 0)); // (448) [ ] (HasPerk)
        Hook(REL_ID(21165, 21620), REL_OF(0x07F, 0x082, 0)); // (449) [ ] (GetFactionRelation)
        Hook(REL_ID(21166, 21621), REL_OF(0x07D, 0x07D, 0)); // (450) [ ] (IsLastIdlePlayed)
        Hook(REL_ID(21168, 21623), REL_OF(0x066, 0x071, 0)); // (453) [ ] (GetPlayerTeammate)
        Hook(REL_ID(21169, 21624), REL_OF(0x056, 0x056, 0)); // (454) [ ] (GetPlayerTeammateCount)
        Hook(REL_ID(21170, 21625), REL_OF(0x0AD, 0x0AD, 0)); // (458) [ ] (GetActorCrimePlayerEnemy)
        Hook(REL_ID(21246, 21703), REL_OF(0x0A6, 0x099, 0)); // (459) [ ] (GetCrimeGold)
        Hook(REL_ID(20986, 21436), REL_OF(0x059, 0x059, 0)); // (465) [ ] (GetKeywordItemCount)
        Hook(REL_ID(20950, 21400), REL_OF(0x061, 0x065, 0)); // (470) [ ] (GetDestructionStage)
        Hook(REL_ID(20948, 21398), REL_OF(0x0D3, 0x0D3, 0)); // (477) [A] (GetThreatRatio)
        Hook(REL_ID(20948, 21398), REL_OF(0x11B, 0x11B, 0)); // (477) [B]
        Hook(REL_ID(20947, 21397), REL_OF(0x09A, 0x09A, 0)); // (479) [ ] (GetIsUsedItemEquipType)
        Hook(REL_ID(21172, 21627), REL_OF(0x08B, 0x08B, 0)); // (487) [ ] (IsCarryable)
        Hook(REL_ID(20946, 21396), REL_OF(0x093, 0x093, 0)); // (491) [ ] (GetMapMarkerVisible)
        Hook(REL_ID(21308, 21767), REL_OF(0x09F, 0x09F, 0)); // (493) [ ] (PlayerKnows)
        Hook(REL_ID(20968, 21418), REL_OF(0x0A4, 0x0A3, 0)); // (494) [ ] (GetPermanentActorValue)
        Hook(REL_ID(21151, 21604), REL_OF(0x070, 0x074, 0)); // (495) [ ] (GetKillingBlowLimb)
        Hook(REL_ID(21247, 21704), REL_OF(0x0BD, 0x0B2, 0)); // (497) [ ] (CanPayCrimeGold)
        Hook(REL_ID(21263, 21722), REL_OF(0x06E, 0x06C, 0)); // (499) [ ] (GetDaysInJail)
        Hook(REL_ID(21316, 21775), REL_OF(0x084, 0x085, 0)); // (503) [ ] (GetAllowWorldInteractions)
        Hook(REL_ID(21176, 21631), REL_OF(0x06E, 0x06E, 0)); // (513) [ ] (IsCombatTarget)
        Hook(REL_ID(21177, 21632), REL_OF(0x06D, 0x06C, 0)); // (515) [ ] (GetVATSRightAreaFree)
        Hook(REL_ID(21178, 21633), REL_OF(0x06D, 0x06C, 0)); // (516) [ ] (GetVATSLeftAreaFree)
        Hook(REL_ID(21179, 21634), REL_OF(0x06D, 0x06C, 0)); // (517) [ ] (GetVATSBackAreaFree)
        Hook(REL_ID(21180, 21635), REL_OF(0x068, 0x064, 0)); // (518) [ ] (GetVATSFrontAreaFree)
        Hook(REL_ID(21085, 21535), REL_OF(0x06B, 0x06B, 0)); // (519) [ ] (GetIsLockBroken)
        Hook(REL_ID(21137, 21590), REL_OF(0x03F, 0x03F, 0)); // (520) [ ] (IsPS3)
        Hook(REL_ID(21138, 21591), REL_OF(0x04A, 0x04A, 0)); // (521) [ ] (IsWin32)
        Hook(REL_ID(21181, 21636), REL_OF(0x06D, 0x06C, 0)); // (522) [ ] (GetVATSRightTargetVisible)
        Hook(REL_ID(21182, 21637), REL_OF(0x06D, 0x06C, 0)); // (523) [ ] (GetVATSLeftTargetVisible)
        Hook(REL_ID(21183, 21638), REL_OF(0x06D, 0x06C, 0)); // (524) [ ] (GetVATSBackTargetVisible)
        Hook(REL_ID(21184, 21639), REL_OF(0x068, 0x064, 0)); // (525) [ ] (GetVATSFrontTargetVisible)
        Hook(REL_ID(20943, 21393), REL_OF(0x084, 0x089, 0)); // (528) [ ] (IsInCriticalStage)
        Hook(REL_ID(21271, 21730), REL_OF(0x087, 0x087, 0)); // (533) [ ] (GetInfamy)
        Hook(REL_ID(21272, 21731), REL_OF(0x087, 0x087, 0)); // (534) [ ] (GetInfamyViolent)
        Hook(REL_ID(21273, 21732), REL_OF(0x087, 0x087, 0)); // (535) [ ] (GetInfamyNonViolent)
        Hook(REL_ID(20998, 21448), REL_OF(0x060, 0x066, 0)); // (543) [ ] (GetQuestCompleted)
        Hook(REL_ID(21185, 21642), REL_OF(0x057, 0x05D, 0)); // (547) [ ] (IsGoreDisabled)
        Hook(REL_ID(21224, 21681), REL_OF(0x0B1, 0x0B1, 0)); // (550) [ ] (IsSceneActionComplete)
        Hook(REL_ID(21098, 21548), REL_OF(0x074, 0x073, 0)); // (552) [ ] (GetSpellUsageNum)
        Hook(REL_ID(21186, 21643), REL_OF(0x053, 0x053, 0)); // (554) [ ] (GetActorsInHigh)
        Hook(REL_ID(20965, 21415), REL_OF(0x065, 0x065, 0)); // (555) [ ] (HasLoaded3D)
        Hook(REL_ID(21187, 21644), REL_OF(0x069, 0x069, 0)); // (560) [ ] (HasKeyword)
        Hook(REL_ID(21191, 21648), REL_OF(0x06D, 0x06D, 0)); // (561) [ ] (HasRefType)
        Hook(REL_ID(21192, 21649), REL_OF(0x07E, 0x07E, 0)); // (562) [ ] (LocationHasKeyword)
        Hook(REL_ID(21193, 21650), REL_OF(0x07F, 0x07F, 0)); // (563) [ ] (LocationHasRefType)
        Hook(REL_ID(21194, 21651), REL_OF(0x071, 0x071, 0)); // (565) [ ] (GetIsEditorLoc[ation])
        Hook(REL_ID(21198, 21655), REL_OF(0x094, 0x094, 0)); // (566) [ ] (GetIsAlias[Ref])
        Hook(REL_ID(21199, 21656), REL_OF(0x08E, 0x08E, 0)); // (567) [ ] (GetIsEditorLocAlias)
        Hook(REL_ID(21120, 21572), REL_OF(0x09E, 0x09E, 0)); // (568) [ ] (IsSprinting)
        Hook(REL_ID(21122, 21574), REL_OF(0x0C6, 0x0C6, 0)); // (569) [ ] (IsBlocking)
        Hook(REL_ID(21102, 21554), REL_OF(0x073, 0x073, 0)); // (570) [ ] (HasEquippedSpell)
        Hook(REL_ID(21103, 21555), REL_OF(0x0A3, 0x0A3, 0)); // (571) [ ] (GetCurrentCastingType)
        Hook(REL_ID(21104, 21556), REL_OF(0x07E, 0x07E, 0)); // (572) [ ] (GetCurrentDeliveryType)
        Hook(REL_ID(21059, 21509), REL_OF(0x067, 0x065, 0)); // (574) [ ] (GetAttackState)
        Hook(REL_ID(21310, 21769), REL_OF(0x0AF, 0x0B6, 0)); // (577) [ ] (IsCloserToAThanB)
        Hook(REL_ID(21218, 21675), REL_OF(0x0AF, 0x0AF, 0)); // (579) [ ] (GetEquippedShout)
        Hook(REL_ID(21010, 21460), REL_OF(0x0BD, 0x0C2, 0)); // (580) [ ] (IsBleedingOut)
        Hook(REL_ID(20960, 21410), REL_OF(0x0A8, 0x0A8, 0)); // (584) [ ] (GetRelativeAngle)
        Hook(REL_ID(21236, 21693), REL_OF(0x10A, 0x0FE, 0)); // (589) [ ] (GetMovementDirection)
        Hook(REL_ID(21225, 21682), REL_OF(0x0A2, 0x0A2, 0)); // (590) [ ] (IsInScene)
        Hook(REL_ID(21226, 21683), REL_OF(0x07F, 0x07F, 0)); // (591) [ ] (GetRefTypeDeadCount)
        Hook(REL_ID(21227, 21684), REL_OF(0x07F, 0x07F, 0)); // (592) [ ] (GetRefTypeAliveCount)
        Hook(REL_ID(21237, 21694), REL_OF(0x0A8, 0x0AA, 0)); // (594) [ ] (GetIsFlying)
        Hook(REL_ID(21105, 21557), REL_OF(0x071, 0x07B, 0)); // (595) [ ] (IsCurrentSpell)
        Hook(REL_ID(21106, 21558), REL_OF(0x08A, 0x08A, 0)); // (596) [ ] (SpellHasKeyword)
        Hook(REL_ID(21220, 21677), REL_OF(0x0B8, 0x0B4, 0)); // (597) [ ] (GetEquippedItemType)
        Hook(REL_ID(21232, 21689), REL_OF(0x077, 0x077, 0)); // (598) [ ] (GetLocationAliasCleared)
        Hook(REL_ID(21233, 21690), REL_OF(0x0A2, 0x0A2, 0)); // (600) [ ] (GetLocAliasRefTypeDeadCount)
        Hook(REL_ID(21234, 21691), REL_OF(0x0A2, 0x0A2, 0)); // (601) [ ] (GetLocAliasRefTypeAliveCount)
        Hook(REL_ID(21245, 21702), REL_OF(0x07F, 0x07F, 0)); // (602) [ ] (IsWardState)
        Hook(REL_ID(21202, 21659), REL_OF(0x05B, 0x05B, 0)); // (603) [ ] (IsInSameCurrentLocAsRef)
        Hook(REL_ID(21203, 21660), REL_OF(0x147, 0x149, 0)); // (604) [ ] (IsInSameCurrentLocAsRefAlias)
        Hook(REL_ID(21204, 21661), REL_OF(0x078, 0x078, 0)); // (605) [ ] (LocAliasIsLocation)
        Hook(REL_ID(21207, 21664), REL_OF(0x063, 0x062, 0)); // (606) [ ] (GetKeywordDataForLocation)
        Hook(REL_ID(21208, 21665), REL_OF(0x07F, 0x07F, 0)); // (608) [ ] (GetKeywordDataForAlias)
        Hook(REL_ID(21209, 21666), REL_OF(0x08C, 0x08C, 0)); // (610) [ ] (LocAliasHasKeyword)
        Hook(REL_ID(21250, 21707), REL_OF(0x0C3, 0x0C3, 0)); // (611) [ ] (IsNullPackageData)
        Hook(REL_ID(21251, 21708), REL_OF(0x0D1, 0x0D1, 0)); // (612) [ ] (GetNumericPackageData)
        Hook(REL_ID(21211, 21668), REL_OF(0x075, 0x075, 0)); // (613) [ ] (IsFurnitureAnimType)
        Hook(REL_ID(21212, 21669), REL_OF(0x075, 0x075, 0)); // (614) [ ] (IsFurnitureEntryType)
        Hook(REL_ID(21213, 21670), REL_OF(0x087, 0x087, 0)); // (615) [ ] (GetHighestRelationshipRank)
        Hook(REL_ID(21214, 21671), REL_OF(0x087, 0x087, 0)); // (616) [ ] (GetLowestRelationshipRank)
        Hook(REL_ID(21216, 21673), REL_OF(0x077, 0x077, 0)); // (617) [ ] (HasAssociationTypeAny)
        Hook(REL_ID(21215, 21672), REL_OF(0x072, 0x072, 0)); // (618) [ ] (HasFamilyRelationshipAny)
        Hook(REL_ID(21254, 21711), REL_OF(0x06F, 0x06F, 0)); // (619) [A] (GetPathingTargetOffset)
        Hook(REL_ID(21254, 21711), REL_OF(0x0CE, 0x0C6, 0)); // (619) [B]
        Hook(REL_ID(21254, 21711), REL_OF(0x0F3, 0x0EB, 0)); // (619) [C]
        Hook(REL_ID(21254, 21711), REL_OF(0x118, 0x110, 0)); // (619) [D]
        Hook(REL_ID(21254, 21711), REL_OF(0x138, 0x130, 0)); // (619) [E]
        Hook(REL_ID(21255, 21712), REL_OF(0x074, 0x074, 0)); // (620) [A] (GetPathingTargetAngleOffset)
        Hook(REL_ID(21255, 21712), REL_OF(0x111, 0x111, 0)); // (620) [B]
        Hook(REL_ID(21255, 21712), REL_OF(0x136, 0x136, 0)); // (620) [C]
        Hook(REL_ID(21255, 21712), REL_OF(0x15B, 0x15B, 0)); // (620) [D]
        Hook(REL_ID(21255, 21712), REL_OF(0x17B, 0x17B, 0)); // (620) [E]
        Hook(REL_ID(21256, 21713), REL_OF(0x0A6, 0x0A6, 0)); // (621) [A] (GetPathingTargetSpeed)
        Hook(REL_ID(21256, 21713), REL_OF(0x0C6, 0x0C6, 0)); // (621) [B]
        Hook(REL_ID(21256, 21713), REL_OF(0x0E6, 0x0E6, 0)); // (621) [C]
        Hook(REL_ID(21256, 21713), REL_OF(0x101, 0x101, 0)); // (621) [D]
        Hook(REL_ID(21257, 21714), REL_OF(0x074, 0x074, 0)); // (622) [A] (GetPathingTargetSpeedAngle)
        Hook(REL_ID(21257, 21714), REL_OF(0x188, 0x188, 0)); // (622) [B]
        Hook(REL_ID(21257, 21714), REL_OF(0x1AD, 0x1AD, 0)); // (622) [C]
        Hook(REL_ID(21257, 21714), REL_OF(0x1DB, 0x1DB, 0)); // (622) [D]
        Hook(REL_ID(21253, 21710), REL_OF(0x069, 0x069, 0)); // (623) [A] (GetMovementSpeed)
        Hook(REL_ID(21253, 21710), REL_OF(0x08D, 0x08D, 0)); // (623) [B]
        Hook(REL_ID(21228, 21685), REL_OF(0x094, 0x094, 0)); // (624) [ ] (GetInContainer)
        Hook(REL_ID(21229, 21686), REL_OF(0x05D, 0x05D, 0)); // (625) [ ] (IsLocationLoaded)
        Hook(REL_ID(21230, 21687), REL_OF(0x078, 0x078, 0)); // (626) [ ] (IsLocAliasLoaded)
        Hook(REL_ID(21261, 21718), REL_OF(0x071, 0x06E, 0)); // (627) [ ] (IsDualCasting)
        Hook(REL_ID(21267, 21726), REL_OF(0x073, 0x070, 0)); // (632) [ ] (IsCasting)
        Hook(REL_ID(21238, 21695), REL_OF(0x062, 0x062, 0)); // (633) [A] (GetFlyingState)
        Hook(REL_ID(21238, 21695), REL_OF(0x07C, 0x07C, 0)); // (633) [B]
        Hook(REL_ID(21238, 21695), REL_OF(0x096, 0x096, 0)); // (633) [C]
        Hook(REL_ID(21268, 21727), REL_OF(0x071, 0x06E, 0)); // (635) [ ] (IsInFavorState)
        Hook(REL_ID(21269, 21728), REL_OF(0x0A3, 0x09F, 0)); // (636) [ ] (HasTwoHandedWeaponEquipped)
        Hook(REL_ID(21319, 21778), REL_OF(0x09D, 0x09D, 0)); // (638) [ ] (IsInFriendStateWithPlayer)
        Hook(REL_ID(20956, 21406), REL_OF(0x08A, 0x08A, 0)); // (639) [ ] (GetWithinDistance)
        Hook(REL_ID(20967, 21417), REL_OF(0x06C, 0x06B, 0)); // (640) [ ] (GetActorValuePercent)
        Hook(REL_ID(20964, 21414), REL_OF(0x071, 0x077, 0)); // (641) [ ] (IsUnique)
        Hook(REL_ID(21270, 21729), REL_OF(0x0BF, 0x0BF, 0)); // (642) [A] (GetLastBumpDirection)
        Hook(REL_ID(21270, 21729), REL_OF(0x0EC, 0x0EC, 0)); // (642) [B]
        Hook(REL_ID(21210, 21667), REL_OF(0x06E, 0x06E, 0)); // (644) [ ] (IsInFurnitureState)
        Hook(REL_ID(21239, 21696), REL_OF(0x09E, 0x09E, 0)); // (645) [ ] (GetIsInjured)
        Hook(REL_ID(21240, 21697), REL_OF(0x09F, 0x09F, 0)); // (646) [ ] (GetIsCrashLandRequest)
        Hook(REL_ID(21241, 21698), REL_OF(0x09F, 0x09F, 0)); // (647) [ ] (GetIsHastyLandRequest)
        Hook(REL_ID(21205, 21662), REL_OF(0x06B, 0x06B, 0)); // (650) [ ] (IsLinkedTo)
        Hook(REL_ID(21206, 21663), REL_OF(0x071, 0x071, 0)); // (651) [ ] (GetKeywordDataForCurrentLocation)
        Hook(REL_ID(21274, 21733), REL_OF(0x0EB, 0x0EA, 0)); // (652) [ ] (GetInSharedCrimeFaction)
        Hook(REL_ID(21275, 21734), REL_OF(0x0A8, 0x0A8, 0)); // (654) [ ] (GetBribeSuccess)
        Hook(REL_ID(21276, 21735), REL_OF(0x0A8, 0x0A8, 0)); // (655) [ ] (GetIntimidateSuccess)
        Hook(REL_ID(21277, 21736), REL_OF(0x0B1, 0x0B1, 0)); // (656) [ ] (GetArrestedState)
        Hook(REL_ID(21278, 21737), REL_OF(0x0A8, 0x0A8, 0)); // (657) [ ] (GetArrestingActor)
        Hook(REL_ID(21219, 21676), REL_OF(0x08F, 0x097, 0)); // (676) [ ] (GetCurrentShoutVariation)
        Hook(REL_ID(21190, 21647), REL_OF(0x07C, 0x07C, 0)); // (682) [ ] (WornHasKeyword)
        Hook(REL_ID(21258, 21715), REL_OF(0x0A3, 0x0A3, 0)); // (683) [A] (GetPathingCurrentSpeed)
        Hook(REL_ID(21258, 21715), REL_OF(0x0C3, 0x0C3, 0)); // (683) [B]
        Hook(REL_ID(21258, 21715), REL_OF(0x0EC, 0x0EC, 0)); // (683) [C]
        Hook(REL_ID(21259, 21716), REL_OF(0x074, 0x074, 0)); // (684) [A] (GetPathingCurrentSpeedAngle)
        Hook(REL_ID(21259, 21716), REL_OF(0x188, 0x188, 0)); // (684) [B]
        Hook(REL_ID(21259, 21716), REL_OF(0x1AD, 0x1AD, 0)); // (684) [C]
        Hook(REL_ID(21259, 21716), REL_OF(0x1DB, 0x1DB, 0)); // (684) [D]
        Hook(REL_ID(21011, 21461), REL_OF(0x08B, 0x088, 0)); // (694) [ ] (GetNoBleedoutRecovery)
        Hook(REL_ID(21012, 21462), REL_OF(0x0A2, 0x09F, 0)); // (698) [ ] (IsAllowedToFly)
        Hook(REL_ID(21188, 21645), REL_OF(0x065, 0x065, 0)); // (699) [ ] (HasMagicEffectKeyword)
        Hook(REL_ID(21189, 21646), REL_OF(0x067, 0x06D, 0)); // (700) [ ] (IsCommandedActor)
        Hook(REL_ID(21311, 21770), REL_OF(0x06B, 0x071, 0)); // (701) [ ] (IsStaggered)
        Hook(REL_ID(21312, 21771), REL_OF(0x06A, 0x070, 0)); // (702) [ ] (IsRecoiling)
        Hook(REL_ID(21313, 21772), REL_OF(0x06E, 0x06E, 0)); // (704) [ ] (IsPathing)
        Hook(REL_ID(21023, 21473), REL_OF(0x080, 0x083, 0)); // (705) [ ] (GetShouldHelp)
        Hook(REL_ID(21315, 21774), REL_OF(0x0C6, 0x0C1, 0)); // (706) [ ] (HasBoundWeaponEquipped)
        Hook(REL_ID(21317, 21776), REL_OF(0x14C, 0x14C, 0)); // (707) [ ] (GetCombatTargetHasKeyword)
        Hook(REL_ID(21318, 21777), REL_OF(0x075, 0x075, 0)); // (709) [ ] (GetCombatGroupMemberCount)
        Hook(REL_ID(21323, 21782), REL_OF(0x06E, 0x06E, 0)); // (710) [ ] (IsIgnoringCombat)
        Hook(REL_ID(21324, 21783), REL_OF(0x068, 0x067, 0)); // (711) [ ] (GetLightLevel)
        Hook(REL_ID(21326, 21785), REL_OF(0x0AC, 0x09C, 0)); // (716) [ ] (GetRealHoursPassed)
        Hook(REL_ID(21327, 21786), REL_OF(0x078, 0x074, 0)); // (718) [ ] (IsUnlockedDoor)
        Hook(REL_ID(21022, 21472), REL_OF(0x07D, 0x07D, 0)); // (719) [ ] (IsHostileToActor)
        Hook(REL_ID(20987, 21437), REL_OF(0x062, 0x062, 0)); // (722) [A] (WornApparelHasKeywordCount)
        Hook(REL_ID(20987, 21437), REL_OF(0x0D4, 0x0D4, 0)); // (722) [B]
        Hook(REL_ID(20988, 21438), REL_OF(0x056, 0x056, 0)); // (723) [ ] (GetItemHealthPercent)
        Hook(REL_ID(21058, 21508), REL_OF(0x076, 0x079, 0)); // (725) [ ] (GetKnockStateEnum)
        Hook(REL_ID(21014, 21464), REL_OF(0x0A4, 0x0A5, 0)); // (730) [A] (IsOnFlyingMount)
        Hook(REL_ID(21014, 21464), REL_OF(0x0CD, 0x0BA, 0)); // (730) [B]
        Hook(REL_ID(21013, 21463), REL_OF(0x0A4, 0x0A5, 0)); // (731) [A] (CanFlyHere)
        Hook(REL_ID(21013, 21463), REL_OF(0x0CD, 0x0BA, 0)); // (731) [B]
        Hook(REL_ID(21015, 21465), REL_OF(0x0DB, 0x0D7, 0)); // (732) [A] (IsFlyingMountPatrolQueued)
        Hook(REL_ID(21015, 21465), REL_OF(0x0F6, 0x0F2, 0)); // (732) [B]
        Hook(REL_ID(21016, 21466), REL_OF(0x078, 0x075, 0)); // (733) [ ] (IsFlyingMountFastTravelling)
        Hook(REL_ID(21017, 21467), REL_OF(0x099, 0x096, 0)); // (734) [A] (IsOverEncumbered)
        Hook(REL_ID(21017, 21467), REL_OF(0x0DE, 0x0DB, 0)); // (734) [B]
        Hook(REL_ID(21017, 21467), REL_OF(0x103, 0x100, 0)); // (734) [C]
        Hook(REL_ID(21018, 21468), REL_OF(0x092, 0x099, 0)); // (735) [A] (GetActorWarmth)
        Hook(REL_ID(21018, 21468), REL_OF(0x0B7, 0x0AE, 0)); // (735) [B]
    }
};

SKSEPluginLoad(const SKSE::LoadInterface* a_skse)
{
    InitLogging();

    const auto plugin = SKSE::PluginDeclaration::GetSingleton();
    logs::info("{} v{} is loading...", plugin->GetName(), plugin->GetVersion());

    SKSE::Init(a_skse);
    SKSE::AllocTrampoline(48);
    ConsoleLogHook::Install();

    logs::info("{} loaded.", plugin->GetName());

    return true;
}
