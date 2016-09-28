
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "../Entities/ArrowEntity.h"
#include "../Entities/Boat.h"
#include "../Entities/EnderCrystal.h"
#include "../Entities/Entity.h"
#include "../Entities/EntityEffect.h"
#include "../Entities/ExpBottleEntity.h"
#include "../Entities/ExpOrb.h"
#include "../Entities/FallingBlock.h"
#include "../Entities/FireChargeEntity.h"
#include "../Entities/FireworkEntity.h"
#include "../Entities/Floater.h"
#include "../Entities/GhastFireballEntity.h"
#include "../Entities/HangingEntity.h"
#include "../Entities/ItemFrame.h"
#include "../Entities/Minecart.h"
#include "../Entities/Painting.h"
#include "../Entities/Pawn.h"
#include "../Entities/Pickup.h"
#include "../Entities/Player.h"
#include "../Entities/ProjectileEntity.h"
#include "../Entities/SplashPotionEntity.h"
#include "../Entities/ThrownEggEntity.h"
#include "../Entities/ThrownEnderPearlEntity.h"
#include "../Entities/ThrownSnowballEntity.h"
#include "../Entities/TNTEntity.h"
#include "../Entities/WitherSkullEntity.h"
#include "../Mobs/IncludeAllMonsters.h"

#include "../MetadataWriter.h"

#pragma once

void cArrowEntity::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteByte(IsCritical() ? 1 : 0);  // Flags
}





void cBoat::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteInt(GetLastDamage());  // Time since last hit
	a_Writer.WriteInt(GetForwardDirection());  // Forward direction
	a_Writer.WriteFloat(GetDamageTaken());  // Damage taken
	a_Writer.WriteInt(GetType());  // Type
	a_Writer.WriteBool(IsRightPaddleUsed());  // Right paddle
	a_Writer.WriteBool(IsLeftPaddleUsed());  // Left paddle
}





void cEnderCrystal::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.SkipMeta();  // Beam target
	a_Writer.SkipMeta();  // Show bottom
}





void cEntity::WriteMetadata(cMetadataWriter & a_Writer) const
{
	Int8 Flags = 0;
	if (IsOnFire())
	{
		Flags |= 0x01;
	}
	if (IsCrouched())
	{
		Flags |= 0x02;
	}
	if (IsSprinting())
	{
		Flags |= 0x08;
	}
	if (IsRclking())
	{
		Flags |= 0x10;
	}
	if (IsInvisible())
	{
		Flags |= 0x20;
	}
	a_Writer.WriteByte(Flags);  // Flags
	a_Writer.SkipMeta();  // Air
	if (HasCustomName())
	{
		a_Writer.WriteString(GetCustomName());  // Custom name
		a_Writer.WriteBool(IsCustomNameAlwaysVisible());  // Custom name visible
	}
	else
	{
		a_Writer.SkipMeta();  // Custom name
		a_Writer.SkipMeta();  // Custom name visible
	}
	a_Writer.SkipMeta();  // Is silent
	if (a_Writer.m_ProtocolVersion >= PROTO_VERSION_1_10_0)
	{
		a_Writer.SkipMeta();  // No gravity
	}
	// Subclasses add aditional metadata fields
}





void cFallingBlock::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WritePosition(m_OriginalPosition.x, m_OriginalPosition.y, m_OriginalPosition.z);  // Spawn position
}





void cFireworkEntity::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteItem(GetItem());  // Firework item
}





void cFloater::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	// Hooked entity ID
	// 0 if none, ID + 1 if attached
	if (GetAttachedMobID() == cEntity::INVALID_ID)
	{
		a_Writer.WriteInt(0);  // No attached entity
	}
	else
	{
		a_Writer.WriteInt(static_cast<Int32>(GetAttachedMobID() + 1));  // Attached entity ID
	}
}





void cItemFrame::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteItem(GetItem());  // Item in frame
	a_Writer.WriteInt(GetItemRotation());  // Item rotation
}





void cMinecart::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	// The following expression makes Minecarts shake more with less health or higher damage taken
	a_Writer.WriteInt((GetMaxHealth() - GetHealth()) * LastDamage() * 4);
	a_Writer.WriteInt(1);  // Shaking direction
	a_Writer.WriteFloat(static_cast<float>(LastDamage() + 10));  // Shaking multiplier

	// TODO: Move these flags to cMinecart, since they are on all minecarts (#3343)
	if (GetPayload() == cMinecart::mpNone)
	{
		auto RideableMinecart = reinterpret_cast<const cRideableMinecart *>(this);
		const cItem & MinecartContent = RideableMinecart->GetContent();
		if (!MinecartContent.IsEmpty())
		{
			int Content = MinecartContent.m_ItemType;
			Content |= MinecartContent.m_ItemDamage << 8;
			a_Writer.WriteInt(Content);  // Block ID and damage
			a_Writer.WriteInt(RideableMinecart->GetBlockHeight());  // Block height
			a_Writer.WriteBool(true);  // Show block
		}
		else
		{
			a_Writer.SkipMeta();  // Block ID and damage
			a_Writer.SkipMeta();  // Block height
			a_Writer.SkipMeta();  // Show block
		}
	}
	else
	{
		a_Writer.SkipMeta();  // Block ID and damage
		a_Writer.SkipMeta();  // Block height
		a_Writer.SkipMeta();  // Show block
	}
}





void cMinecartWithFurnace::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteBool(IsFueled());  // Is powered
}





void cPawn::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	// Living flags
	a_Writer.SkipMeta();  // Hand flags
	a_Writer.WriteFloat(static_cast<float>(GetHealth()));  // Health
	a_Writer.SkipMeta();  // Potion effect color
	a_Writer.SkipMeta();  // Potion effect ambient
	a_Writer.SkipMeta();  // Number of arrows
	// Insentient flags
	a_Writer.SkipMeta();  // NoAI, left-handed
}





void cPickup::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteItem(GetItem());  // Item
}





void cPlayer::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	// Player-specific fields (none currently implemented)
	a_Writer.SkipMeta();  // Aditional hearts
	a_Writer.SkipMeta();  // Score
	a_Writer.SkipMeta();  // Skin flags
	a_Writer.SkipMeta();  // Main hand
}





void cSplashPotionEntity::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteItem(GetItem());  // Potion item
}





void cTNTEntity::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteInt(GetFuseTicks());  // Fuse time
}





void cWitherSkullEntity::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.SkipMeta();  // Invulnerable / blue
}





void cBat::WriteMetadata(cMetadataWriter & a_Writer) const
{
	// super isn't used here because that refers to cPassiveMonster, which contains the is baby flag.
	// Since bat doesn't extend Ageable but does extend insentient, we need to skip the ageable metadata.
	// This is an odd special case that is caused by the differences between vanilla's entity hierarchy and
	// Cuberite's entity hierarchy.
	cMonster::WriteMetadata(a_Writer);
	a_Writer.WriteByte(IsHanging() ? 1 : 0);  // Flags
}





void cBlaze::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.SkipMeta();  // Is on fire / preparing to shoot
}





void cCreeper::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteInt(IsBlowing() ? 1 : -1);  // State
	a_Writer.WriteBool(IsCharged());  // Is charged
	a_Writer.WriteBool(IsBurnedWithFlintAndSteel());  // Is burned with flint and steel
}





void cEnderDragon::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.SkipMeta();  // 1.9 battle phase
}





void cEnderman::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteBlockType(GetCarriedBlock(), GetCarriedMeta());  // Carried block
	a_Writer.WriteBool(IsScreaming());  // Is screaming
}





void cGhast::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteBool(IsCharging());  // Is shooting
}





void cGuardian::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.SkipMeta();  // Flags
	a_Writer.SkipMeta();  // Target
}





void cHorse::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	Int8 Flags = 0;
	if (IsTame())
	{
		Flags |= 0x02;
	}
	if (IsSaddled())
	{
		Flags |= 0x04;
	}
	if (IsChested())
	{
		Flags |= 0x08;
	}
	if (IsEating())
	{
		Flags |= 0x20;
	}
	if (IsRearing())
	{
		Flags |= 0x40;
	}
	if (IsMthOpen())
	{
		Flags |= 0x80;
	}
	a_Writer.WriteByte(Flags);  // Flags
	a_Writer.WriteInt(GetHorseType());  // Type
	int Appearance = 0;
	Appearance |= GetHorseColor();
	Appearance |= GetHorseStyle() << 8;
	a_Writer.WriteInt(Appearance);  // Appearence
	a_Writer.SkipMeta();  // Owner
	a_Writer.WriteInt(GetHorseArmour());  // Armor
}





void cIronGolem::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.SkipMeta();  // Is player created
}





void cMagmaCube::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	// Note: This is slime metadata
	a_Writer.WriteInt(GetSize());  // Size
}





void cPassiveMonster::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteBool(IsBaby());  // Is baby
}





void cPig::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteBool(IsSaddled());  // Has saddle
}





void cRabbit::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteInt(static_cast<Int32>(GetRabbitType()));  // Rabit type
}





void cSheep::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	Int8 SheepMetadata = 0;
	SheepMetadata = static_cast<Int8>(GetFurColor());
	if (IsSheared())
	{
		SheepMetadata |= 0x10;
	}
	a_Writer.WriteByte(SheepMetadata);  // Sheep flags (wool & sheered)
}





void cSkeleton::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteInt(IsWither() ? 1 : 0);  // Type (TODO: Stray)
	a_Writer.SkipMeta();  // Is swinging arms
}





void cSlime::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteInt(GetSize());  // Size
}





void cSnowGolem::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.SkipMeta();  // Is pumpkin removed
}





void cSpider::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.SkipMeta();  // Is climbing
}





void cVillager::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteInt(GetVilType());  // Villager profession
}





void cWitch::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteBool(IsAngry());  // Is aggressive
}





void cWither::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.SkipMeta();  // Center head target
	a_Writer.SkipMeta();  // Left head target
	a_Writer.SkipMeta();  // Right head target
	a_Writer.WriteInt(static_cast<Int32>(GetWitherInvulnerableTicks()));
}





void cWolf::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);

	// Ageable fields (since wolf inherits cPassiveAgressiveMonster instead of cPassiveMonster, this isn't handled directly)
	a_Writer.WriteBool(IsBaby());

	// Tameable fields
	Int8 WolfStatus = 0;
	if (IsSitting())
	{
		WolfStatus |= 0x1;
	}
	if (IsAngry())
	{
		WolfStatus |= 0x2;
	}
	if (IsTame())
	{
		WolfStatus |= 0x4;
	}
	a_Writer.WriteByte(WolfStatus);  // Status
	a_Writer.SkipMeta();  // Owner

	// Wolf flags
	a_Writer.WriteFloat(static_cast<float>(GetHealth()));  // Damage taken / tail rotation
	a_Writer.WriteBool(IsBegging());  // Is begging
	a_Writer.WriteInt(GetCollarColor());  // Collar color
}





void cZombie::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteBool(IsBaby());  // Is baby
	// TODO: This actually encodes the zombie villager profession, but that isn't implemented yet.
	a_Writer.WriteInt(IsVillagerZombie() ? 1 : 0);  // Profession
	a_Writer.WriteBool(IsConverting());  // Is converting
	a_Writer.SkipMeta();  // Are hands held up
}





void cZombiePigman::WriteMetadata(cMetadataWriter & a_Writer) const
{
	super::WriteMetadata(a_Writer);
	a_Writer.WriteBool(IsBaby());  // Is baby
	// Note: Zombie pigmen inherit zombie metadata
	a_Writer.SkipMeta();  // Zombie type / profession
	a_Writer.SkipMeta();  // Is converting
	a_Writer.SkipMeta();  // Are hands up
}
