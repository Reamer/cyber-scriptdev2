/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_AHNKAHET_H
#define DEF_AHNKAHET_H
/* Encounters
 * Elder Nadox         = 1
 * Prince Taldram      = 2
 * Jedoga Shadowseeker = 3
 * Herald Volazj       = 4
 * Amanitar            = 5
*/
enum
{
    MAX_ENCOUNTER           = 5,

    TYPE_NADOX              = 0,
    TYPE_TALDARAM           = 1,
    TYPE_JEDOGA             = 2,
    TYPE_VOLAZJ             = 3,
    TYPE_AMANITAR           = 4,

    GO_DOOR_TALDARAM        = 192236,
    GO_ANCIENT_DEVICE_L     = 193093,
    GO_ANCIENT_DEVICE_R     = 193094,
    GO_VORTEX               = 193564,

    NPC_ELDER_NADOX         = 29309,
    NPC_TALDARAM            = 29308,
    NPC_JEDOGA_SHADOWSEEKER = 29310,
    NPC_TWILIGHT_INITIATE   = 30114,


    ACHIEV_CRITERIA_VOLUNTEER_WORK      = 7359,
    ACHIEV_CRITERIA_RESPECT_YOUR_ELDERS = 7317
};

class MANGOS_DLL_DECL instance_ahnkahet : public ScriptedInstance
{
    public:
        instance_ahnkahet(Map* pMap);
        ~instance_ahnkahet() {}

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        void SetAchiev(uint32 uiType, bool get);
        uint32 GetData(uint32 uiType);

        bool CheckAchievementCriteriaMeet(uint32 uiCriteriaId, Player const* pSource, Unit const* pTarget, uint32 uiMiscValue1 /* = 0*/);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

        GUIDList m_lTwilightInitiate;


    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        bool   m_bCriteriaVolunteerWork;
        bool   m_bCriteriaRespectYourElders;

        uint8  m_uiDevicesActivated;
};

#endif
