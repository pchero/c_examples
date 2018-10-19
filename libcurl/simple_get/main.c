#include <stdio.h>
#include <curl/curl.h>
 
int main(void)
{
  CURL *curl;
  CURLcode res;
  char *location;
  long response_code;
long http_code;

 
  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://wa01.highside.net/sox/sotiris.wav");

    //
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 180);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "asterisk-libcurl-agent/1.0");
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	// curl_easy_setopt(curl, CURLOPT_URL, ast_sorcery_object_get_id(cb_data->bucket_file));
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, NULL);



 
    // /* example.com is redirected, figure out the redirection! */ 
 
    // /* Perform the request, res will get the return code */ 
    // res = curl_easy_perform(curl);
    // /* Check for errors */ 
    // if(res != CURLE_OK)
    //   fprintf(stderr, "curl_easy_perform() failed: %s\n",
    //           curl_easy_strerror(res));
    // else {
    //   res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    //   if((res == CURLE_OK) &&
    //      ((response_code / 100) != 3)) {
    //     /* a redirect implies a 3xx response code */ 
    //     fprintf(stderr, "Not a redirect.\n");
    //   }
    //   else {
    //     res = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);
 
    //     if((res == CURLE_OK) && location) {
    //       /* This is the new absolute URL that you could redirect to, even if
    //        * the Location: response header may have been a relative URL. */ 
    //       printf("Redirected to: %s\n", location);
    //     }
    //   }
    // }

    int ret;
    res = curl_easy_perform(curl);


    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  return 0;
}