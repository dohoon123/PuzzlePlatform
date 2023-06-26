// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"
#include "PuzzlePlatforms/PPGameInstance.h"
#include "ServerRow.h"
#include "Components/TextBlock.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) {
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (ServerRowBPClass.Class == nullptr) return;
	ServerRowClass = ServerRowBPClass.Class;
}

void UMainMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->bShowMouseCursor = false;
}

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (HostButton == nullptr) return false;
	if (JoinButton == nullptr) return false;
	
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	CompleteJoinButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);
	
	return true;
}


void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr) {
		MenuInterface->Host();
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (JoinMenu == nullptr) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (MenuInterface != nullptr) {
		MenuInterface->RefreshServerList();
	}

}

void UMainMenu::BackToMainMenu()
{
	if (MenuSwitcher == nullptr) return;
	if (MainMenu == nullptr) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->ConsoleCommand("quit");
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ServerList->ClearChildren();

	uint32 i = 0;

	for (const FString& ServerName : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(this, ServerRowClass);
		if (Row == nullptr) return;

		Row->ServerName->SetText(FText::FromString(ServerName));
		Row->Setup(this, i);
		++i;

		ServerList->AddChild(Row);
	}
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
}

void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set."));
	}

	if (MenuInterface != nullptr)
	{
		//MenuInterface->RefreshServerList();
	}
}
