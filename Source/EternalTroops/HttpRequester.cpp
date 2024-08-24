#include "HttpRequester.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "UObject/ConstructorHelpers.h"

FString UHttpRequester::GetRequest(const FString& url, const FString& uaid, const FString& sign)
{
    // Создаем новый HTTP-запрос
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

    // Устанавливаем URL для запроса
    HttpRequest->SetURL(url);

    // Устанавливаем метод запроса (GET)
    HttpRequest->SetVerb(TEXT("GET"));

    // Добавляем необходимые заголовки
    HttpRequest->SetHeader(TEXT("uaid"), uaid);
    HttpRequest->SetHeader(TEXT("sign"), sign);

    // Переменная для хранения ответа
    FString ResponseContent;

    // Обработка завершения запроса
    HttpRequest->OnProcessRequestComplete().BindLambda([&ResponseContent](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
        {
            if (bWasSuccessful && Response.IsValid())
            {
                // Сохраняем ответ
                ResponseContent = Response->GetContentAsString();
            }
            else
            {
                // В случае ошибки сохраняем сообщение об ошибке
                ResponseContent = TEXT("Request failed or invalid response.");
            }
        });

    // Выполняем запрос
    HttpRequest->ProcessRequest();

    // Ждем завершения запроса
    while (HttpRequest->GetStatus() != EHttpRequestStatus::Succeeded && HttpRequest->GetStatus() != EHttpRequestStatus::Failed)
    {
        FPlatformProcess::Sleep(0.01f);
    }

    // Возвращаем ответ
    return ResponseContent;
}

FString UHttpRequester::TestGet()
{
    /*FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &UHttpRequester::OnResponseReceived);
      Request->SetURL("https://vkplay.ru/app/34638/user/profile");
      Request->SetVerb("GET");
      //Request->SetHeader("uaid", "267257659");
      //Request->SetHeader("sign", "beffd5ed92eaede2d1ed8f60f6d9ef85");
      Request->ProcessRequest();*/



    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    //TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
    //RequestObj->SetStringField("uaid", "267257659");
    //RequestObj->SetStringField("sign", "beffd5ed92eaede2d1ed8f60f6d9ef85");

    FString RequestBody = "fuck";
    //TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    //FJsonSerializer::Serialize(RequestObj, Writer);

    Request->OnProcessRequestComplete().BindUObject(this, &UHttpRequester::OnResponseReceived);
    Request->SetURL("https://vkplay.ru/app/34638/user/profile");
    Request->SetVerb("POST");
    //Request->SetHeader("Content-Type", "application/json");
    Request->SetContentAsString(RequestBody);
    Request->ProcessRequest();


    return FString();
}



void UHttpRequester::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!bConnectedSuccessfully)
    {
        UE_LOG(LogTemp, Error, TEXT("Request failed"));
    }
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Response is invalid"));
    }
    if (!Response.IsValid() || !bConnectedSuccessfully)
    {
        return;
    }
    

    TSharedPtr<FJsonObject> ResponseObj;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    if (FJsonSerializer::Deserialize(Reader, ResponseObj))
    {
        UE_LOG(LogTemp, Display, TEXT("Response %s"), *Response->GetContentAsString());
        UE_LOG(LogTemp, Display, TEXT("Title: %s"), *ResponseObj->GetStringField("title"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response"));
    }
}