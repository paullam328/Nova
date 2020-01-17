// Copywrite Shapeshifter 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniLeaderboardWidget.generated.h"

/**
 * 
 */

class UTextBlock;

USTRUCT()
struct FMiniLeaderboardInfo
{
	GENERATED_BODY()

	UPROPERTY()
		FString Name;

	UPROPERTY()
		int32 Health;

	//Set
	void SetName(const FString name)
	{
		Name = name;
	}

	//Set
	void SetHealth(const int32 health)
	{
		Health = health;
	}

	//Get
	FString GetName()
	{
		return Name;
	}

	//Get
	int32 GetHealth()
	{
		return Health;
	}

	//Constructor
	FMiniLeaderboardInfo()
	{
		//Always initialize your USTRUCT variables!
		//exception is if you know the variable type has its own default constructor

		Name = "";
		Health = 0;
	}
};

USTRUCT()
struct FMiniLeaderboardInfoPredicate
{
	GENERATED_BODY()
	//Larger goes first...
	bool operator() (const FMiniLeaderboardInfo& A, const FMiniLeaderboardInfo& B) const
	{
		return A.Health > B.Health;
	}
};

UCLASS()
class NOVAFINAL_API UMiniLeaderboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UTextBlock* Rank1Text;
	UTextBlock* Rank2Text;
	UTextBlock* Rank3Text;
	UTextBlock* Rank4Text;
	UTextBlock* Rank5Text;
	UTextBlock* Rank6Text;

	TArray<UTextBlock*> RankTexts;

	UTextBlock* Rank11Text;
	UTextBlock* Rank21Text;
	UTextBlock* Rank31Text;
	UTextBlock* Rank41Text;
	UTextBlock* Rank51Text;
	UTextBlock* Rank61Text;

	TArray<UTextBlock*> RankTexts1;

	UTextBlock* Rank12Text;
	UTextBlock* Rank22Text;
	UTextBlock* Rank32Text;
	UTextBlock* Rank42Text;
	UTextBlock* Rank52Text;
	UTextBlock* Rank62Text;

	TArray<UTextBlock*> RankTexts2;

	UTextBlock* Username1Text;
	UTextBlock* Username2Text;
	UTextBlock* Username3Text;
	UTextBlock* Username4Text;
	UTextBlock* Username5Text;
	UTextBlock* Username6Text;

	TArray<UTextBlock*> UsernameTexts;

	UTextBlock* Username11Text;
	UTextBlock* Username21Text;
	UTextBlock* Username31Text;
	UTextBlock* Username41Text;
	UTextBlock* Username51Text;
	UTextBlock* Username61Text;

	TArray<UTextBlock*> UsernameTexts1;

	UTextBlock* Username12Text;
	UTextBlock* Username22Text;
	UTextBlock* Username32Text;
	UTextBlock* Username42Text;
	UTextBlock* Username52Text;
	UTextBlock* Username62Text;

	TArray<UTextBlock*> UsernameTexts2;

	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	virtual void NativeConstruct() override;

	void UpdateLeaderboardRankings();

	void SortUpdateMiniLeaderboard();

	//FMiniLeaderboardInfo MiniLeaderboardInfo;
	TArray<FMiniLeaderboardInfo> MiniLeaderboardInfoPriorityHeap;

	FString PlayerName;

};
