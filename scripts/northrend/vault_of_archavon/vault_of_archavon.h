/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_VAULT_OF_ARCHAVON_H
#define DEF_VAULT_OF_ARCHAVON_H

enum
{
    MAX_ENCOUNTER               = 4,

    TYPE_ARCHAVON               = 0,
    TYPE_EMALON                 = 1,
    TYPE_KORALON                = 2,
    TYPE_TORAVON                = 3,

    DATA_TEMPEST_MINION_1       = 101,
    DATA_TEMPEST_MINION_2       = 102,
    DATA_TEMPEST_MINION_3       = 103,
    DATA_TEMPEST_MINION_4       = 104,


    NPC_TORAVON                 = 38433,
    NPC_ARCHAVON                = 31125,
    NPC_EMALON                  = 33993,
    NPC_KORALON                 = 35013,
    NPC_TEMPEST_MINION          = 33998,
};

class MANGOS_DLL_DECL instance_vault_of_archavon : public ScriptedInstance
{
    public:
        instance_vault_of_archavon(Map* pMap);

        void Initialize();

        void OnCreatureCreate(Creature* pCreature);

        uint32 GetData(uint32 uiType);
        void SetData(uint32 uiType, uint32 uiData);

        void Load(const char* chrIn);

        bool IsEncounterInProgress() const;
       
        GUIDList m_lTempestMinion;
    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;
};

#endif
