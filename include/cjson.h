#ifndef VIRMAT_CJSON_H
#define VIRMAT_CJSON_H

#include "cJSON/cJSON.h"
void cJsonInit();
cJSON *parseJsonFile(FILE *file);

#endif //VIRMAT_CJSON_H
