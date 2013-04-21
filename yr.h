#ifndef __YR_H__
#define __YR_H__

#include "http.h"


#define HOST "ar.answers.yahoo.com"
#define DIR "/dir/index?sid=---------&link=resolved&cp="

#define MAX_PAGES 50

#define SEARCH_QUESTION_TOKEN "/question/index"
#define SEARCH_QID_TOKEN "?qid="
#define SEARCH_USER "-USER-ID--"



void processPage(int sockfd, t_Buffer *document);



#endif
