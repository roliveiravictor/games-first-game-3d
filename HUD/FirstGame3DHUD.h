

#pragma once

#include "GameFramework/HUD.h"
#include "FirstGame3DHUD.generated.h"

/**
 * 
 */
UCLASS()
class AFirstGame3DHUD : public AHUD
{
	GENERATED_UCLASS_BODY()

	/* Variable for storing the font */
	UPROPERTY()
	UFont* HUDFont;
	
	/* Primary draw call for the HUD */
	virtual void DrawHUD() OVERRIDE;
};
