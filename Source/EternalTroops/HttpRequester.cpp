#include "HttpRequester.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

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
