#include <stdio.h>
#include <string.h>
#include "route_io.h"

#define rio_check_http_header_structure(request) \
if( (request->out_buff == NULL) || strncmp( (const char*) request->out_buff->start, "HTTP/1.1 ", 9) != 0) { \
  fprintf(stderr, "%s\n", "Invalid Http header, no http status code on top");\
  return -1;\
}

#define rio_check_http_if_content_length(request, len) \
if(! rio_memstr( request->out_buff->start, request->out_buff->end, "Content-Length:")) { \
char content_len_str[50]; \
snprintf(content_len_str, 50, "Content-Length: %zu", len); \
rio_write_http_header_2(request, content_len_str); \
}

unsigned char*
rio_memstr(unsigned char * start, unsigned char *end, char *pattern) {
  size_t len = end - start, ptnlen = strlen(pattern);
  if (len) {
    while ( (start = 
      (unsigned char*) memchr(start, 
        (int)pattern[0], 
        len) ) ) {
      len = end - start;
      if (len >= ptnlen) {
        if ( memcmp(start, pattern, ptnlen) == 0) {
          return start;
        }
        start++;
        len--;
        continue;
      }
      return NULL;
    }
  }
  return NULL;
}

int
rio_write_http_status(rio_request_t * request, int statuscode) {
  if (request->out_buff) {
    fprintf(stderr, "%s\n", "Invalid Http status code should be write first"); \
    return -1;
  }

  switch (statuscode) {
  case 100:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 100 Continue");
    break;
  case 101:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 101 Switching Protocols");
    break;
  case 200:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 200 Ok");
    break;
  case 201:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 201 Created");
    break;
  case 202:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 202 Accepted");
    break;
  case 203:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 203 Partial Information");
    break;
  case 204:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 204 No Response");
    break;
  case 205:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 205 Reset Content");
    break;
  case 206:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 206 Partial Content");
    break;
  case 301:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 301 Moved");
    break;
  case 302:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 302 Found");
    break;
  case 303:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 303 Method");
    break;
  case 304:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 304 Not Modified");
    break;
  case 305:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 305 Use Proxy");
    break;
  case 400:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 400 Bad Request");
    break;
  case 401:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 401 Unauthorized");
    break;
  case 402:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 402 Payment Required");
    break;
  case 403:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 403 Forbidden");
    break;
  case 404:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 404 Not Found");
    break;
  case 405:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 405 Method Not Allowed");
    break;
  case 406:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 406 Not Acceptable");
    break;
  case 407:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 407 Proxy Authentication Required");
    break;
  case 408:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 408 Request Timeout");
    break;
  case 409:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 409 Conflict");
    break;
  case 410:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 410 Gone");
    break;
  case 411:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 411 Length Required");
    break;
  case 412:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 412 Precondition Failed");
    break;
  case 413:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 413 Request Entity Too Large");
    break;
  case 414:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 414 Request URI Too Large");
    break;
  case 415:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 415 Unsupported Media Type");
    break;
  case 416:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 416 Requested Rage Not Satisfiable");
    break;
  case 417:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 417 Expectation Failed");
    break;
  case 500:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 500 Internal Error");
    break;
  case 501:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 501 Not Implemented");
    break;
  case 502:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 502 Service temporarily overloaded");
    break;
  case 503:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 503 Service Unavailable");
    break;
  case 504:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 504 Gateway Timeout");
    break;
  case 505:
    rio_write_output_buffer(request, (unsigned char*)"HTTP/1.1 505 Http Version Not Supported");
    break;
  default:
    fprintf(stderr, "%s\n", "No http status code found, please write out customly");
    return -1;
  }
  rio_write_output_buffer(request, (unsigned char*)"\r\n");
  return 0;
}

int
rio_write_http_header(rio_request_t * request, char* key, char *val) {
  rio_check_http_header_structure(request);
  rio_write_output_buffer(request, (unsigned char*) key);
  rio_write_output_buffer(request, (unsigned char*) ": ");
  rio_write_output_buffer(request, (unsigned char*) val);
  rio_write_output_buffer(request, (unsigned char*) "\r\n");
  return 0;
}

int
rio_write_http_header_2(rio_request_t * request, char* keyval) {
  rio_check_http_header_structure(request);
  rio_write_output_buffer(request, (unsigned char*) keyval);
  rio_write_output_buffer(request, (unsigned char*) "\r\n");
  return 0;
}


int
rio_write_http_header_3(rio_request_t * request, char* keyval, size_t len) {
  rio_check_http_header_structure(request);
  rio_write_output_buffer_l(request, (unsigned char*) keyval, len);
  rio_write_output_buffer(request, (unsigned char*) "\r\n");
  return 0;
}

int
rio_write_http_content(rio_request_t * request, char* content) {
  size_t len;
  rio_check_http_header_structure(request);
  len = strlen(content);
  rio_check_http_if_content_length(request, len);
  rio_write_output_buffer(request, (unsigned char*) "\r\n");
  rio_write_output_buffer_l(request, (unsigned char*) content, len);
  return 0;
}

int
rio_write_http_content_2(rio_request_t * request, char* content, size_t len) {
  rio_check_http_header_structure(request);
  rio_check_http_if_content_length(request, len);
  rio_write_output_buffer(request, (unsigned char*) "\r\n");
  rio_write_output_buffer_l(request, (unsigned char*) content, len);
  return 0;
}

void
rio_http_getpath(rio_request_t *req, rio_buf_t *buf) {
  size_t buflen;
  rio_buf_t *pbuf = req->in_buff;
  if (pbuf) {
    buflen = pbuf->end - pbuf->start;
    if ( (buf->start = (unsigned char*) memchr(pbuf->start, '/', buflen) ) && (buf->end = (unsigned char*) memchr(buf->start, ' ', pbuf->end - buf->start )) ) {
      buf->total_size = buf->end - buf->start;
      return;
    }
  }
  buf->total_size = 0;
}

void
rio_http_getbody(rio_request_t *req, rio_buf_t *buf) {
  rio_buf_t *pbuf = req->in_buff;
  if (pbuf) {
    if ( (buf->start = rio_memstr(pbuf->start, pbuf->end, "\r\n\r\n") ) || (buf->start = rio_memstr(pbuf->start, pbuf->end, "\n\n") )) {
      buf->end = pbuf->end;
      buf->total_size = buf->end - buf->start;
      return;
    }
  }
  buf->total_size = 0;
}