#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Horse.h"
#include "../World.h"
#include "../Entities/Player.h"





cHorse::cHorse(int Type, int Color, int Style, int TameTimes) :
	super("Horse", mtHorse, "mob.horse.hit", "mob.horse.death", 1.4, 1.6),
	m_bHasChest(false),
	m_bIsEating(false),
	m_bIsRearing(false),
	m_bIsMouthOpen(false),
	m_bIsTame(false),
	m_bIsSaddled(false),
	m_Type(Type),
	m_Color(Color),
	m_Style(Style),
	m_Armour(0),
	m_TimesToTame(TameTimes),
	m_TameAttemptTimes(0),
	m_RearTickCount(0)
{
}





void cHorse::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (!m_bIsMouthOpen)
	{
		if (m_World->GetTickRandomNumber(50) == 25)
		{
			m_bIsMouthOpen = true;
		}
	}
	else
	{
		if (m_World->GetTickRandomNumber(10) == 5)
		{
			m_bIsMouthOpen = false;
		}
	}

	if ((m_Attachee != nullptr) && (!m_bIsTame))
	{
		if (m_TameAttemptTimes < m_TimesToTame)
		{
			if (m_World->GetTickRandomNumber(50) == 25)
			{
				m_World->BroadcastSoundParticleEffect(2000, FloorC(GetPosX()), FloorC(GetPosY()), FloorC(GetPosZ()), 0);
				m_World->BroadcastSoundParticleEffect(2000, FloorC(GetPosX()), FloorC(GetPosY()), FloorC(GetPosZ()), 2);
				m_World->BroadcastSoundParticleEffect(2000, FloorC(GetPosX()), FloorC(GetPosY()), FloorC(GetPosZ()), 6);
				m_World->BroadcastSoundParticleEffect(2000, FloorC(GetPosX()), FloorC(GetPosY()), FloorC(GetPosZ()), 8);

				m_Attachee->Detach();
				m_bIsRearing = true;
			}
		}
		else
		{
			m_bIsTame = true;
		}
	}
	
	if (m_bIsRearing)
	{
		if (m_RearTickCount == 20)
		{
			m_bIsRearing = false;
			m_RearTickCount = 0;
		}
		else
		{
			m_RearTickCount++;
		}
	}

	m_World->BroadcastEntityMetadata(*this);
}





void cHorse::OnRightClicked(cPlayer & a_Player)
{
	if (!m_bIsSaddled && m_bIsTame)
	{
		if (a_Player.GetEquippedItem().m_ItemType == E_ITEM_SADDLE)
		{
			// Saddle the horse:
			if (!a_Player.IsGameModeCreative())
			{
				a_Player.GetInventory().RemoveOneEquippedItem();
			}
			m_bIsSaddled = true;
			m_World->BroadcastEntityMetadata(*this);
		}
		else if (!a_Player.GetEquippedItem().IsEmpty())
		{
			// The horse doesn't like being hit, make it rear:
			m_bIsRearing = true;
			m_RearTickCount = 0;
		}
	}
	else
	{
		if (m_Attachee != nullptr)
		{
			if (m_Attachee->GetUniqueID() == a_Player.GetUniqueID())
			{
				a_Player.Detach();
				return;
			}

			if (m_Attachee->IsPlayer())
			{
				return;
			}

			m_Attachee->Detach();
		}

		// If the player is in creative mode and the horse is not tame or saddled, instant tame and saddle.
		if (a_Player.IsGameModeCreative() && !(m_bIsTame || m_bIsSaddled))
		{
			m_bIsRearing = false;
			m_bIsTame = true;
			m_bIsSaddled = true;
			m_World->BroadcastEntityMetadata(*this);
		}

		m_TameAttemptTimes++;
		a_Player.AttachTo(this);
	}
}





void cHorse::HandleSpeedFromAttachee(float a_Forward, float a_Sideways, bool a_IsJumping)
{
	if (!IsPlayerControlled())
	{
		return;
	}

	// TODO: Handle jumping

	super::HandleSpeedFromAttachee(a_Forward * 10.0f, a_Sideways * 5.0f, a_IsJumping);
}





void cHorse::OnEntityAttached(cEntity & a_Attachee)
{
	super::OnEntityAttached(a_Attachee);

	if (IsPlayerControlled())
	{
		// Stop the horse and disable path finding.
		// As a player is now controlling the horse movement.
		m_PathfinderEnabled = false;
		StopMovingToPosition();
		SetSpeed(0.0, 0.0, 0.0);
	}
}





void cHorse::OnEntityDetached(cEntity & a_Detachee)
{
	super::OnEntityDetached(a_Detachee);
	m_PathfinderEnabled = true;
}





void cHorse::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	unsigned int LootingLevel = 0;
	if (a_Killer != nullptr)
	{
		LootingLevel = a_Killer->GetEquippedWeapon().m_Enchantments.GetLevel(cEnchantments::enchLooting);
	}
	AddRandomDropItem(a_Drops, 0, 2 + LootingLevel, E_ITEM_LEATHER);
	if (m_bIsSaddled)
	{
		a_Drops.push_back(cItem(E_ITEM_SADDLE, 1));
	}
}




