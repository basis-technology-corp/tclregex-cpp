#include "tcl_regex.h"
#include "bt_ustring.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static const char* progname = "tclgrep";

int main(int argc, char* argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s pattern file\n", progname);
        exit(1);
    }
    bt_ustring pattern;
    bt_strtoustr(pattern, argv[1]);

    regex_t re;
    int result = bt_regcomp(&re, pattern.data(), pattern.size(), REG_ADVANCED);
    if (result != REG_OKAY) {
        char buf[100];
        bt_regerror(result, &re, buf, sizeof(buf)/sizeof(buf[0]));
        fprintf(stderr, "%s: %s\n", progname, buf);
        exit(1);
    }

    if (getenv("TCLGREP_VERBOSE") != NULL) {
      printf("max captures: %d\n", (int)re.re_nsub);
    }

    if (strcmp(argv[2], "-") != 0) {
        if (freopen(argv[2], "r", stdin) == NULL) {
            fprintf(stderr, "%s: %s: %s\n", progname, argv[2], strerror(errno));
            bt_regfree(&re);
            exit(1);
        }
    }
    
    char line[1024];
    bt_ustring wline;
    while (fgets(line, sizeof(line)/sizeof(line[0]), stdin) != NULL) {
        size_t len = strlen(line);
        if (len && line[len - 1] == '\n') {
            line[--len] = '\0';
        }
        bt_strtoustr(wline, line);   
        regmatch_t matches[10];
        size_t maxMatches = sizeof(matches)/sizeof(matches[0]);
        int result = bt_regexec(&re, wline.data(), wline.size(), NULL, maxMatches, matches, 0);
        if (result == REG_OKAY) {
            printf("%s\n", line);
            if (getenv("TCLGREP_VERBOSE") != NULL) {
                for (size_t i = 0; i < maxMatches; ++i) {
                    if (matches[i].rm_so == -1) {
                        break;
                    }
                    printf("matches[%d]: [%d, %d)\n", (int)i,
               (int)matches[i].rm_so, (int)matches[i].rm_eo);
                }
            }
        }
    }

    bt_regfree(&re);

    return 0;
}
