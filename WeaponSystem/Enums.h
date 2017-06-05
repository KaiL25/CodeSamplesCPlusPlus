// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EFirearmType : uint8
{
	VE_Pistol	UMETA(DisplayName = "Pistol"),
	VE_SMG 	UMETA(DisplayName = "SMG"),
	VE_Shotgun	UMETA(DisplayName = "Shotgun"),
	VE_Rifle UMETA(DisplayName = "Rifle")
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EFireType : uint8
{
	VE_Semiautomatic	UMETA(DisplayName = "Semiautomatic"),
	VE_Automatic 	UMETA(DisplayName = "Automatic"),
	VE_Beam	UMETA(DisplayName = "Beam")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EWeaponNames : uint8
{
	VE_P2041	UMETA(DisplayName = "P2041"),
	VE_SMG 	UMETA(DisplayName = "SMG")	
};

/**
 * 
 */
class CYBERPUNKPROJECT_API Enums
{
public:
	Enums();
	~Enums();
};
