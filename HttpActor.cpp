//// Copywrite Shapeshifter 2019
//
#include "HttpActor.h"
//
//
//// Sets default values
AHttpActor::AHttpActor(const class FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Http = &FHttpModule::Get();
	
}
//
//// Called when the game starts or when spawned
void AHttpActor::BeginPlay()
{
	Super::BeginPlay();
}


void AHttpActor::PostHTTPCall(TArray<FDataToPost> Data)
{
	// Creating an HTTP Request
	TSharedPtr<IHttpRequest> Request = Http->CreateRequest();

	TSharedPtr<FJsonObject> JSON = MakeShareable(new FJsonObject);


	// Just checking that Array has data...
	if (Data.Num() == 0)
	{
		return;
	}
	else
	{
		for (int i = 0; i < Data.Num(); i++)
		{

		}
	}

	
	

	JSON->SetStringField("Name", "Yassine");
	JSON->SetNumberField("Health", 10);

	FString JSONString;

	// Converting the JSON To String for Post
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JSONString);
	FJsonSerializer::Serialize(JSON.ToSharedRef(), Writer);


	// Request Settings
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResponseReceived);
	Request->SetURL("http://localhost:8080/gamedata");
	Request->SetVerb("POST");

	// Printing for Debug if its converted
	UE_LOG(LogTemp, Warning, TEXT("resulting jsonString -> %s"), *JSONString);
	Request->SetContentAsString(JSONString);
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();


}

void AHttpActor::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	////Create a pointer to hold the json serialized data
	//TSharedPtr<FJsonObject> JsonObject;

	////Create a reader pointer to read the json data
	//TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	////Deserialize the json data given Reader and the actual object to deserialize
	//if (FJsonSerializer::Deserialize(Reader, JsonObject))
	//{
	//	//Get the value of the json object by field name
	//	int32 recievedInt = JsonObject->GetIntegerField("customInt");

	//	//Output it to the engine
	//	GEngine->AddOnScreenDebugMessage(1, 2.0f, FColor::Green, FString::FromInt(recievedInt));
	//}
}

