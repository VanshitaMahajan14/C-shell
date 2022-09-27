#ifndef __header__h
#define __header__h

#define Black "\033[1;30m"
#define Red "\033[1;31m"
#define Green "\033[1;32m"
#define Yellow "\033[1;33m"
#define Blue "\033[1;94m"
#define Purple "\033[1;35m"
#define Cyan "\033[1;36m"
#define White "\033[1;37m"
#define Pink "\033[1;34m"

#include <stdio.h>
#include <pwd.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>
#include <dirent.h>
#include <time.h>

#include "pwd.h" 
#include "prompt.h"
#include "echo.h"
#include "cd.h"
#include "ls.h"
#include "pinfo.h"
#include "discover.h"
#include "sys.h"

#endif
