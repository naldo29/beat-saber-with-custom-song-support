// Exercise the original WebUtils implementation through its published guest ABI.
#include <unordered_map>
#include <cstdio>
#include "WebUtils.hpp"
extern "C" __attribute__((visibility("default")))
int sc2_test_webutils(const char *url) {
    WebUtils::DownloaderUtility downloader{.userAgent="Scotland2ApplePort/0.1",.timeOut=15};
    WebUtils::URLOptions options{url,true}; // certificate and hostname verification
    auto response=downloader.Get<WebUtils::DataResponse>(std::move(options));
    auto size=response.responseData?response.responseData->size():0;
    std::fprintf(stderr,"WEBUTILS_RESPONSE curl=%d http=%d bytes=%zu verifiedTLS=1\n",response.get_CurlStatus(),response.get_HttpCode(),size);
    return response.IsSuccessful() && size?0:1;
}

extern "C" __attribute__((visibility("default")))
int sc2_test_webutils_reject_bad_tls(const char *url) {
    WebUtils::DownloaderUtility downloader{.userAgent="Scotland2ApplePort/0.1",.timeOut=10};
    WebUtils::URLOptions options{url,false}; // old callers' default must still verify HTTPS
    options.noEscape=true; // preserve the local test server's explicit port
    auto response=downloader.Get<WebUtils::DataResponse>(std::move(options));
    std::fprintf(stderr,"WEBUTILS_BAD_TLS curl=%d http=%d defaultCaller=1\n",response.get_CurlStatus(),response.get_HttpCode());
    return response.get_CurlStatus()==60 && !response.IsSuccessful()?0:1;
}
