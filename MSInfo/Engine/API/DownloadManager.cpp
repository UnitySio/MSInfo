﻿#include "DownloadManager.h"

#include "curl/curl.h"

void DownloadManager::DownloadFile(const std::string& url, const std::string& file_name)
{
    CURL* curl = curl_easy_init();
    if (curl)
    {
        FILE* fp = nullptr;
        fopen_s(&fp, file_name.c_str(), "wb");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

        CURLcode res_code = curl_easy_perform(curl);

        fclose(fp);
        
        curl_easy_cleanup(curl);
    }
}

size_t DownloadManager::WriteCallback(void* contents, size_t size, size_t nmemb, FILE* userp)
{
    return fwrite(contents, size, nmemb, userp);
}
