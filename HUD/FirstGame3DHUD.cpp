

#include "FirstGame3D.h"
#include "FirstGame3DHUD.h"
#include "FirstGame3DGameMode.h"
#include "FirstGame3DCharacter.h"
#include "Kismet/GameplayStatics.h"

AFirstGame3DHUD::AFirstGame3DHUD(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	/* Use the robotDistanceField font from the engine */
	static ConstructorHelpers::FObjectFinder<UFont> HUDFontOb(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = HUDFontOb.Object;
}

void AFirstGame3DHUD::DrawHUD()
{
	/* get screen dimensions */
	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);

	/* call parent version of HUD */
	Super::DrawHUD();

	/* get character and print is powerlevel */
	AFirstGame3DCharacter* MyCharacter = Cast<AFirstGame3DCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	FString PowerLevelString = FString::Printf(TEXT("%10.1f"), FMath::Abs(MyCharacter->PowerLevel));
	DrawText(PowerLevelString, FColor::White, 50, 50, HUDFont);

	AFirstGame3DGameMode* MyGameMode = Cast<AFirstGame3DGameMode>(UGameplayStatics::GetGameMode(this));

	/*if game over*/
	if (MyGameMode->GetCurrentState() == EFirstGame3DPlayState::EGameOver)
	{
		/* create variable for string the size of printing game over */
		FVector2D GameOverSize;
		GetTextSize(TEXT("GAME OVER"), GameOverSize.X, GameOverSize.Y, HUDFont);
		DrawText(TEXT("GAME OVER"), FColor::White, (ScreenDimensions.X - GameOverSize.X) / 2.0f, (ScreenDimensions.Y - GameOverSize.Y) / 2.0f, HUDFont);
	}
}


