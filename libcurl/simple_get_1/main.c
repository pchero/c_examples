#include <stdio.h>
#include <curl/curl.h>

int main(int argc, char** argv) {
  long response_code;

  printf("hello world!\n");

  CURL *curl = curl_easy_init();
  if (curl == NULL) {
    return 1;
  }

  CURLcode res;
  curl_easy_setopt(curl, CURLOPT_URL, "https://www.naver.com/sdflkdsjf");
  res = curl_easy_perform(curl);

  curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);

  printf("result: %d, response: %ld\n", res, response_code);


  // curl_easy_cleanup(curl);

  return 0;
}
