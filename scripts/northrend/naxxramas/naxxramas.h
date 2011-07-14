/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_NAXXRAMAS_H
#define DEF_NAXXRAMAS_H

enum
{
    MAX_ENCOUNTER               = 15,

    // Kel'Thuzad's taunts after killing Wing Bosses
    SAY_KELTHUZAD_TAUNT1        = -1533090,
    SAY_KELTHUZAD_TAUNT2        = -1533091,
    SAY_KELTHUZAD_TAUNT3        = -1533092,
    SAY_KELTHUZAD_TAUNT4        = -1533093,

    TYPE_ANUB_REKHAN            = 0,
    TYPE_FAERLINA               = 1,
    TYPE_MAEXXNA                = 2,

    TYPE_NOTH                   = 3,
    TYPE_HEIGAN                 = 4,
    TYPE_LOATHEB                = 5,

    TYPE_RAZUVIOUS              = 6,
    TYPE_GOTHIK                 = 7,
    TYPE_FOUR_HORSEMEN          = 8,
    TYPE_BLAUMEUX               = 81,
    TYPE_RIVENDARE              = 82,
    TYPE_KORTHAZZ               = 83,
    TYPE_ZELIEK                 = 84,

    TYPE_PATCHWERK              = 9,
    TYPE_GROBBULUS              = 10,
    TYPE_GLUTH                  = 11,
    TYPE_THADDIUS               = 12,
    TYPE_STALAGG                = 121,
    TYPE_FEUGEN                 = 122,

    TYPE_SAPPHIRON              = 13,
    TYPE_KELTHUZAD              = 14,

    NPC_ANUB_REKHAN             = 15956,
    NPC_FAERLINA                = 15953,
    NPC_WORSHIPPER              = 16506,
    NPC_FOLLOWER                = 16505,
    NPC_MAEXXNA                 = 15952,

    NPC_PATCHWERK               = 16028,
    NPC_GROBBULUS               = 15931,
    NPC_GLUTH                   = 15932,
    NPC_THADDIUS                = 15928,
    NPC_STALAGG                 = 15929,
    NPC_FEUGEN                  = 15930,

    NPC_NOTH                    = 15954,
    NPC_HEIGAN                  = 15936,
    NPC_LOATHEB                 = 16011,

    NPC_RAZUVIOUS               = 16061,
    NPC_DEATH_KNIGHT_UNDERSTUDY = 16803,

    
    NPC_ZELIEK                  = 16063,
    NPC_THANE                   = 16064,
    NPC_BLAUMEUX                = 16065,
    NPC_RIVENDARE               = 30549,

    NPC_SAPPHIRON               = 15989,
    NPC_KELTHUZAD               = 15990,

    // Gothik
    NPC_GOTHIK                  = 16060,
    NPC_SUB_BOSS_TRIGGER        = 16137,                    //summon locations
    NPC_UNREL_TRAINEE           = 16124,
    NPC_UNREL_DEATH_KNIGHT      = 16125,
    NPC_UNREL_RIDER             = 16126,
    NPC_SPECT_TRAINEE           = 16127,
    NPC_SPECT_DEATH_KNIGTH      = 16148,
    NPC_SPECT_RIDER             = 16150,
    NPC_SPECT_HORSE             = 16149,

    // End boss adds
    NPC_SOLDIER_FROZEN          = 16427,
    NPC_UNSTOPPABLE_ABOM        = 16428,
    NPC_SOUL_WEAVER             = 16429,
    NPC_GUARDIAN                = 16441,

    // Arachnid Quarter
    GO_ARAC_ANUB_DOOR           = 181126,                   //encounter door
    GO_ARAC_ANUB_GATE           = 181195,                   //open after boss is dead
    GO_ARAC_FAER_WEB            = 181235,                   //encounter door
    GO_ARAC_FAER_DOOR           = 194022,                   //after faerlina, to outer ring
    GO_ARAC_MAEX_INNER_DOOR     = 181197,                   //encounter door
    GO_ARAC_MAEX_OUTER_DOOR     = 181209,                   //right before maex

    // Plague Quarter
    GO_PLAG_SLIME01_DOOR        = 181198,                   //not used
    GO_PLAG_SLIME02_DOOR        = 181199,                   //not used
    GO_PLAG_NOTH_ENTRY_DOOR     = 181200,                   //encounter door
    GO_PLAG_NOTH_EXIT_DOOR      = 181201,                   //exit, open when boss dead
    GO_PLAG_HEIG_ENTRY_DOOR     = 181202,
    GO_PLAG_HEIG_EXIT_DOOR      = 181203,                   //exit, open when boss dead
    GO_PLAG_LOAT_DOOR           = 181241,                   //encounter door
    GO_ERUPTION_ONE_OBJECT      = 181678,                   //eruption objects
    GO_ERUPTION_TWO_OBJECT      = 181676,
    GO_ERUPTION_THREE_OBJECT    = 181677,
    GO_ERUPTION_FOUR_OBJECT     = 181695,

    // Military Quarter
    GO_MILI_GOTH_ENTRY_GATE     = 181124,                   //used while encounter is in progress
    GO_MILI_GOTH_EXIT_GATE      = 181125,                   //exit, open at boss dead
    GO_MILI_GOTH_COMBAT_GATE    = 181170,                   //used while encounter is in progress
    GO_MILI_HORSEMEN_DOOR       = 181119,                   //encounter door

    GO_CHEST_HORSEMEN_NORM      = 181366,                   //four horsemen event, DoRespawnGameObject() when event == DONE
    GO_CHEST_HORSEMEN_HERO      = 193426,

    // Construct Quarter
    GO_CONS_PATH_EXIT_DOOR      = 181123,
    GO_CONS_GLUT_EXIT_DOOR      = 181120,
    GO_CONS_THAD_DOOR           = 181121,                   // Thaddius enc door

    // Frostwyrm Lair
    GO_KELTHUZAD_WATERFALL_DOOR = 181225,                   // exit, open after sapphiron is dead
    GO_KELTHUZAD_EXIT_DOOR      = 181228,

    // Eyes
    GO_ARAC_EYE_RAMP            = 181212,
    GO_PLAG_EYE_RAMP            = 181211,
    GO_MILI_EYE_RAMP            = 181210,
    GO_CONS_EYE_RAMP            = 181213,

    // Portals
    GO_ARAC_PORTAL              = 181575,
    GO_PLAG_PORTAL              = 181577,
    GO_MILI_PORTAL              = 181578,
    GO_CONS_PORTAL              = 181576,

    AREATRIGGER_FROSTWYRM       = 4120,                    //not needed here, but AT to be scripted
    AREATRIGGER_KELTHUZAD       = 4112,
    AREATRIGGER_GOTHIK          = 4116,

    //Achievements
    ACHIEV_CRITERIA_THE_SAFETY_DANCE            = 7264,
    ACHIEV_CRITERIA_THE_SAFETY_DANCE_H          = 7548,
    ACHIEV_CRITERIA_MOMMA_SAID_KNOCK_YOU_OUT    = 7265,
    ACHIEV_CRITERIA_MOMMA_SAID_KNOCK_YOU_OUT_H  = 7549,
    ACHIEV_CRITERIA_SPORE_LOSER                 = 7612,
    ACHIEV_CRITERIA_SPORE_LOSER_H               = 7613,
    ACHIEV_CRITERIA_SHOCKING                    = 7604,
    ACHIEV_CRITERIA_SHOCKING_H                  = 7605,
    ACHIEV_CRITERIA_THE_HUNDRED_CLUB            = 7567,
    ACHIEV_CRITERIA_THE_HUNDRED_CLUB_H          = 7568,
    ACHIEV_CRITERIA_JUST_CANT_GET_ENOUGH        = 7614,
    ACHIEV_CRITERIA_JUST_CANT_GET_ENOUGH_H      = 7615,

    ACHIEV_CRITERIA_THE_IMMORTAL_1              = 13235,
    ACHIEV_CRITERIA_THE_IMMORTAL_2              = 13234,
    ACHIEV_CRITERIA_THE_IMMORTAL_3              = 7616,
    ACHIEV_CRITERIA_THE_IMMORTAL_4              = 13236,
    ACHIEV_CRITERIA_THE_IMMORTAL_5              = 13233,

    ACHIEV_CRITERIA_THE_UNDYING_1               = 13240,
    ACHIEV_CRITERIA_THE_UNDYING_2               = 13239,
    ACHIEV_CRITERIA_THE_UNDYING_3               = 7617,
    ACHIEV_CRITERIA_THE_UNDYING_4               = 13238,
    ACHIEV_CRITERIA_THE_UNDYING_5               = 13237,

    ACHIEV_CRITERIA_ARCHNOPHOBIA                = 7128,
    ACHIEV_CRITERIA_ARCHNOPHOBIA_H              = 7129,
};

struct GothTrigger
{
    bool bIsRightSide;
    bool bIsAnchorHigh;
};

class MANGOS_DLL_DECL instance_naxxramas : public ScriptedInstance
{
    public:
        instance_naxxramas(Map* pMap);
        ~instance_naxxramas() {}

        void Initialize();

        bool IsEncounterInProgress() const;
        bool IsAllEncounterDone();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        void SetAchiev(uint32 uiType, bool get);
        void OnPlayerDeath(Player* pPlayer);
        void OnPlayerLeave(Player* pPlayer);

        uint32 GetData(uint32 uiType);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);

        // goth
        void SetGothTriggers();
        Creature* GetClosestAnchorForGoth(Creature* pSource, bool bRightSide);
        void GetGothSummonPointCreatures(std::list<Creature*> &lList, bool bRightSide);
        bool IsInRightSideGothArea(Unit* pUnit);

        // kel
        void SetChamberCenterCoords(float fX, float fY, float fZ);
        void GetChamberCenterCoords(float &fX, float &fY, float &fZ) { fX = m_fChamberCenterX; fY = m_fChamberCenterY; fZ = m_fChamberCenterZ; }
        void DoTaunt();

        //Heigan
        GUIDList m_lEruptionObjectOneGUIDs;
        GUIDList m_lEruptionObjectTwoGUIDs;
        GUIDList m_lEruptionObjectThreeGUIDs;
        GUIDList m_lEruptionObjectFourGUIDs;

        // Gothik
        GUIDList m_lGothTriggerList;
        UNORDERED_MAP<uint64, GothTrigger> m_mGothTriggerMap;
        // Faerlina
        GUIDList m_lFaerlinaAddsGUID;
        // Razuvious
        GUIDVector m_lDeathKnightUnderstudyGUID;

        time_t m_uiArachnophobiaTimer;
        
    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        bool m_bBlaumeuxDead;
        bool m_bRivendareDead;
        bool m_bZeliekDead;
        bool m_bKorthazzDead;

        float m_fChamberCenterX;
        float m_fChamberCenterY;
        float m_fChamberCenterZ;

        //Achievements
        bool m_bTheSafetyDance;
        bool m_bMommaSaidKnockYouOut;
        bool m_bSporeLoser;
        bool m_bShocking;
        bool m_bTheHundredClub;
        bool m_bJustCantGetEnough;
        bool m_bMonsterRun;
};

#endif
