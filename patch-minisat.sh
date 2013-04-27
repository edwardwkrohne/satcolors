#
#  Minisat was written before C++11.  C++11 introduced new string literal syntax
#  that breaks a few lines in Minisat's options.  Running this script will fix those
#  files.
#

#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run as root" 1>&2
   exit 1
fi

patch -d /usr/include/minisat/utils/ <<'EOF'
--- Options.h	2013-04-26 23:02:29.000000000 -0500
+++ Options.h	2013-04-26 22:56:52.000000000 -0500
@@ -282,15 +282,15 @@
         if (range.begin == INT64_MIN)
             fprintf(stderr, "imin");
         else
-            fprintf(stderr, "%4"PRIi64, range.begin);
+            fprintf(stderr, "%4" PRIi64, range.begin);
 
         fprintf(stderr, " .. ");
         if (range.end == INT64_MAX)
             fprintf(stderr, "imax");
         else
-            fprintf(stderr, "%4"PRIi64, range.end);
+            fprintf(stderr, "%4" PRIi64, range.end);
 
-        fprintf(stderr, "] (default: %"PRIi64")\n", value);
+        fprintf(stderr, "] (default: %" PRIi64 ")\n", value);
         if (verbose){
             fprintf(stderr, "\n        %s\n", description);
             fprintf(stderr, "\n");
EOF