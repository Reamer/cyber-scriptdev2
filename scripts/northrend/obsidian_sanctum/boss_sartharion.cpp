/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Boss Sartharion
SD%Complete: 100%
SDComment: It's alive! ;) Now this is script is alive realy! (c) MaxXx2021 and PSZ
TODO: Will of sartharoion to all dragons
SDCategory: Obsidian Sanctum
EndScriptData */

#include "precompiled.h"
#include "obsidian_sanctum.h"

enum
{
    //Sartharion Yell
    SAY_SARTHARION_AGGRO                        = -1615020,
    SAY_SARTHARION_BERSERK                      = -1615021,
    SAY_SARTHARION_BREATH                       = -1615022,
    SAY_SARTHARION_CALL_SHADRON                 = -1615023,
    SAY_SARTHARION_CALL_TENEBRON                = -1615024,
    SAY_SARTHARION_CALL_VESPERON                = -1615025,
    SAY_SARTHARION_DEATH                        = -1615026,
    SAY_SARTHARION_SPECIAL_1                    = -1615027,
    SAY_SARTHARION_SPECIAL_2                    = -1615028,
    SAY_SARTHARION_SPECIAL_3                    = -1615029,
    SAY_SARTHARION_SPECIAL_4                    = -1615030,
    SAY_SARTHARION_SLAY_1                       = -1615031,
    SAY_SARTHARION_SLAY_2                       = -1615032,
    SAY_SARTHARION_SLAY_3                       = -1615033,

    WHISPER_LAVA_CHURN                          = -1615034,

    WHISPER_SHADRON_DICIPLE                     = -1615008,
    WHISPER_VESPERON_DICIPLE                    = -1615048,
    WHISPER_HATCH_EGGS                          = -1615018,
    WHISPER_OPEN_PORTAL                         = -1615035, // whisper, shared by two dragons

    //Sartharion Spells
    SPELL_BERSERK                               = 61632,    // Increases the caster's attack speed by 150% and all damage it deals by 500% for 5 min.
    SPELL_CLEAVE                                = 56909,    // Inflicts 35% weapon damage to an enemy and its nearest allies, affecting up to 10 targets.
    SPELL_FLAME_BREATH                          = 56908,    // Inflicts 8750 to 11250 Fire damage to enemies in a cone in front of the caster.
    SPELL_FLAME_BREATH_H                        = 58956,    // Inflicts 10938 to 14062 Fire damage to enemies in a cone in front of the caster.
    SPELL_TAIL_LASH                             = 56910,    // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
    SPELL_TAIL_LASH_H                           = 58957,    // A sweeping tail strike hits all enemies behind the caster, inflicting 4375 to 5625 damage and stunning them for 2 sec.
    SPELL_WILL_OF_SARTHARION                    = 61254,    // Sartharion's presence bolsters the resolve of the Twilight Drakes, increasing their total health by 25%. This effect also increases Sartharion's health by 25%.
    SPELL_LAVA_STRIKE                           = 57571,    // (Real spell casted should be 57578) 57571 then trigger visual missile, then summon Lava Blaze on impact(spell 57572)
    SPELL_CYCLONE_AURA_2                        = 57598,
    SPELL_TWILIGHT_REVENGE                      = 60639,

    SPELL_PYROBUFFET                            = 56916,    // currently used for hard enrage after 15 minutes
    SPELL_PYROBUFFET_RANGE                      = 58907,    // possibly used when player get too far away from dummy creatures (2x creature entry 30494)

    SPELL_TWILIGHT_SHIFT_ENTER                  = 57620,    // enter phase. Player get this when click GO
    SPELL_TWILIGHT_SHIFT_DMG                    = 57874,    // damage during being in twilight realm
    SPELL_TWILIGHT_SHIFT_REMOVAL                = 61187,    // leave phase
    SPELL_TWILIGHT_SHIFT_REMOVAL_ALL            = 61190,    // leave phase (probably version to make all leave)

    //Mini bosses common spells
    SPELL_TWILIGHT_RESIDUE                      = 61885,    // makes immune to shadow damage, applied when leave phase

    //Miniboses (Vesperon, Shadron, Tenebron)
    SPELL_SHADOW_BREATH_H                       = 59126,    // Inflicts 8788 to 10212 Fire damage to enemies in a cone in front of the caster.
    SPELL_SHADOW_BREATH                         = 57570,    // Inflicts 6938 to 8062 Fire damage to enemies in a cone in front of the caster.

    SPELL_SHADOW_FISSURE_H                      = 59127,    // Deals 9488 to 13512 Shadow damage to any enemy within the Shadow fissure after 5 sec.
    SPELL_SHADOW_FISSURE                        = 57579,    // Deals 6188 to 8812 Shadow damage to any enemy within the Shadow fissure after 5 sec.

    //Vesperon
    //In portal is a disciple, when disciple killed remove Power_of_vesperon, portal open multiple times
    SPELL_POWER_OF_VESPERON                     = 61251,    // Vesperon's presence decreases the maximum health of all enemies by 25%.
    SPELL_TWILIGHT_TORMENT_VESP                 = 57948,    // (Shadow only) trigger 57935 then 57988
    SPELL_TWILIGHT_TORMENT_VESP_ACO             = 58853,    // (Fire and Shadow) trigger 58835 then 57988

    //Shadron
    //In portal is a disciple, when disciple killed remove Power_of_vesperon, portal open multiple times
    SPELL_POWER_OF_SHADRON                      = 58105,    // Shadron's presence increases Fire damage taken by all enemies by 100%.
    SPELL_GIFT_OF_TWILIGTH_SHA                  = 57835,    // TARGET_SCRIPT shadron
    SPELL_GIFT_OF_TWILIGTH_SAR                  = 58766,    // TARGET_SCRIPT sartharion

    //Tenebron
    //in the portal spawns 6 eggs, if not killed in time (approx. 20s)  they will hatch,  whelps can cast 60708
    SPELL_POWER_OF_TENEBRON                     = 61248,    // Tenebron's presence increases Shadow damage taken by all enemies by 100%.
    //Tenebron, dummy spell
    SPELL_SUMMON_TWILIGHT_WHELP                 = 58035,    // doesn't work, will spawn NPC_TWILIGHT_WHELP
    SPELL_SUMMON_SARTHARION_TWILIGHT_WHELP      = 58826,    // doesn't work, will spawn NPC_SHARTHARION_TWILIGHT_WHELP

    SPELL_HATCH_EGGS_H                          = 59189,
    SPELL_HATCH_EGGS                            = 58542,
    SPELL_HATCH_EGGS_EFFECT_H                   = 59190,
    SPELL_HATCH_EGGS_EFFECT                     = 58685,

    //Whelps
    NPC_TWILIGHT_WHELP                          = 30890,
    NPC_SHARTHARION_TWILIGHT_WHELP              = 31214,
    SPELL_FADE_ARMOR                            = 60708,    // Reduces the armor of an enemy by 1500 for 15s

    //flame tsunami
    SPELL_FLAME_TSUNAMI                         = 57494,    // the visual dummy
    SPELL_FLAME_TSUNAMI_LEAP                    = 60241,    // SPELL_EFFECT_138 some leap effect, causing caster to move in direction
    SPELL_FLAME_TSUNAMI_DMG_AURA                = 57492,    // periodic damage, npc has this aura
    SPELL_FLAME_TSUNAMI_DMG                     = 57491,    // damage players
    SPELL_FLAME_TSUNAMI_BUFF                    = 60430,    // buff Lava Blazes

    NPC_FLAME_TSUNAMI                           = 30616,    // for the flame waves
    NPC_LAVA_BLAZE                              = 30643,    // adds spawning from flame strike


    //using these custom points for dragons start and end
    POINT_ID_INIT                               = 100,
    POINT_ID_LAND                               = 200
};

struct Waypoint
{
    float m_fX, m_fY, m_fZ;
};

//each dragons special points. First where fly to before connect to connon, second where land point is.
Waypoint m_aTene[]=
{
    {3212.854f, 575.597f, 109.856f}, // init
    {3246.425f, 565.367f, 61.249f}   // end
};

Waypoint m_aShad[]=
{
    {3293.238f, 472.223f, 106.968f},
    {3227.268f, 533.238f, 59.995f}
};

Waypoint m_aVesp[]=
{
    {3193.310f, 472.861f, 102.697f},
    {3271.669f, 526.907f, 61.931f}
};

//points around raid "isle", counter clockwise. should probably be adjusted to be more alike
Waypoint m_aDragonCommon[]=
{
    {3214.012f, 468.932f, 90.652f},
    {3244.950f, 468.427f, 90.652f},
    {3283.520f, 496.869f, 90.652f},
    {3287.316f, 555.875f, 90.652f},
    {3250.479f, 585.827f, 90.652f},
    {3209.969f, 566.523f, 90.652f}
};

float m_afTsunamiStartLoc[5][4]=
{
    //left to right
    {3201.0f, 481.82f, 58.6f, 6.23f}, // covers 470.82 - 481.82 - 492.82
    {3201.0f, 524.50f, 58.6f, 6.23f}, // covers 513.50 - 524.50 - 535.50
    {3201.0f, 566.64f, 58.6f, 6.23f}, // covers 555.64 - 566.64 - 577.64
    //right to left
    {3287.5f, 545.50f, 58.6f, 3.19f}, // covers 534.50 - 545.50 - 556.50
    {3287.5f, 503.00f, 58.6f, 3.19f}, // covers 492.00 - 503.00 - 514.00
};

/*######
## Boss Sartharion
######*/

struct MANGOS_DLL_DECL boss_sartharionAI : public ScriptedAI
{
    boss_sartharionAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_obsidian_sanctum*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_bTenebronHelpedInFight = false;
        m_bShadronHelpedInFight = false;
        m_bVesperonHelpedInFight = false;
        Reset();
    }

    instance_obsidian_sanctum* m_pInstance;
    bool m_bIsRegularMode;

    bool m_bIsBerserk;
    bool m_bIsSoftEnraged;

    uint32 m_uiEnrageTimer;
    bool m_bIsHardEnraged;

    uint32 m_uiTenebronTimer;
    uint32 m_uiShadronTimer;
    uint32 m_uiVesperonTimer;

    uint32 m_uiFlameTsunamiTimer;
    uint32 m_uiFlameBreathTimer;
    uint32 m_uiTailSweepTimer;
    uint32 m_uiCleaveTimer;
    uint32 m_uiCycloneAuraTimer;

    bool m_bHasCalledTenebron;
    bool m_bHasCalledShadron;
    bool m_bHasCalledVesperon;
    bool m_bTenebronHelpedInFight;
    bool m_bShadronHelpedInFight;
    bool m_bVesperonHelpedInFight;

    uint32 m_uiCheckTwilightTimer;

    void Reset()
    {
        m_bIsBerserk        = false;
        m_bIsSoftEnraged    = false;

        m_uiEnrageTimer     = 15*MINUTE*IN_MILLISECONDS;
        m_bIsHardEnraged    = false;

        m_uiTenebronTimer   = 30000;
        m_uiShadronTimer    = 75000;
        m_uiVesperonTimer   = 120000;

        m_uiFlameTsunamiTimer   = 30000;
        m_uiFlameBreathTimer    = 20000;
        m_uiTailSweepTimer      = 5000;
        m_uiCleaveTimer         = 7000;
        m_uiCycloneAuraTimer    = 10000;

        m_bHasCalledTenebron    = false;
        m_bHasCalledShadron     = false;
        m_bHasCalledVesperon    = false;

        m_uiCheckTwilightTimer  = 2000;

        if (m_pInstance)
        {
            Creature* pTene = m_pInstance->GetSingleCreatureFromStorage(NPC_TENEBRON);
            Creature* pShad = m_pInstance->GetSingleCreatureFromStorage(NPC_SHADRON);
            Creature* pVesp = m_pInstance->GetSingleCreatureFromStorage(NPC_VESPERON);

            if (m_bTenebronHelpedInFight && pTene)
            {
                if (pTene->isDead())
                    pTene->Respawn();
                else
                    pTene->AI()->EnterEvadeMode();
            }

            if (m_bShadronHelpedInFight && pShad)
            {
                if (pShad->isDead())
                    pShad->Respawn();
                else
                    pShad->AI()->EnterEvadeMode();
            }

            if (m_bVesperonHelpedInFight && pVesp)
            {
                if (pVesp->isDead())
                    pVesp->Respawn();
                else
                    pVesp->AI()->EnterEvadeMode();
            }
        }

        m_bTenebronHelpedInFight = false;
        m_bShadronHelpedInFight = false;
        m_bVesperonHelpedInFight = false;

        m_creature->RemoveAurasDueToSpell(SPELL_TWILIGHT_REVENGE);
    }

    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, NOT_STARTED);
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_SARTHARION_AGGRO,m_creature);

        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, IN_PROGRESS);
            FetchDragons();
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_SARTHARION_DEATH, m_creature);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_SARTHARION_EVENT, DONE);
    }

    void KilledUnit(Unit* pVictim)
    {
        switch (urand(0, 2))
        {
            case 0: DoScriptText(SAY_SARTHARION_SLAY_1, m_creature); break;
            case 1: DoScriptText(SAY_SARTHARION_SLAY_2, m_creature); break;
            case 2: DoScriptText(SAY_SARTHARION_SLAY_3, m_creature); break;
        }
    }

    void FetchDragons()
    {
        Creature* pTene = m_pInstance->GetSingleCreatureFromStorage(NPC_TENEBRON);
        Creature* pShad = m_pInstance->GetSingleCreatureFromStorage(NPC_SHADRON);
        Creature* pVesp = m_pInstance->GetSingleCreatureFromStorage(NPC_VESPERON);

        //if at least one of the dragons are alive and are being called
        uint8 uiCountFetchableDragons = 0;

        if (pTene && pTene->isAlive() && !pTene->getVictim())
        {
            ++uiCountFetchableDragons;
            pTene->CastSpell(pTene, SPELL_POWER_OF_TENEBRON, false);
            pTene->SetLevitate(true);
            pTene->SetWalk(false);
            pTene->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aTene[0].m_fX, m_aTene[0].m_fY, m_aTene[0].m_fZ);

            if (!pTene->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pTene->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (pShad && pShad->isAlive() && !pShad->getVictim())
        {
            ++uiCountFetchableDragons;
            pShad->CastSpell(pShad, SPELL_POWER_OF_SHADRON, false);
            pShad->SetLevitate(true);
            pShad->SetWalk(false);
            pShad->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aShad[0].m_fX, m_aShad[0].m_fY, m_aShad[0].m_fZ);

            if (!pShad->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pShad->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (pVesp && pVesp->isAlive() && !pVesp->getVictim())
        {
            ++uiCountFetchableDragons;
            pVesp->CastSpell(pVesp, SPELL_POWER_OF_VESPERON, false);
            pVesp->SetLevitate(true);
            pVesp->SetWalk(false);
            pVesp->GetMotionMaster()->MovePoint(POINT_ID_INIT, m_aVesp[0].m_fX, m_aVesp[0].m_fY, m_aVesp[0].m_fZ);

            if (!pVesp->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                pVesp->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        }

        if (uiCountFetchableDragons)
            DoCastSpellIfCan(m_creature, SPELL_WILL_OF_SARTHARION, CAST_TRIGGERED);

        m_pInstance->SetData(TYPE_ALIVE_DRAGONS, uiCountFetchableDragons);
    }

    void CallDragon(uint32 uiEntry)
    {
        if (m_pInstance)
        {
            Creature* pTemp = m_pInstance->GetSingleCreatureFromStorage(uiEntry);

            if (pTemp && pTemp->isAlive() && !pTemp->getVictim())
            {
                pTemp->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

                int32 iTextId = 0;

                switch(uiEntry)
                {
                    case NPC_TENEBRON:
                        iTextId = SAY_SARTHARION_CALL_TENEBRON;
                        pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aTene[1].m_fX, m_aTene[1].m_fY, m_aTene[1].m_fZ);
                        m_bTenebronHelpedInFight = true;
                        break;
                    case NPC_SHADRON:
                        iTextId = SAY_SARTHARION_CALL_SHADRON;
                        pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aShad[1].m_fX, m_aShad[1].m_fY, m_aShad[1].m_fZ);
                        m_bShadronHelpedInFight = true;
                        break;
                    case NPC_VESPERON:
                        iTextId = SAY_SARTHARION_CALL_VESPERON;
                        pTemp->GetMotionMaster()->MovePoint(POINT_ID_LAND, m_aVesp[1].m_fX, m_aVesp[1].m_fY, m_aVesp[1].m_fZ);
                        m_bVesperonHelpedInFight = true;
                        break;
                }
                DoScriptText(iTextId, m_creature);
            }
        }
    }

    void SendFlameTsunami()
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive())
                        DoScriptText(WHISPER_LAVA_CHURN, m_creature, i->getSource());
                }
            }
        }

        uint8 uiTsunamiDirection = urand(0, 1);
        uint8 uiTsunamiWavesAmount = 3;
        if (uiTsunamiDirection)
            uiTsunamiWavesAmount = 2;
        for (uint8 i = uiTsunamiDirection*3; i < uiTsunamiDirection*3+uiTsunamiWavesAmount; ++i)
        {
            m_creature->SummonCreature(NPC_FLAME_TSUNAMI, m_afTsunamiStartLoc[i][0], m_afTsunamiStartLoc[i][1], m_afTsunamiStartLoc[i][2], m_afTsunamiStartLoc[i][3], TEMPSUMMON_TIMED_DESPAWN, 18000);
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //spell will target dragons, if they are still alive at 35%
        if (!m_bIsBerserk && m_creature->GetHealthPercent() <= 35.0f)
        {
            DoScriptText(SAY_SARTHARION_BERSERK, m_creature);
            DoCast(m_creature, SPELL_BERSERK);
            m_bIsBerserk = true;
        }

        //soft enrage
        if (!m_bIsSoftEnraged && m_creature->GetHealthPercent() <= 10.0f)
        {
            if (m_pInstance)
            {
                for (GUIDList::iterator iter = m_pInstance->m_lFireCyclones.begin(); iter != m_pInstance->m_lFireCyclones.end(); ++iter)
                {
                    if (Creature* pFireCyclone = m_creature->GetMap()->GetCreature(*iter))
                    {
                        pFireCyclone->CastSpell(pFireCyclone, SPELL_CYCLONE_AURA_2, true);
                    }
                }
            }
            m_bIsSoftEnraged = true;
        }

        // hard enrage
        if (!m_bIsHardEnraged)
        {
            if (m_uiEnrageTimer < uiDiff)
            {
                DoCastSpellIfCan(m_creature, SPELL_PYROBUFFET, CAST_TRIGGERED);
                m_bIsHardEnraged = true;
            }
            else
                m_uiEnrageTimer -= uiDiff;
        }

        // flame tsunami
        if (m_uiFlameTsunamiTimer < uiDiff)
        {
            SendFlameTsunami();
            switch(urand(0, 3))
            {
               case 0: DoScriptText(SAY_SARTHARION_SPECIAL_1, m_creature); break;
               case 1: DoScriptText(SAY_SARTHARION_SPECIAL_2, m_creature); break;
               case 2: DoScriptText(SAY_SARTHARION_SPECIAL_3, m_creature); break;
               case 3: DoScriptText(SAY_SARTHARION_SPECIAL_4, m_creature); break;
            }

            m_uiFlameTsunamiTimer = 30000;
        }
        else
            m_uiFlameTsunamiTimer -= uiDiff;

        // flame breath
        if (m_uiFlameBreathTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_FLAME_BREATH : SPELL_FLAME_BREATH_H) == CAST_OK)
            {
                DoScriptText(SAY_SARTHARION_BREATH, m_creature);
                m_uiFlameBreathTimer = urand(25000, 35000);
            }
        }
        else
            m_uiFlameBreathTimer -= uiDiff;

        // Tail Sweep
        if (m_uiTailSweepTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_TAIL_LASH : SPELL_TAIL_LASH_H) == CAST_OK)
                m_uiTailSweepTimer = urand(5000, 7000);
        }
        else
            m_uiTailSweepTimer -= uiDiff;

        // Cleave
        if (m_uiCleaveTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_CLEAVE) == CAST_OK)
                m_uiCleaveTimer = urand(7000, 10000);
        }
        else
            m_uiCleaveTimer -= uiDiff;

        // Lavas Strike
        if (m_uiCycloneAuraTimer < uiDiff)
        {
            if (m_pInstance)
            {
                GUIDList::iterator iter = m_pInstance->m_lFireCyclones.begin();
                advance(iter, urand(0, m_pInstance->m_lFireCyclones.size()-1));
                if (Creature* pFireCyclone = m_creature->GetMap()->GetCreature(*iter))
                {
                    pFireCyclone->CastSpell(pFireCyclone, SPELL_CYCLONE_AURA_2, true);

                    switch(urand(0, 10))
                    {
                        case 0: DoScriptText(SAY_SARTHARION_SPECIAL_1, m_creature); break;
                        case 1: DoScriptText(SAY_SARTHARION_SPECIAL_2, m_creature); break;
                        case 2: DoScriptText(SAY_SARTHARION_SPECIAL_3, m_creature); break;
                    }
                    if (m_bIsSoftEnraged)
                        m_uiCycloneAuraTimer = 10000;
                    else
                        m_uiCycloneAuraTimer = urand(20000, 25000);
                }
            }
        }
        else
            m_uiCycloneAuraTimer -= uiDiff;

        // call tenebron
        if (!m_bHasCalledTenebron && m_uiTenebronTimer < uiDiff)
        {
            CallDragon(NPC_TENEBRON);
            m_bHasCalledTenebron = true;
        }
        else
            m_uiTenebronTimer -= uiDiff;

        // call shadron
        if (!m_bHasCalledShadron && m_uiShadronTimer < uiDiff)
        {
            CallDragon(NPC_SHADRON);
            m_bHasCalledShadron = true;
        }
        else
            m_uiShadronTimer -= uiDiff;

        // call vesperon
        if (!m_bHasCalledVesperon && m_uiVesperonTimer < uiDiff)
        {
            CallDragon(NPC_VESPERON);
            m_bHasCalledVesperon = true;
        }
        else
            m_uiVesperonTimer -= uiDiff;

        if (m_uiCheckTwilightTimer < uiDiff)
        {
            bool bNoAliveTwilightRealm = true;
            if (m_pInstance)
            {
                if (Creature* pAcolyte = m_pInstance->GetSingleCreatureFromStorage(NPC_ACOLYTE_OF_SHADRON))
                {
                    if (pAcolyte->isAlive())
                    {
                        bNoAliveTwilightRealm = false;
                    }
                }
                if (Creature* pAcolyte = m_pInstance->GetSingleCreatureFromStorage(NPC_ACOLYTE_OF_VESPERON))
                {
                    if (pAcolyte->isAlive())
                    {
                        bNoAliveTwilightRealm = false;
                    }
                }
                
                for (GUIDList::iterator itr = m_pInstance->m_lEggsGUIDList.begin(); itr != m_pInstance->m_lEggsGUIDList.end(); ++itr)
                {
                    if (Creature* pTemp = m_creature->GetMap()->GetCreature(*itr))
                    {
                        if (pTemp->isAlive())
                        {
                            bNoAliveTwilightRealm = false;
                            break;
                        }
                    }
                }                
                if (bNoAliveTwilightRealm)
                {
                    Map* pMap = m_creature->GetMap();

                    if (pMap && pMap->IsDungeon())
                    {
                        Map::PlayerList const &PlayerList = pMap->GetPlayers();

                        if (!PlayerList.isEmpty())
                            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                            {
                                if (i->getSource()->isAlive() && i->getSource()->GetPhaseMask() != 1)
                                    i->getSource()->CastSpell(i->getSource(), SPELL_TWILIGHT_SHIFT_REMOVAL, true);
                            }
                    }
                }
            }
            m_uiCheckTwilightTimer = 2000;
        }
        else
            m_uiCheckTwilightTimer -= uiDiff;

        DoMeleeAttackIfReady();

        EnterEvadeIfOutOfCombatArea(uiDiff);
    }
};

CreatureAI* GetAI_boss_sartharion(Creature* pCreature)
{
    return new boss_sartharionAI(pCreature);
}

enum TeneText
{
    SAY_TENEBRON_AGGRO                      = -1615010,
    SAY_TENEBRON_SLAY_1                     = -1615011,
    SAY_TENEBRON_SLAY_2                     = -1615012,
    SAY_TENEBRON_DEATH                      = -1615013,
    SAY_TENEBRON_BREATH                     = -1615014,
    SAY_TENEBRON_RESPOND                    = -1615015,
    SAY_TENEBRON_SPECIAL_1                  = -1615016,
    SAY_TENEBRON_SPECIAL_2                  = -1615017
};

enum ShadText
{
    SAY_SHADRON_AGGRO                       = -1615000,
    SAY_SHADRON_SLAY_1                      = -1615001,
    SAY_SHADRON_SLAY_2                      = -1615002,
    SAY_SHADRON_DEATH                       = -1615003,
    SAY_SHADRON_BREATH                      = -1615004,
    SAY_SHADRON_RESPOND                     = -1615005,
    SAY_SHADRON_SPECIAL_1                   = -1615006,
    SAY_SHADRON_SPECIAL_2                   = -1615007
};

enum VespText
{
    SAY_VESPERON_AGGRO                      = -1615040,
    SAY_VESPERON_SLAY_1                     = -1615041,
    SAY_VESPERON_SLAY_2                     = -1615042,
    SAY_VESPERON_DEATH                      = -1615043,
    SAY_VESPERON_BREATH                     = -1615044,
    SAY_VESPERON_RESPOND                    = -1615045,
    SAY_VESPERON_SPECIAL_1                  = -1615046,
    SAY_VESPERON_SPECIAL_2                  = -1615047
};


float m_aPortalLoc[4][4] =
{
    {3238.37f, 518.595f, 58.9057f, 0.739184f}, //Sartharion
    {3239.05f, 677.645f, 89.5893f, 4.72206f }, //Tenebron
    {3354.34f, 516.81f , 99.0972f, 2.64546f }, //Shadron
    {3136.69f, 510.132f, 89.1018f, 1.42888f }, //Vesperon
};
//to control each dragons common abilities
struct MANGOS_DLL_DECL dummy_dragonAI : public ScriptedAI
{
    dummy_dragonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_obsidian_sanctum*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    instance_obsidian_sanctum* m_pInstance;
    bool m_bIsRegularMode;

    uint32 m_uiWaypointId;
    uint32 m_uiMoveNextTimer;
    uint32 m_uiPortalTimer;
    bool m_bCanMoveFree;

    GUIDList m_lPortalGUIDList;

    void Reset()
    {
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        
        m_uiWaypointId = 0;
        m_uiMoveNextTimer = 500;
        m_uiPortalTimer = 0;
        m_bCanMoveFree = false;
        ClosePortal();
    }

    void AttackStart(Unit* pWho)
    {
        if (!pWho || m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == POINT_MOTION_TYPE)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);

            if (IsCombatMovement())
                m_creature->GetMotionMaster()->MoveChase(pWho);
        }
    }

    void JustReachedHome()
    {
        m_creature->SetLevitate(false);
    }

    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if (!m_pInstance || uiType != POINT_MOTION_TYPE)
            return;

        debug_log("dummy_dragonAI: %s reached point %u", m_creature->GetName(), uiPointId);

        //if healers messed up the raid and we was already initialized
        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
        {
            EnterEvadeMode();
            return;
        }

        //this is the end (!)
        if (uiPointId == POINT_ID_LAND)
        {
            m_creature->GetMotionMaster()->Clear();
            m_bCanMoveFree = false;
            m_creature->SetLevitate(false);
            m_creature->SetInCombatWithZone();
            return;
        }

        //get amount of common points
        uint32 uiCommonWPCount = sizeof(m_aDragonCommon)/sizeof(Waypoint);

        //increase
        m_uiWaypointId = uiPointId+1;

        //if we have reached a point bigger or equal to count, it mean we must reset to point 0
        if (m_uiWaypointId >= uiCommonWPCount)
        {
            if (!m_bCanMoveFree)
                m_bCanMoveFree = true;

            m_uiWaypointId = 0;
        }

        m_uiMoveNextTimer = 500;
    }

    //used when open portal and spawn mobs in phase
    void DoRaidWhisper(int32 iTextId)
    {
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
            {
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    DoScriptText(iTextId, m_creature, i->getSource());
            }
        }
    }

    void ExitTwilightRealm()
    {
        Map* pMap = m_creature->GetMap();
        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (!PlayerList.isEmpty())
                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                {
                    if (i->getSource()->isAlive() && i->getSource()->GetPhaseMask() != 1)
                        i->getSource()->CastSpell(i->getSource(), SPELL_TWILIGHT_SHIFT_REMOVAL, true);
                }
        }
    }

    void ClosePortal()
    {
        if (Map* pMap = m_creature->GetMap())
        {
            if (!m_lPortalGUIDList.empty())
            {
                for(GUIDList::iterator itr = m_lPortalGUIDList.begin(); itr != m_lPortalGUIDList.end(); ++itr)
                    if (GameObject* pTemp = pMap->GetGameObject(*itr))
                        pTemp->Delete();
            }
            m_lPortalGUIDList.clear();
        }
    }

    GameObject* SummonPortal(uint32 uiData)
    {
        GameObject* pPortal = NULL;

        switch(uiData)
        {
            case NPC_SARTHARION:
                if (!m_lPortalGUIDList.empty())
                {
                    if (Map* pMap = m_creature->GetMap())
                    {
                        GUIDList::iterator itr = m_lPortalGUIDList.begin();
                        pPortal = pMap->GetGameObject(*itr);
                    }
                }
                else
                {
                    pPortal = m_creature->SummonGameobject(193988,m_aPortalLoc[0][0],m_aPortalLoc[0][1],m_aPortalLoc[0][2],m_aPortalLoc[0][3], 100);
                    m_lPortalGUIDList.push_back(pPortal->GetObjectGuid());
                }
                break;
            case NPC_TENEBRON:
                pPortal = m_creature->SummonGameobject(193988,m_aPortalLoc[1][0],m_aPortalLoc[1][1],m_aPortalLoc[1][2],m_aPortalLoc[1][3], 100);
                m_lPortalGUIDList.push_back(pPortal->GetObjectGuid());
                break;
            case NPC_SHADRON:
                pPortal = m_creature->SummonGameobject(193988,m_aPortalLoc[2][0],m_aPortalLoc[2][1],m_aPortalLoc[2][2],m_aPortalLoc[2][3], 100);
                m_lPortalGUIDList.push_back(pPortal->GetObjectGuid());
                break;
            case NPC_VESPERON:
                pPortal = m_creature->SummonGameobject(193988,m_aPortalLoc[3][0],m_aPortalLoc[3][1],m_aPortalLoc[3][2],m_aPortalLoc[3][3], 100);
                m_lPortalGUIDList.push_back(pPortal->GetObjectGuid());
                break;
        }
        return pPortal;
    }


    //"opens" the portal and does the "opening" whisper
    void OpenPortal()
    {
        int32 iTextId = 0;
        GameObject* pPortal = NULL;
        m_uiPortalTimer = 30000;
        switch(m_creature->GetEntry())
        {
            case NPC_TENEBRON:
                if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS ? pPortal = SummonPortal(NPC_SARTHARION) : pPortal = SummonPortal(NPC_TENEBRON))
                {
                    if (pPortal)
                    {
                        iTextId = WHISPER_HATCH_EGGS;
                        m_pInstance->m_lEggsGUIDList.clear();
                        for (uint8 i=0; i<6; ++i)
                        {
                            m_creature->SummonCreature(NPC_TWILIGHT_EGG, pPortal->GetPositionX()-10+urand(0, 20), pPortal->GetPositionY()-10+urand(0, 20), pPortal->GetPositionZ()+1.0f, 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000);
                        }
                    }
                }
                break;
            case NPC_SHADRON:
                if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS ? pPortal = SummonPortal(NPC_SARTHARION) : pPortal = SummonPortal(NPC_SHADRON))
                {
                    if (pPortal)
                    {
                        iTextId = WHISPER_OPEN_PORTAL;
                        if (m_pInstance)
                        {
                            Creature* pAcolyte = m_pInstance->GetSingleCreatureFromStorage(NPC_ACOLYTE_OF_SHADRON);
                            if (!pAcolyte || (pAcolyte && pAcolyte->isDead()))
                            {
                                if (Creature* pAcolyte2 = m_creature->SummonCreature(NPC_ACOLYTE_OF_SHADRON, pPortal->GetPositionX()-10+urand(0, 20), pPortal->GetPositionY()-10+urand(0, 20), pPortal->GetPositionZ()+1.0f, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 90000))
                                {
                                    if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS) 
                                    {
                                        if (Creature* pSarth = m_pInstance->GetSingleCreatureFromStorage(NPC_SARTHARION))
                                            pSarth->CastSpell(pSarth, SPELL_GIFT_OF_TWILIGTH_SAR, true);
                                    }
                                    else
                                    {
                                        if (Creature* pShad = m_pInstance->GetSingleCreatureFromStorage(NPC_SHADRON))
                                            pShad->CastSpell(pShad, SPELL_GIFT_OF_TWILIGTH_SHA, true);
                                    }
                                }
                            }
                        }
                    }
                }
                break;
            case NPC_VESPERON:
                if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS ? pPortal = SummonPortal(NPC_SARTHARION) : pPortal = SummonPortal(NPC_VESPERON))
                {
                    if (pPortal)
                    {
                        iTextId = WHISPER_OPEN_PORTAL;
                        if (m_pInstance)
                        {
                            uint32 uiTempSpell;
                            if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) == IN_PROGRESS)
                                uiTempSpell = 58835;
                            else
                                uiTempSpell = 57935;

                            SpellEntry* pTempSpell = (SpellEntry*)GetSpellStore()->LookupEntry(uiTempSpell);
                            if (pTempSpell)
                            {
                                pTempSpell->StackAmount = 1;
                                pTempSpell->procCharges = 0;
                                m_creature->CastSpell(m_creature, pTempSpell, true);
                            }

                            Creature* pAcolyte = m_pInstance->GetSingleCreatureFromStorage(NPC_ACOLYTE_OF_VESPERON);
                            if (!pAcolyte || (pAcolyte && pAcolyte->isDead()))
                            {
                                pAcolyte = m_creature->SummonCreature(NPC_ACOLYTE_OF_VESPERON, pPortal->GetPositionX()-10+urand(0, 20), pPortal->GetPositionY()-10+urand(0, 20), pPortal->GetPositionZ()+1.0f, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 90000);
                            }
                        }
                    }
                }
                break;
        }
        DoRaidWhisper(iTextId);
    }

    void CheckTwilightRealm()
    {
        bool bNoAliveTwilightRealm = true;
        if (m_pInstance)
        {
            if (Creature* pAcolyte = m_pInstance->GetSingleCreatureFromStorage(NPC_ACOLYTE_OF_SHADRON))
                if (pAcolyte->isAlive())
                    bNoAliveTwilightRealm = false;
            if (Creature* pAcolyte = m_pInstance->GetSingleCreatureFromStorage(NPC_ACOLYTE_OF_VESPERON))
                if (pAcolyte->isAlive())
                    bNoAliveTwilightRealm = false;
            if (!m_pInstance->m_lEggsGUIDList.empty())
            {
                for (GUIDList::iterator itr = m_pInstance->m_lEggsGUIDList.begin(); itr != m_pInstance->m_lEggsGUIDList.end(); ++itr)
                {
                    if (Creature* pTemp = m_creature->GetMap()->GetCreature( *itr))
                        if (pTemp->isAlive())
                        {
                            bNoAliveTwilightRealm = false;
                            break;
                        }
                }
            }
            if (bNoAliveTwilightRealm)
                ExitTwilightRealm();
        }
    }

    void JustDied(Unit* pKiller)
    {
        ClosePortal();
        int32 iTextId = 0;

        switch(m_creature->GetEntry())
        {
            case NPC_TENEBRON:
            {
                iTextId = SAY_TENEBRON_DEATH;

                if (!m_pInstance->m_lEggsGUIDList.empty())
                    for (GUIDList::iterator itr = m_pInstance->m_lEggsGUIDList.begin(); itr != m_pInstance->m_lEggsGUIDList.end(); ++itr)
                        if (Creature* pEgg = m_creature->GetMap()->GetCreature( *itr))
                            pEgg->ForcedDespawn();
                break;
            }
            case NPC_SHADRON:
            {
                iTextId = SAY_SHADRON_DEATH;
                if (Creature* pAcolyte = m_pInstance->GetSingleCreatureFromStorage(NPC_ACOLYTE_OF_SHADRON))
                    pAcolyte->DealDamage(pAcolyte, pAcolyte->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                //not solo fight, so main boss has deduff
                if (Creature* pSartharion = m_pInstance->GetSingleCreatureFromStorage(NPC_SARTHARION))
                {
                    if (pSartharion->isAlive() && pSartharion->HasAura(SPELL_GIFT_OF_TWILIGTH_SAR))
                        pSartharion->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SAR);
                }
                //event not in progress, then solo fight and must remove debuff mini-boss
                if (Creature* pShadron = m_pInstance->GetSingleCreatureFromStorage(NPC_SHADRON))
                {
                    if (pShadron->isAlive() && pShadron->HasAura(SPELL_GIFT_OF_TWILIGTH_SHA))
                        pShadron->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SHA);
                }
                break;
            }
            case NPC_VESPERON:
            {
                iTextId = SAY_VESPERON_DEATH;
                if (Creature* pAcolyte = m_pInstance->GetSingleCreatureFromStorage(NPC_ACOLYTE_OF_VESPERON))
                    pAcolyte->DealDamage(pAcolyte, pAcolyte->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                // remove twilight torment
                Map* pMap = m_creature->GetMap();

                if (pMap && pMap->IsDungeon())
                {
                    Map::PlayerList const &PlayerList = pMap->GetPlayers();

                    if (PlayerList.isEmpty())
                        return;

                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    {
                        i->getSource()->RemoveAurasDueToSpell(57935);
                        i->getSource()->RemoveAurasDueToSpell(58835);
                    }
                }
                break;
            }
        }

        DoScriptText(iTextId, m_creature);

        if (m_pInstance)
        {
            // not if solo mini-boss fight
            if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
            {
                ExitTwilightRealm();
                return;
            }

            // Twilight Revenge to main boss
            if (Creature* pSartharion = m_pInstance->GetSingleCreatureFromStorage(NPC_SARTHARION))
            {
                if (pSartharion->isAlive())
                    pSartharion->CastSpell(pSartharion, SPELL_TWILIGHT_REVENGE, true);
                m_creature->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bCanMoveFree && m_uiMoveNextTimer)
        {
            if (m_uiMoveNextTimer <= uiDiff)
            {
                m_creature->GetMotionMaster()->MovePoint(m_uiWaypointId,
                    m_aDragonCommon[m_uiWaypointId].m_fX, m_aDragonCommon[m_uiWaypointId].m_fY, m_aDragonCommon[m_uiWaypointId].m_fZ);

                debug_log("dummy_dragonAI: %s moving to point %u", m_creature->GetName(), m_uiWaypointId);
                m_uiMoveNextTimer = 0;
            }
            else
                m_uiMoveNextTimer -= uiDiff;
        }
    }
};

/*######
## Mob Tenebron
######*/

struct MANGOS_DLL_DECL mob_tenebronAI : public dummy_dragonAI
{
    mob_tenebronAI(Creature* pCreature) : dummy_dragonAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiShadowBreathTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiHatchEggTimer;
    uint32 m_uiCheckTimer;
    uint32 m_uiTailSweepTimer;

    void Reset()
    {
        dummy_dragonAI::Reset();
        m_uiShadowBreathTimer = 10000;
        m_uiShadowFissureTimer = 8000;
        m_uiHatchEggTimer = 15000;
        m_uiCheckTimer = 2000;
        m_uiTailSweepTimer = 5000;
        m_pInstance->m_lEggsGUIDList.clear();
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_TENEBRON_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_TENEBRON_SLAY_1 : SAY_TENEBRON_SLAY_2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        // shadow fissure
        if (m_uiShadowFissureTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, 0.0f, SELECT_FLAG_PLAYER))
                if(DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_SHADOW_FISSURE : SPELL_SHADOW_FISSURE_H) == CAST_OK)
                    m_uiShadowFissureTimer = urand(8000, 10000);
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        // shadow breath
        if (m_uiShadowBreathTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BREATH : SPELL_SHADOW_BREATH_H) == CAST_OK)
            {
                DoScriptText(SAY_TENEBRON_BREATH, m_creature);
                m_uiShadowBreathTimer = urand(10000, 30000);
            }
        }
        else
            m_uiShadowBreathTimer -= uiDiff;

        if (m_uiHatchEggTimer < uiDiff)
        {
            OpenPortal();
            m_uiHatchEggTimer = 45000;
        }
        else
            m_uiHatchEggTimer -= uiDiff;

        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
        {
            if (m_uiPortalTimer < uiDiff)
                ClosePortal();
            else
                m_uiPortalTimer -= uiDiff;

            if (m_uiCheckTimer < uiDiff)
            {
                CheckTwilightRealm();
                m_uiCheckTimer = 2000;
            }
            else
                m_uiCheckTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_tenebron(Creature* pCreature)
{
    return new mob_tenebronAI(pCreature);
}

/*######
## Mob Shadron
######*/

struct MANGOS_DLL_DECL mob_shadronAI : public dummy_dragonAI
{
    mob_shadronAI(Creature* pCreature) : dummy_dragonAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiShadowBreathTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiAcolyteShadronTimer;
    uint32 m_uiCheckTimer;
    uint32 m_uiTailSweepTimer;

    void Reset()
    {
        dummy_dragonAI::Reset();
        m_uiShadowBreathTimer = 10000;
        m_uiShadowFissureTimer = 8000;
        m_uiAcolyteShadronTimer = 15000;
        m_uiCheckTimer = 2000;
        m_uiTailSweepTimer = 5000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_SHADRON_AGGRO,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_SHADRON_SLAY_1 : SAY_SHADRON_SLAY_2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        // shadow fissure
        if (m_uiShadowFissureTimer < uiDiff)
        {            
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, 0.0f, SELECT_FLAG_PLAYER))
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_SHADOW_FISSURE : SPELL_SHADOW_FISSURE_H) == CAST_OK)
                    m_uiShadowFissureTimer = urand(8000, 10000);
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        // shadow breath
        if (m_uiShadowBreathTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BREATH : SPELL_SHADOW_BREATH_H) == CAST_OK)
            {
                DoScriptText(SAY_SHADRON_BREATH, m_creature);            
                m_uiShadowBreathTimer = urand(10000, 30000);
            }
        }
        else
            m_uiShadowBreathTimer -= uiDiff;

        if (m_uiAcolyteShadronTimer < uiDiff)
        {
            OpenPortal();
            m_uiAcolyteShadronTimer = 60000;
        }
        else
            m_uiAcolyteShadronTimer -= uiDiff;
        
        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
        {       
            if (m_uiPortalTimer < uiDiff)
                ClosePortal();
            else
                m_uiPortalTimer -= uiDiff;

            if (m_uiCheckTimer < uiDiff)
            {
                CheckTwilightRealm();
                m_uiCheckTimer = 2000;
            }
            else
                m_uiCheckTimer -= uiDiff;
        }       

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_shadron(Creature* pCreature)
{
    return new mob_shadronAI(pCreature);
}

/*######
## Mob Vesperon
######*/

struct MANGOS_DLL_DECL mob_vesperonAI : public dummy_dragonAI
{
    mob_vesperonAI(Creature* pCreature) : dummy_dragonAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiShadowBreathTimer;
    uint32 m_uiShadowFissureTimer;
    uint32 m_uiAcolyteVesperonTimer;
    uint32 m_uiCheckTimer;
    uint32 m_uiTailSweepTimer;

    void Reset()
    {
        dummy_dragonAI::Reset();
        m_uiShadowBreathTimer = 10000;
        m_uiShadowFissureTimer = 8000;
        m_uiAcolyteVesperonTimer = 15000;
        m_uiCheckTimer = 2000;
        m_uiTailSweepTimer = 5000;
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_VESPERON_AGGRO,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(urand(0, 1) ? SAY_VESPERON_SLAY_1 : SAY_VESPERON_SLAY_2, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //if no target, update dummy and return
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            dummy_dragonAI::UpdateAI(uiDiff);
            return;
        }

        // shadow fissure
        if (m_uiShadowFissureTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1, 0.0f, SELECT_FLAG_PLAYER))
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_SHADOW_FISSURE : SPELL_SHADOW_FISSURE_H) == CAST_OK)
                    m_uiShadowFissureTimer = urand(8000, 10000);
        }
        else
            m_uiShadowFissureTimer -= uiDiff;

        // shadow breath
        if (m_uiShadowBreathTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_SHADOW_BREATH : SPELL_SHADOW_BREATH_H) == CAST_OK)
            {
                DoScriptText(SAY_VESPERON_BREATH, m_creature);
                m_uiShadowBreathTimer = urand(10000, 30000);
            }
        }
        else
            m_uiShadowBreathTimer -= uiDiff;

        if (m_uiAcolyteVesperonTimer < uiDiff)
        {
            OpenPortal();
            m_uiAcolyteVesperonTimer = 60000;
        }
        else
            m_uiAcolyteVesperonTimer -= uiDiff;

        if (m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS)
        {
            if (m_uiPortalTimer < uiDiff)
                ClosePortal();
            else
                m_uiPortalTimer -= uiDiff;
            
            if (m_uiCheckTimer < uiDiff)
            {
                CheckTwilightRealm();
                m_uiCheckTimer = 2000;
            }
            else
                m_uiCheckTimer -= uiDiff;
        }
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_vesperon(Creature* pCreature)
{
    return new mob_vesperonAI(pCreature);
}

/*######
## Mob Acolyte of Shadron
######*/

struct MANGOS_DLL_DECL mob_acolyte_of_shadronAI : public ScriptedAI
{
    mob_acolyte_of_shadronAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_obsidian_sanctum*)pCreature->GetInstanceData();
        Reset();
    }

    instance_obsidian_sanctum* m_pInstance;

    void Reset()
    {
    }
    void JustDied(Unit* killer)
    {
        if (m_pInstance)
        {            
            //not solo fight, so main boss has deduff
            if (Creature* pSartharion = m_pInstance->GetSingleCreatureFromStorage(NPC_SARTHARION))
            {
                if (pSartharion->isAlive() && pSartharion->HasAura(SPELL_GIFT_OF_TWILIGTH_SAR))
                    pSartharion->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SAR);
            }
            //event not in progress, then solo fight and must remove debuff mini-boss
            if (Creature* pShadron = m_pInstance->GetSingleCreatureFromStorage(NPC_SHADRON))
            {
                if (pShadron->isAlive() && pShadron->HasAura(SPELL_GIFT_OF_TWILIGTH_SHA))
                    pShadron->RemoveAurasDueToSpell(SPELL_GIFT_OF_TWILIGTH_SHA);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_acolyte_of_shadron(Creature* pCreature)
{
    return new mob_acolyte_of_shadronAI(pCreature);
}

/*######
## Mob Acolyte of Vesperon
######*/

struct MANGOS_DLL_DECL mob_acolyte_of_vesperonAI : public ScriptedAI
{
    mob_acolyte_of_vesperonAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (instance_obsidian_sanctum*)pCreature->GetInstanceData();
        Reset();
    }

    instance_obsidian_sanctum* m_pInstance;

    void Reset()
    {
    }

    void JustDied(Unit* pKiller)
    {
        // remove twilight torment
        Map* pMap = m_creature->GetMap();

        if (pMap && pMap->IsDungeon())
        {
            Map::PlayerList const &PlayerList = pMap->GetPlayers();

            if (PlayerList.isEmpty())
                return;

            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
            {
                i->getSource()->RemoveAurasDueToSpell(57935);
                i->getSource()->RemoveAurasDueToSpell(58835);
            }
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_acolyte_of_vesperon(Creature* pCreature)
{
    return new mob_acolyte_of_vesperonAI(pCreature);
}

/*######
## Mob Twilight Eggs
######*/

struct MANGOS_DLL_DECL mob_twilight_eggsAI : public ScriptedAI
{
    mob_twilight_eggsAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiSummonWhelpTimer;

    void Reset()
    {
        m_uiSummonWhelpTimer = 20000;
    }

    void AttackStart(Unit* pWho)
    {
    }

    void MoveInLineOfSight(Unit* pWho)
    {
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiSummonWhelpTimer < uiDiff)
        {
            if (Creature* pWhelp = DoSpawnCreature(NPC_TWILIGHT_WHELP, 0, 0, 0, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000))
            {
                pWhelp->SetPhaseMask(1, true);
                pWhelp->SetInCombatWithZone();
            }
            m_uiSummonWhelpTimer = 20000;
            m_creature->ForcedDespawn();
        }
        else
            m_uiSummonWhelpTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_twilight_eggs(Creature* pCreature)
{
    return new mob_twilight_eggsAI(pCreature);
}

/*######
## Mob Twilight Whelps
######*/

struct MANGOS_DLL_DECL mob_twilight_whelpAI : public ScriptedAI
{
    mob_twilight_whelpAI(Creature* pCreature) : ScriptedAI(pCreature) { Reset(); }

    uint32 m_uiFadeArmorTimer;

    void Reset()
    {
        m_uiFadeArmorTimer = 1000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        // twilight torment
        if (m_uiFadeArmorTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FADE_ARMOR) == CAST_OK)
                m_uiFadeArmorTimer = urand(5000, 10000);
        }
        else
            m_uiFadeArmorTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_twilight_whelp(Creature* pCreature)
{
    return new mob_twilight_whelpAI(pCreature);
}

/*######
## Mob Fire Cyclone
######*/

struct MANGOS_DLL_DECL mob_fire_cycloneAI : public ScriptedAI
{
    mob_fire_cycloneAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiLavaStrikeTimer;
    uint8 m_uiLavaStrikesCount;
    bool m_bLavaStrikeAllowed;


    void Reset()
    {
        m_bLavaStrikeAllowed = false;
    }

    void AttackStart(Unit* pWho)
    {
    }

    void MoveInLineOfSight(Unit* pWho)
    {
    }

    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if (pSpell->Id == SPELL_CYCLONE_AURA_2)
        {
            m_bLavaStrikeAllowed = true;
            m_uiLavaStrikeTimer = 0;
            m_uiLavaStrikesCount = 0;
        }
    }

    void JustSummoned(Creature* pSummoned)
    {
        if (pSummoned->GetEntry() == NPC_LAVA_BLAZE)
            if (urand(0, 3))                                //25% to stay
                pSummoned->ForcedDespawn();
            else
                pSummoned->SetInCombatWithZone();
    }

    void UpdateAI(const uint32 uiDiff)
    {

        if (m_bLavaStrikeAllowed)
        {
            if (m_uiLavaStrikeTimer < uiDiff)
            {
                m_creature->CastSpell(urand(3220, 3275), urand(486, 575), 58.8f, SPELL_LAVA_STRIKE, true);
                m_uiLavaStrikeTimer = 1000;
                ++m_uiLavaStrikesCount;
                if (m_uiLavaStrikesCount>=5)
                    m_bLavaStrikeAllowed = false;
            }
            else
                m_uiLavaStrikeTimer -= uiDiff;
        }
    }
};

CreatureAI* GetAI_mob_fire_cyclone(Creature* pCreature)
{
    return new mob_fire_cycloneAI(pCreature);
}

/*######
## Mob Flame Tsunami
######*/

struct MANGOS_DLL_DECL mob_flame_tsunamiAI : public ScriptedAI
{
    mob_flame_tsunamiAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        Reset();
    }

    uint32 m_uiMovementStartTimer;

    void Reset()
    {
        DoCast(m_creature, SPELL_FLAME_TSUNAMI, true);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetDisplayId(11686);
        DoCast(m_creature, SPELL_FLAME_TSUNAMI_DMG_AURA,true);
        m_creature->setFaction(19);
        m_creature->SetLevitate(true);
        m_uiMovementStartTimer = 4000;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiMovementStartTimer < uiDiff)
        {
            int8 uiDirection = 1;
            if (m_creature->GetPositionX() > 3240.0f)
                uiDirection = -1;
            m_creature->GetMotionMaster()->MovePoint(0, m_creature->GetPositionX()+uiDirection*86.5f, m_creature->GetPositionY(), m_creature->GetPositionZ());
            m_uiMovementStartTimer = 30000;
        }
        else
            m_uiMovementStartTimer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_flame_tsunami(Creature* pCreature)
{
    return new mob_flame_tsunamiAI(pCreature);
}

struct MANGOS_DLL_DECL  mob_lava_blazeAI : public ScriptedAI
{
    mob_lava_blazeAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (instance_obsidian_sanctum*)pCreature->GetInstanceData();
        Reset();
    }
    instance_obsidian_sanctum* m_pInstance;
    uint32 m_uiStartHealth;

    void Reset()
    {
        m_uiStartHealth = m_creature->GetHealth();
        m_creature->setFaction(19);
        m_creature->SetRespawnDelay(DAY);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance || m_pInstance->GetData(TYPE_SARTHARION_EVENT) != IN_PROGRESS) 
              m_creature->ForcedDespawn();
        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_lava_blaze(Creature* pCreature)
{
    return new mob_lava_blazeAI(pCreature);
};

void AddSC_boss_sartharion()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_sartharion";
    newscript->GetAI = &GetAI_boss_sartharion;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_vesperon";
    newscript->GetAI = &GetAI_mob_vesperon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_shadron";
    newscript->GetAI = &GetAI_mob_shadron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_tenebron";
    newscript->GetAI = &GetAI_mob_tenebron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_acolyte_of_shadron";
    newscript->GetAI = &GetAI_mob_acolyte_of_shadron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_acolyte_of_vesperon";
    newscript->GetAI = &GetAI_mob_acolyte_of_vesperon;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_twilight_eggs";
    newscript->GetAI = &GetAI_mob_twilight_eggs;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_twilight_whelp";
    newscript->GetAI = &GetAI_mob_twilight_whelp;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_fire_cyclone";
    newscript->GetAI = &GetAI_mob_fire_cyclone;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_flame_tsunami";
    newscript->GetAI = &GetAI_mob_flame_tsunami;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_lava_blaze";
    newscript->GetAI = &GetAI_mob_lava_blaze;
    newscript->RegisterSelf();
}
