/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_VIOLET_H
#define DEF_VIOLET_H

enum
{
    MAX_ENCOUNTER               = 10,

    TYPE_MAIN                   = 0,
    TYPE_PORTAL                 = 1,
    TYPE_SEAL                   = 2,

    TYPE_EREKEM                 = 3,
    TYPE_MORAGG                 = 4,
    TYPE_ICHORON                = 5,
    TYPE_XEVOZZ                 = 6,
    TYPE_LAVANTHOR              = 7,
    TYPE_ZURAMAT                = 8,
    TYPE_CYANIGOSA              = 9,

    TYPE_LASTBOSS               = 11,
    TYPE_DOOR                   = 12,
    TYPE_SEAL_DMG_SAY           = 13,

    TYPE_RAND_BOSS_ID           = 14,

    DATA_EREKEM                 = 23,
    DATA_MORAGG                 = 24,
    DATA_ICHORON                = 25,
    DATA_XEVOZZ                 = 26,
    DATA_LAVANTHOR              = 27,
    DATA_ZURAMAT                = 28,
    DATA_SINCLARI               = 29,
    DATA_NPC_SEAL_DOOR          = 31,

    DATA_SEAL_DOOR              = 32,
    DATA_EREKEM_DOOR            = 33,
    DATA_MORAGG_DOOR            = 34,
    DATA_ICHORON_DOOR           = 35,
    DATA_XEVOZZ_DOOR            = 36,
    DATA_LAVANTHOR_DOOR         = 37,
    DATA_ZURAMAT_DOOR           = 38,
    DATA_EREKEM_DOOR_L          = 39,
    DATA_EREKEM_DOOR_R          = 40,

    WORLD_STATE_ID              = 3816,
    WORLD_STATE_SEAL            = 3815,
    WORLD_STATE_PORTALS         = 3810,

    GO_INTRO_CRYSTAL            = 193615,

    GO_DOOR_SEAL                = 191723,
    GO_DOOR_EREKEM              = 191564,
    GO_DOOR_EREKEM_RIGHT        = 191563,
    GO_DOOR_EREKEM_LEFT         = 191562,
    GO_DOOR_MORAGG              = 191606,
    GO_DOOR_ICHORON             = 191722,
    GO_DOOR_XEVOZZ              = 191556,
    GO_DOOR_LAVANTHOR           = 191566,
    GO_DOOR_ZURAMAT             = 191565,

    NPC_EVENT_CONTROLLER        = 30883,
    NPC_PORTAL_INTRO            = 31011,
    NPC_PORTAL                  = 30679,
    NPC_PORTAL_ELITE            = 32174,
    NPC_DOOR_SEAL               = 30896,

    NPC_SINCLARI                = 30658,
    NPC_SINCLARI_ALT            = 32204,                    // yeller for seal weakening and summoner for portals
    NPC_HOLD_GUARD              = 30659,

    NPC_EREKEM                  = 29315,
    NPC_EREKEM_GUARD            = 29395,
    NPC_MORAGG                  = 29316,
    NPC_ICHORON                 = 29313,
    NPC_XEVOZZ                  = 29266,
    NPC_LAVANTHOR               = 29312,
    NPC_ZURAMAT                 = 29314,
    NPC_CYANIGOSA               = 31134,

    NPC_PORTAL_GUARDIAN         = 30660,
    NPC_PORTAL_KEEPER           = 30695,

    NPC_AZURE_INVADER           = 30661,
    NPC_AZURE_SPELLBREAKER      = 30662,
    NPC_AZURE_BINDER            = 30663,
    NPC_AZURE_MAGE_SLAYER       = 30664,
    NPC_MAGE_HUNTER             = 30665,
    NPC_AZURE_CAPTAIN           = 30666,
    NPC_AZURE_SORCEROR          = 30667,
    NPC_AZURE_RAIDER            = 30668,
    NPC_AZURE_STALKER           = 32191,

    // used for intro
    NPC_AZURE_BINDER_INTRO      = 31007,
    NPC_AZURE_INVADER_INTRO     = 31008,
    NPC_AZURE_SPELLBREAKER_INTRO= 31009,
    NPC_AZURE_MAGE_SLAYER_INTRO = 31010,

    NPC_AZURE_SABOTEUR          = 31079,

    NPC_DEFENSE_SYSTEM          = 30837,
    NPC_DEFENSE_DUMMY_TARGET    = 30857,

    NPC_ARAKKOA                 = 32226,
    NPC_VOID_LORD               = 32230,
    NPC_ETHERAL                 = 32231,
    NPC_SWIRLING                = 32234,
    NPC_WATCHER                 = 32235,
    NPC_LAVA_HOUND              = 32237,

    SPELL_DEFENSE_SYSTEM_VISUAL = 57887,
    SPELL_DEFENSE_SYSTEM_SPAWN  = 57886,

    SPELL_DESTROY_DOOR_SEAL     = 58040,                    // spell periodic cast by misc
    SPELL_TELEPORTATION_PORTAL  = 57687,                    // visual aura, but possibly not used? creature_template model for portals are same

    SPELL_SHIELD_DISRUPTION     = 58291,                    // dummy when opening a cell

    SPELL_PORTAL_PERIODIC       = 58008,                    // most likely the tick for each summon (tick each 15 seconds)
    SPELL_PORTAL_CHANNEL        = 58012,                    // the blue "stream" between portal and guardian/keeper
    SPELL_PORTAL_BEAM           = 56046,                    // large beam, unsure if really used here (or possible for something different)

    SPELL_PORTAL_VISUAL_1       = 57872,                    // no idea, but is possibly related based on it's visual appearence
    SPELL_PORTAL_VISUAL_2       = 57630,

    SAY_SEAL_75                 = -1608002,
    SAY_SEAL_50                 = -1608003,
    SAY_SEAL_5                  = -1608004,

    EMOTE_GUARDIAN_PORTAL       = -1608005,
    EMOTE_DRAGONFLIGHT_PORTAL   = -1608006,
    EMOTE_KEEPER_PORTAL         = -1608007,

    MAX_NORMAL_PORTAL           = 8
};

static float fDefenseSystemLoc[4] = {1888.146f, 803.382f, 58.604f, 3.072f};

enum ePortalType
{
    PORTAL_TYPE_NORM = 0,
    PORTAL_TYPE_SQUAD,
    PORTAL_TYPE_BOSS,
};

struct sPortalData
{
    ePortalType pPortalType;
    float fX, fY, fZ, fOrient;
};

static sPortalData afPortalLocation[]=
{
    {PORTAL_TYPE_NORM, 1936.07f, 803.198f, 53.3749f, 3.1241f},  //balcony
    {PORTAL_TYPE_NORM, 1877.51f, 850.104f, 44.6599f, 4.7822f},  //erekem
    {PORTAL_TYPE_NORM, 1890.64f, 753.471f, 48.7224f, 1.7104f},  //moragg
    {PORTAL_TYPE_SQUAD, 1911.06f, 802.103f, 38.6465f, 2.8908f}, //below balcony
    {PORTAL_TYPE_SQUAD, 1928.06f, 763.256f, 51.3167f, 2.3905f}, //bridge
    {PORTAL_TYPE_SQUAD, 1924.26f, 847.661f, 47.1591f, 4.0202f}, //zuramat
    {PORTAL_TYPE_NORM, 1914.16f, 832.527f, 38.6441f, 3.5160f},  //xevozz
    {PORTAL_TYPE_NORM, 1857.30f, 764.145f, 38.6543f, 0.8339f},  //lavanthor
    {PORTAL_TYPE_BOSS, 1890.73f, 803.309f, 38.4001f, 2.4139f},  //center
};

class MANGOS_DLL_DECL instance_violet_hold : public ScriptedInstance
{
    public:
        instance_violet_hold(Map* pMap);
        ~instance_violet_hold() {}

        void Initialize();
        void ResetAll();
        void ResetVariables();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void UpdateCellForBoss(uint32 uiBossEntry);
        void UpdateWorldState(bool bEnable = true);

        void OnPlayerEnter(Player* pPlayer);

        void SetIntroPortals(bool bDeactivate);
        void SpawnPortal();

        void SetPortalId();

        void CallGuards(bool bRespawn);

        uint32 GetRandomPortalEliteEntry();
        uint32 GetRandomMobForNormalPortal();

        uint32 GetCurrentPortalNumber() { return m_uiWorldStatePortalCount; }

        sPortalData const* GetPortalData() { return &afPortalLocation[m_uiPortalId]; }

        bool IsCurrentPortalForTrash()
        {
            if (m_uiWorldStatePortalCount % 6)
                return true;

            return false;
        }

        bool IsNextPortalForTrash()
        {
            if ((m_uiWorldStatePortalCount+1) % 6)
                return true;

            return false;
        }

        void ProcessActivationCrystal(Unit* pUser, bool bIsIntro = false);

        void SetRandomBosses();

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        void Update(uint32 uiDiff);

        typedef std::multimap<uint32, uint64> BossToCellMap;

    protected:

        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint64 m_uiSinclariGUID;
        uint64 m_uiSinclariAltGUID;
        uint64 m_uiErekemGUID;
        uint64 m_uiMoraggGUID;
        uint64 m_uiIchoronGUID;
        uint64 m_uiXevozzGUID;
        uint64 m_uiLavanthorGUID;
        uint64 m_uiZuramatGUID;

        uint64 m_uiSealDoorGUID;
        uint64 m_uiErekemDoorGUID;
        uint64 m_uiErekemDoorLeftGUID;
        uint64 m_uiErekemDoorRightGUID;
        uint64 m_uiMoraggDoorGUID;
        uint64 m_uiIchoronDoorGUID;
        uint64 m_uiXevozzDoorGUID;
        uint64 m_uiLavanthorDoorGUID;
        uint64 m_uiZuramatDoorGUID;

        uint64 m_uiCellErekemGuard_LGUID;
        uint64 m_uiCellErekemGuard_RGUID;
        uint64 m_uiIntroCrystalGUID;

        uint32 m_uiWorldState;
        uint32 m_uiWorldStateSealCount;
        uint32 m_uiWorldStatePortalCount;

        uint8 m_uiPortalId;
        uint32 m_uiPortalTimer;
        uint32 m_uiMaxCountPortalLoc;
        uint32 m_uiSealDmgSay;
        uint32 m_uiChosenBoss;

        std::list<uint64> m_lIntroPortalList;
        std::list<uint64> m_lGuardsList;
};

struct Locations
{
    float x, y, z;
    uint32 id;
};

static Locations BossLoc[]=
{
    {1876.100f, 857.079f, 43.333f}, // Erekem
    {1892.737f, 744.589f, 47.666f}, // Moragg
    {1908.863f, 785.647f, 37.435f}, // Ichoron
    {1905.364f, 840.607f, 38.670f}, // Xevozz
    {1857.125f, 763.295f, 38.654f}, // Lavanthor
    {1925.480f, 849.981f, 47.174f}, // Zuramat
};

static Locations PortalLoc[]=
{
    {1888.271f, 810.781f, 38.441f}, // 0 center
    {1857.125f, 763.295f, 38.654f}, // 1 Lavanthor
    {1925.480f, 849.981f, 47.174f}, // 2 Zuramat
    {1892.737f, 744.589f, 47.666f}, // 3 Moragg
    {1878.198f, 850.005f, 43.333f}, // 4 Portal in front of Erekem
    {1909.381f, 806.796f, 38.645f}, // 5 Portal outside of Ichoron
    {1936.101f, 802.950f, 52.417f}, // 6 at the highest platform
};

#endif
