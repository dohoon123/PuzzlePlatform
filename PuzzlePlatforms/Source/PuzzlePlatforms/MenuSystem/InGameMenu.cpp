// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

void UInGameMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->bShowMouseCursor = false;
}

void UInGameMenu::SetUp()
{
	if (isOnScreen == true) return;

	isOnScreen = true;
	Super::SetUp();
}

bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (QuitButton == nullptr) return false;
	if (CancelButton == nullptr) return false;

	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::ConfirmQuit);
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::CancelQuitMenu);

	return true;
}

void UInGameMenu::CancelQuitMenu()
{
	if (isOnScreen == false) return;
	if (CancelButton == nullptr) return;

	this->RemoveFromViewport();
	isOnScreen = false;
}

void UInGameMenu::ConfirmQuit()
{
	if (MenuInterface != nullptr) {
		MenuInterface->Quit();
	}
}
