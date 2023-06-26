// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "PuzzlePlatforms/PPGameInstance.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* InputMenuInterface) {
	this->MenuInterface = InputMenuInterface;
}

void UMenuWidget::SetUp()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->bShowMouseCursor = true;
}
