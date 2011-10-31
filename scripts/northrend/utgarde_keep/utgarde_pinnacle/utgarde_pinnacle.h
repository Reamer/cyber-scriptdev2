/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_UTG_PINNACLE_H
#define DEF_UTG_PINNACLE_H

enum
{
    MAX_ENCOUNTER                   = 4,

    TYPE_SVALA                      = 0,
    TYPE_GORTOK                     = 1,
    TYPE_SKADI                      = 2,
    TYPE_YMIRON                     = 3,

    GO_STASIS_GENERATOR             = 188593,
    GO_DOOR_SKADI                   = 192173,

    NPC_FURBOLG                     = 26684,
    NPC_WORGEN                      = 26683,
    NPC_JORMUNGAR                   = 26685,
    NPC_RHINO                       = 26686,

    NPC_GRAUF                       = 26893,
    NPC_SKADI                       = 26693,
    NPC_YMIRJAR_WARRIOR             = 26690,
    NPC_YMIRJAR_WITCH_DOCTOR        = 26691,
    NPC_YMIRJAR_HARPOONER           = 26692,

    NPC_FLAME_BRAZIER               = 27273,            // throw flames at players on Svalna event
    NPC_FLAME_BREATH_TRIGGER        = 28351,
    NPC_YMIRON                      = 26861,

    SPELL_BALL_OF_FLAME             = 48246,            // spell used by the flame braziers
    SPELL_FREEZING_CLOUD            = 47579,
};

class MANGOS_DLL_DECL instance_pinnacle : public ScriptedInstance
{
    public:
        instance_pinnacle(Map* pMap);

        void Initialize();

        void OnObjectCreate(GameObject* pGo);
        void OnCreatureCreate(Creature* pCreature);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

        void DoProcessCallFlamesEvent();
        void DoMakeFreezingCloud();

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        GUIDList m_lFlameBraziersList;

        GUIDList m_lFlameBreathTrigger;
};

#endif
